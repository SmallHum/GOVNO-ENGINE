#define EDITOR

#include <core/controls.h>
#include <core/assets.h>
#include <core/viewport.h>
#include <struct_loader.h>

#include <imgui/imgui.h>
#include <imgui/imgui_stdlib.h>
#include <imgui/imgui-SFML.h>

#define CTRL_DOWN (ImGui::IsKeyDown(ImGuiKey_LeftCtrl) || \
        ImGui::IsKeyDown(ImGuiKey_RightCtrl))

#define SHIFT_DOWN (ImGui::IsKeyDown(ImGuiKey_LeftShift) || \
        ImGui::IsKeyDown(ImGuiKey_RightShift))

#define PRESSED(K) ImGui::IsKeyPressed(K)
#define DOWN(K) ImGui::IsKeyDown(K)

#define SHIFT(K) (SHIFT_DOWN && PRESSED(K))
#define CTRL(K) (CTRL_DOWN && PRESSED(K))

#define SHIFT_AND(COND) (SHIFT_DOWN && (COND))
#define CTRL_AND(COND) (CTRL_DOWN && (COND))

using ImGui::MenuItem,
        ImGui::Separator;

namespace editor{

    shared_ptr<Node> node_root;
    shared_ptr<Node> selection;
    size_t selection_depth = 0;

    namespace history{
        struct Action{
            bool failed = 0;

            Action(){}

            virtual void execute(){}
            virtual void undo(){}
        };

        struct OnANode: Action{
            shared_ptr<Node> selected;

            OnANode(
                shared_ptr<Node> sel = selection
            ): Action(),
                selected(sel)
            {
                failed = failed || !selected;
                if(!selected){
                    cout << "SELECTED SOMEHOW DOESN'T SATISFY (OnANode constructor)\n";
                }
            }
        };

        struct AddNode: OnANode{

            shared_ptr<Node> added;

            AddNode(
                shared_ptr<Node> added,
                shared_ptr<Node> sel = selection
            ): OnANode(sel),
                added(added)
            {
                failed = failed || !added;
                if(!added){
                    cout << "ADDED SOMEHOW DOESN'T SATISFY (AddNode constructor)\n";
                }
            }

            void execute() override{
                selected->addChild(added);
            }
            void undo() override{
                added->removeGently();
            }
        };

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
                delete a;
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

        void exit(){
            for(size_t i = 0; i < undo_end; i++) delete undo_data[i];
            for(size_t i = 0; i < redo_end; i++) delete redo_data[i];
            delete[] undo_data;
            delete[] redo_data;
        }
    };

    inline bool newWindow(
        const char *label,
        ImVec2 pos,
        ImVec2 size,
        ImVec2 min_size,
        ImVec2 max_size,
        ImGuiWindowFlags flags
    ){
        ImGui::SetNextWindowPos(pos);
        ImGui::SetNextWindowSize(size);
        ImGui::SetNextWindowSizeConstraints(min_size, max_size);
        return ImGui::Begin(label, nullptr, flags);
    }

    void init(){
        node_root = make_shared<Node>("root");
        node_root->printTree();

        selection = node_root;

        history::init(64);
    }

    void exit(){
        history::exit();
    }

    void open(){
        string path = "saved.ntr";
        if(!node_root){
            cout << "HOW DID YOU DELETE THE ROOT LMAO\n"; return;
        }
        node_root = constructFromFile(path);
    }

    void save(){
        string path = "saved.ntr";
        if(!node_root){
            cout << "HOW DID YOU DELETE THE ROOT LMAO\n"; return;
        }
        node_root->writeToFile(path);
    }

    void editFieldBool(string label, bool &v){
        ImGui::Checkbox(label.c_str(), &v);
    }

    void editFieldFloat(string label, float &v){
        float buffer = v;

        if(!ImGui::InputFloat(label.c_str(), &buffer))
            return;

        if(!PRESSED(ImGuiKey_Enter))
            return;

        cout << "    applying " << label << '\n';
        v = buffer;
    }

    void editFieldV2F(string label, v2f &v){
        float buffer[2] = {v.x, v.y};

        if(!ImGui::InputFloat2(label.c_str(), buffer))
            return;
        if(!PRESSED(ImGuiKey_Enter))
            return;

        cout << "    applying " << label << '\n';
        v.x = buffer[0];
        v.y = buffer[1];
    }

    void editFieldString(string label, string &v, bool multiline = 0){
        string buffer = v;

        if(multiline && !ImGui::InputTextMultiline(label.c_str(), &buffer))
                return;
        else if(!ImGui::InputText(label.c_str(), &buffer))
                return;
        
        if(!PRESSED(ImGuiKey_Enter))
                return;

        cout << "    applying " << label << '\n';
        v.resize(strlen(buffer.c_str()));
        v = buffer;
    }

