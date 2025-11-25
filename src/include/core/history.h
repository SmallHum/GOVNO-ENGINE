#include <core/controls.h>
#include <core/assets.h>
#include <core/viewport.h>
#include <struct_loader.h>

#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>

namespace editor{
    extern shared_ptr<Node> node_root;
    extern shared_ptr<Node> selection;

    namespace history{
        struct Action{
            bool failed = 0;

            Action(){}

            virtual void execute(){}
            virtual void undo(){}
        };

        struct OnANode: Action{
            shared_ptr<Node> selected_on_action;

            OnANode(shared_ptr<Node> selected_on_action = selection);
        };

        struct AddNode: OnANode{
            shared_ptr<Node> added;

            AddNode(
                shared_ptr<Node> added,
                shared_ptr<Node> selected_on_action = selection
            );

            void execute() override;
            void undo() override;
        };

        struct DeleteNode: OnANode{
            shared_ptr<Node> parent;
            size_t del_index;

            DeleteNode(shared_ptr<Node> selected_on_action = selection);

            void execute() override;
            void undo() override;
        };

        template <typename T>
        struct Apply: OnANode{
            T before;
            T after;
            T &link;

            Apply(
                T before, T after, T &link,
                shared_ptr<Node> selected_on_action = selection
            ): OnANode(selected_on_action),
                before(before),
                after(after),
                link(link)
            {}

            void execute() override{
                link = after;
            }
            void undo() override{
                link = before;
            }
        };

        extern size_t history_size;
        extern Action **undo_data,
                **redo_data;

        extern size_t undo_end,
                redo_end;

        void init(size_t size);
        void exit();

        void logData();

        void act(Action *a);

        void undo();
        void redo();
    };
};