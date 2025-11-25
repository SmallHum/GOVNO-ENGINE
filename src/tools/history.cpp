#include <core/history.h>

namespace editor{
    shared_ptr<Node> node_root;
    shared_ptr<Node> selection;

    namespace history{
        OnANode::OnANode(
            shared_ptr<Node> selected_on_action
        ): Action(),
            selected_on_action(selected_on_action)
        {
            failed = failed || !this->selected_on_action;
            if(!this->selected_on_action){
                cout << failed << " SELECTED SOMEHOW DOESN'T SATISFY (OnANode constructor)\n";
            }
        }

        AddNode::AddNode(
            shared_ptr<Node> added,
            shared_ptr<Node> selected_on_action
        ): OnANode(selected_on_action),
            added(added)
        {
            if(failed){
                cout << "SETTING SELECTED AS NODE ROOT...\n";
                failed = 0;
                this->selected_on_action = node_root;
            }
            failed = failed || !added;
            if(!added){
                cout << failed << " ADDED SOMEHOW DOESN'T SATISFY (AddNode constructor)\n";
            }
        }

        void AddNode::execute(){
            selected_on_action->addChild(added);
        }
        void AddNode::undo(){
            added->removeGently();
        }

        DeleteNode::DeleteNode(
            shared_ptr<Node> selected_on_action
        ): OnANode(selected_on_action)
        {
            if(failed)return;

            this->parent = this->selected_on_action->parent.lock();
            del_index = this->selected_on_action->parent_index;

            failed = failed || !this->parent;
            if(!this->parent){
                cout << failed << " PARENT SOMEHOW DOESN'T SATISFY (DeleteNode constructor)\n";
            }
        }

        void DeleteNode::execute(){
            // cout << "action execute begins\n";
            parent->removeChild(selected_on_action);
            // cout << "called removechild succesfully\n";

            selection.reset();
        }
        void DeleteNode::undo(){
            parent->addChild(selected_on_action, del_index);
        }

        size_t history_size;
        Action **undo_data,
                **redo_data;

        size_t undo_end,
                redo_end;

        void init(size_t size){
            history_size = size;

            undo_end = redo_end = 0;

            undo_data = new Action*[size];
            redo_data = new Action*[size];
        }

        void exit(){
            for(size_t i = 0; i < undo_end; i++) delete undo_data[i];
            for(size_t i = 0; i < redo_end; i++) delete redo_data[i];
            delete[] undo_data;
            delete[] redo_data;
            cout << "EXITED HISTORY SUCCESSFULLY\n";
        }

        void logData(){
            cout << "UNDO DATA:\n";
            for(int i = 0; i < undo_end; i++){
                cout << ((size_t)undo_data[i])%0x1000 << ' ';
            }
            cout << '\n';
            cout << "REDO DATA:\n";
            for(int i = 0; i < redo_end; i++){
                cout << ((size_t)redo_data[i])%0x1000 << ' ';
            }
            cout << "\n---------\n";
        }

        void act(Action *a){
            if(a->failed){
                cout << "FAILED (editor::history::act)\n";
                delete a;
                cout << "ACTION OBJECT DELETED (editor::history::act)\n";
                return;
            }

            if(undo_end == history_size){
                delete undo_data[0];
                for(size_t i = 1; i < history_size; i++){
                    undo_data[i-1] = undo_data[i];
                }
            }

            undo_data[undo_end++] = a;
            a->execute();

            for(size_t i = 0; i < redo_end; i++)delete redo_data[i];
            redo_end = 0;

            // logData();
        }

        void undo(){
            if(undo_end == 0){
                cout << "I EITHER FORGOR OR THERE'S NOTHING TO BE UNDONE\n";
                return;
            }
            // cout << "trying to undo...\n";
            undo_data[undo_end-1]->undo();
            // cout << "trying to fetch undo top to redo...\n";
            redo_data[redo_end++] = undo_data[--undo_end];

            // logData();
        }

        void redo(){
            if(redo_end == 0){
                cout << "I EITHER FORGOR OR THERE'S NOTHING TO BE REDONE\n";
                return;
            }
            // cout << "trying to redo...\n";
            redo_data[redo_end-1]->execute();
            // cout << "trying to fetch redo top back to undo...\n";
            undo_data[undo_end++] = redo_data[--redo_end];

            // logData();
        }
    };
};