    void LabelEditMenu(Label *node){
        ImGui::SeparatorText("Label");

        editFieldString("Text", node->text, true);
    }

    void AABBEditMenu(AABB *node){
        ImGui::SeparatorText("AABB");

        editFieldFloat("Top", node->top);
        editFieldFloat("Bottom", node->bottom);
        editFieldFloat("Left", node->left);
        editFieldFloat("Right", node->right);
    }

    void spatialEditMenu(Spatial *node){
        ImGui::SeparatorText("Spatial");

        editFieldV2F("Position", node->pos);
        editFieldFloat("Angle", node->angle);
        editFieldV2F("Scale", node->scale);

        auto aabb = dynamic_cast<AABB*>(node);
        if(aabb)AABBEditMenu(aabb);

        auto label = dynamic_cast<Label*>(node);
        if(label)LabelEditMenu(label);
    }

    void nodeEditMenu(Node *node){
        ImGui::SeparatorText("Node");

        editFieldString("Name", node->name);
        editFieldBool("Visible", node->visible);

        auto s = dynamic_cast<Spatial*>(node);
        if(s)spatialEditMenu(s);
    }

    int id_fix = 0;
    int sel_margin = 20;

    bool any_editor_window_focused = 0,
        controlling_camera = 0;

    v2f cam_pos = {-480.f, -360.f};

    float tree_view_width = 240,
        actions_width = 160,
        bar_height_button_size = 26,
        edit_width = 240,
        edit_pos_x, menu_aware_height;

    float wheel_delta_x, wheel_delta_y;

    shared_ptr<Node> getHovered(shared_ptr<Node> root, size_t depth = 0){
        Spatial *sp = dynamic_cast<Spatial *>(root.get());

        if(sp && selection_depth < depth){
            auto m = ImGui::GetMousePos();
            if((v2f(m.x,m.y) - sp->getGlobalPos() + cam_pos).length() <= sel_margin){
                selection_depth = depth + 1;
                return root;
            }
        }

        for(auto &i : root->children){
            auto result = getHovered(i, depth + 1);
            if(result)return result;
        }

        selection_depth = 0;
        return shared_ptr<Node>();
    }

    void menuProc(float &dt){
        if(ImGui::BeginMainMenuBar()){
            if(ImGui::BeginMenu("File")){

                if(MenuItem("Open", "Ctrl+O"))open();
                if(MenuItem("Save", "Ctrl+S"))save();

                ImGui::EndMenu();
            }

            if(ImGui::BeginMenu("Edit")){
                if(MenuItem("Delete Node", "Del")){}

                Separator();

                if(MenuItem("Copy", "Ctrl+C")){}
                if(MenuItem("Paste", "Ctrl+V")){}

                Separator();

                if(MenuItem("Undo", "Ctrl+Z"))history::undo();
                if(MenuItem("Redo", "Ctrl+Y"))history::redo();

                ImGui::EndMenu();
            }

            ImGui::EndMainMenuBar();
        }

        if(CTRL(ImGuiKey_O))open();
        else if(CTRL(ImGuiKey_S))save();
        else if(PRESSED(ImGuiKey_Delete)){}
        else if(CTRL(ImGuiKey_C)){}
        else if(CTRL(ImGuiKey_V)){}
        else if(CTRL(ImGuiKey_Z))history::undo();
        else if(CTRL(ImGuiKey_Y))history::redo();
    }

    void treeViewProc(float &dt){
        menu_aware_height = viewport::wind.getSize().y-bar_height_button_size;

        if(newWindow(
            "Tree view", 
            {0,bar_height_button_size}, 
            {tree_view_width,menu_aware_height},
            {240,menu_aware_height},
            {FLT_MAX,menu_aware_height},
            ImGuiWindowFlags_NoMove 
            | ImGuiWindowFlags_NoCollapse 
            | ImGuiWindowFlags_NoScrollWithMouse
        )){
            tree_view_width = ImGui::GetWindowSize().x;
            // ImGui::Columns(2);
        
            // tree_root->drawCol1(active_selection, multiple_selection);
            // ImGui::NextColumn();
            
            // tree_root->drawCol2();

            ImGui::End();
        }


        
    } // void treeViewProc(float &dt)

    void editProc(float &dt){
        edit_pos_x = viewport::wind.getSize().x - edit_width;

        if(newWindow(
            "Edit",
            {edit_pos_x,bar_height_button_size},
            {edit_width,menu_aware_height},
            {240,menu_aware_height},
            {FLT_MAX,menu_aware_height},
            ImGuiWindowFlags_NoMove
            | ImGuiWindowFlags_NoCollapse 
            | ImGuiWindowFlags_NoScrollWithMouse
        )){
            if(selection)
            nodeEditMenu(selection.get());

            ImGui::End();
        }
    } // void editProc(float &dt)

    void toolsList(float &dt){

        if(ImGui::Begin("Tools")){
            for(auto &i : factory::creators)
            if(ImGui::Button(factory::names[(size_t)i.first].c_str())){
                history::act(new history::AddNode(i.second(), selection));
            }

            ImGui::End();
        }
    } // void makeNodePopUp(float &dt)

    void viewportProc(float &dt){
        any_editor_window_focused = ImGui::IsWindowFocused(
            ImGuiFocusedFlags_AnyWindow | 
            ImGuiFocusedFlags_ChildWindows
        ) || ImGui::IsAnyItemFocused();

        if(any_editor_window_focused)return;

        if(ImGui::IsMouseClicked(ImGuiMouseButton_Left)){
            selection = getHovered(node_root);
        }

        cam_pos.x -= wheel_delta_x * 128.f;
        cam_pos.y -= wheel_delta_y * 128.f;
        wheel_delta_x = wheel_delta_y = 0.f;

        if(selection){
            auto p = dynamic_cast<Spatial*>(selection.get());
            if(p){
                p->pos += 150.f * dt * v2f(getDirHeld());
                if(PRESSED(ImGuiKey_Q))p->angle += 50.f * dt;
                if(PRESSED(ImGuiKey_W))p->angle -= 50.f * dt;
            }
        }
    } // void viewportProc(float &dt)

    void process(float &dt){

        id_fix = 0;

        menuProc(dt);
        treeViewProc(dt);
        editProc(dt);
        toolsList(dt);

        viewportProc(dt);
    } 
};

void init(){
    assets::init();
    cout << "Assets init done.\n";
    viewport::init({960,720});
    cout << "Viewport init done.\n";
    debug::init();
    cout << "Debug init done.\n";
    factory::init();
    cout << "Node loader init done.\n";
    editor::init();

    srand(time(nullptr));

    debug::showAll();

    bool imgui_init_complete = ImGui::SFML::Init(viewport::wind);
    if(!imgui_init_complete){
        cout << "Note: couldn't init ImGUI in SFML window for some reason.\n";
    }
}

void exit(){
    viewport::exit();
    editor::exit();
}

int main(){
    init();

    sf::Clock dt_clock;
    sf::Time dt_time;
    float dt = 1.f/60.f;

    ImGuiIO& io = ImGui::GetIO();
    ImFont *def = io.Fonts->AddFontFromFileTTF("assets/main/fonts/courier.ttf", 20.f);

    bool result = ImGui::SFML::UpdateFontTexture();
    if(!result){
        cout << "Couldn't update font texture for some reason\n";
    }

    ImGuiStyle& style = ImGui::GetStyle();
    style.ChildRounding = 3.f;
    style.FrameRounding = 3.f;
    style.WindowRounding = 3.f;

    // cout << "loop begins...\n";

    //main loop
    while(viewport::wind.isOpen()){
        //update events
        while(const std::optional ev = viewport::wind.pollEvent()){
            ImGui::SFML::ProcessEvent(viewport::wind,*ev);
            if(ev->is<sf::Event::Closed>()){
                exit();
            }
            if(const auto* e = ev->getIf<sf::Event::MouseWheelScrolled>()){
                float d = e->delta;
                if(e->wheel == sf::Mouse::Wheel::Horizontal || SHIFT_AND(e->wheel == sf::Mouse::Wheel::Vertical))
                    editor::wheel_delta_x = d;
                if(e->wheel == sf::Mouse::Wheel::Vertical && !SHIFT_DOWN)
                    editor::wheel_delta_y = d;
            }
        }
        updateControls();

        //physics
        // viewport::bg_color = editor::any_editor_window_focused ? sf::Color(63,63,0) : sf::Color::Black;

        viewport::cam_pos = editor::cam_pos;
        //render

        // cout << "node processing...\n";
        editor::node_root->process();
        // cout << "node drawing...\n";
        editor::node_root->draw();
        // cout << "node debug drawing...\n";
        editor::node_root->drawDebug();
        // cout << "displaying...\n";
        // cout << viewport::draw_queue.size();
        viewport::display(dt);

        ImGui::SFML::Update(viewport::wind, dt_time);
        // cout << "tree viewer processing...\n";
        // cout << "pushing font\n";
        ImGui::PushFont(def);
        // cout << "pushed font\n";
        // cout << "trying processing editor...\n";
        editor::process(dt);
        ImGui::PopFont();
        ImGui::SFML::Render(viewport::wind);
        viewport::wind.display();

        dt_time = dt_clock.restart();
        dt = dt_time.asSeconds();
    }
    exit();
}