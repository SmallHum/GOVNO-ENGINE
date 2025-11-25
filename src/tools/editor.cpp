#define EDITOR
#define GVE_ENABLE_ASSERT

#include <core/history.h>
#include <imgui-SFML.h>

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

#define LMB ImGui::IsMouseClicked(ImGuiMouseButton_Left)

#define APPLY (PRESSED(ImGuiKey_Enter) || LMB)

using ImGui::MenuItem,
        ImGui::Separator;

// unsigned char map_208[256];
// unsigned char map_209[256];
// void init_map(){
//     map_208[129] = 
// }

string UTF8toCP1251(string s){
    string result;

    for(size_t i = 0; i < s.size(); i++){
        unsigned char c = (unsigned char)s[i];
        if(c == 208){
            i++;
            result += s[i] + 48;
        }
        else if(c == 209){
            i++;
            result += s[i] + 48;
        }
        else{
            result += s[i];
        }
    }

    return result;
}

namespace editor{

    // struct PopupWindow{
    //     bool is_open = 0;

    //     bool beginWin()

    //     void process(){
    //         if(!is_open)
    //         return;


    //     }

    //     void open(){

    //     }
    // }

    extern shared_ptr<Node> node_root;
    extern shared_ptr<Node> selection;
    size_t selection_depth = 0;

    int id_fix = 0;

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

    template<typename T>
    bool apply(
        const string &label, T &v, const T &buffer, 
        bool documentate = true
        // const function<void(T&, const K&)> applier = [](T &v, const K &buffer){
        //     v = buffer;
        // }
    ){
        if(!APPLY)
            return false;

        
        cout << "applying " << label << "...\n";
        // applier(v, buffer);
        if(documentate)
            history::act(new history::Apply<T>(v, buffer, v));
        else
            v = buffer;

        return true;
    }

    bool editFieldBool(string label, bool &v){
        return ImGui::Checkbox(label.c_str(), &v);
    }

    bool editFieldFloat(string label, float &v, bool doc = true){
        float buffer = v;

        if(!ImGui::InputFloat(label.c_str(), &buffer))
            return false;

        return apply(label, v, buffer, doc);
    }

    bool editFieldInt(string label, int &v, bool doc = true){
        int buffer = v;

        if(!ImGui::InputInt(label.c_str(), &buffer))
            return false;

        return apply(label, v, buffer, doc);
    }

    bool editFieldV2F(string label, v2f &v, bool doc = true){
        float buffer[2] = {v.x, v.y};

        if(!ImGui::InputFloat2(label.c_str(), buffer))
            return false;
        
        return apply(label, v, v2f(buffer[0], buffer[1]), doc);
    }

    bool editFieldString(string label, string &v, bool multiline = 0, bool doc = true){
        string buffer = v;

        // ImGui::PushID(id_fix++);
        if(multiline){
            if(!ImGui::InputTextMultiline(label.c_str(), &buffer))
                return false;
        }else if(!ImGui::InputText(label.c_str(), &buffer))
                return false;
        // ImGui::PopID();

        // if(cp1251)
        //     buffer = UTF8toCP1251(buffer);

        // return apply(label, v, buffer,
        //     {[](string &v, const string &buffer){
        //         v.resize(strlen(buffer.c_str()));
        //         v = buffer;
        //     }}
        // );

        return apply(label, v, buffer, doc);
    }

    bool editFieldSprite(sf::Sprite *&sprite){
        string buffer = assets::getSpriteName(*sprite);
        if(!editFieldString("Sprite", buffer, false, false))
            return 0;
        
        cout << "    applying sprite for real now" << '\n';
        try{
            sprite = &getSprite(buffer);
        }
        catch(...){}
        return true;
    }

    bool editFieldFont(GVEFont *&font){
        string buffer = assets::getFontName(*font);
        if(!editFieldString("Font", buffer, false, false))
            return 0;
        
        cout << "    applying font for real now" << '\n';
        try{
            font = &getFont(buffer);
        }
        catch(...){}
        return true;
    }

    bool sprite_edit_window_open = false;
    void SpriteEditMenu(Sprite *node){
        ImGui::SeparatorText("Sprite");

        // editFieldSprite(node->anim.sheet);
        editFieldInt("Z", node->z);
        if(ImGui::Button("Edit Animations")){
            sprite_edit_window_open = true;
        }
    }

    void LabelEditMenu(Label *node){
        ImGui::SeparatorText("Label");

        editFieldInt("Z", node->z);
        editFieldString("Text", node->text, true, true);
        editFieldFont(node->font);
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

        auto sprite = dynamic_cast<Sprite*>(node);
        if(sprite)SpriteEditMenu(sprite);
    }

    void nodeEditMenu(Node *node){
        ImGui::SeparatorText("Node");

        editFieldString("Name", node->name);
        editFieldBool("Visible", node->visible);

        auto s = dynamic_cast<Spatial*>(node);
        if(s)spatialEditMenu(s);
    }

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

    void feedEvent(const std::optional<sf::Event> &ev){
        if(const auto* e = ev->getIf<sf::Event::MouseWheelScrolled>()){
            float d = e->delta;
            if(CTRL_DOWN){
                v2f old_global_cursor_pos = viewport::getGlobalCursorPos();
                viewport::zoom *= (d == 1.f ? 1.1 : 0.9090);
                cout << d << '\n';
                if(viewport::zoom >= 0.1f && viewport::zoom <= 10.f)
                    cam_pos += old_global_cursor_pos - viewport::getGlobalCursorPos();
                cout << viewport::cam_pos << '\n';
                return;
            }
            if(e->wheel == sf::Mouse::Wheel::Horizontal || SHIFT_AND(e->wheel == sf::Mouse::Wheel::Vertical))
                editor::wheel_delta_x = d/viewport::zoom;
            if(e->wheel == sf::Mouse::Wheel::Vertical && !SHIFT_DOWN)
                editor::wheel_delta_y = d/viewport::zoom;
        }
    }

    bool isIntersecting(v2f a, v2f b, float margin = sel_margin / viewport::zoom){
        return (a-b).length() <= margin;
    }

    shared_ptr<Node> getHovered(shared_ptr<Node> root, size_t depth = 0){
        Spatial *sp = dynamic_cast<Spatial *>(root.get());

        if(sp && selection_depth < depth){
            v2f m = viewport::getGlobalCursorPos();
            // cout << m << '\n';
            if(isIntersecting(m, sp->getGlobalPos())){
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

    namespace grid{
        v2f size = {64.f, 64.f};
        v2f offset;

        void process(){
            for(float ys = viewport::cam_pos.y; ys < viewport::zoom*viewport::res.y + viewport::cam_pos.y; ys += size.y){
                v2f a = {0,ys - viewport::cam_pos.y},
                    b = {viewport::res.x,ys - viewport::cam_pos.y};
                ImGui::DrawLine(a, b, sf::Color::White);
            }
        }
    };

    // namespace axis_gizmo{
    //     Spatial *s;

    //     void process(){
    //         s = dynamic_cast<Spatial*>(selection.get());
    //     }

    //     void move(){
    //         if(!s)return;


    //     }
    // };

    void menuProc(float &dt){
        if(ImGui::BeginMainMenuBar()){
            if(ImGui::BeginMenu("File")){

                if(MenuItem("Open", "Ctrl+O"))open();
                if(MenuItem("Save", "Ctrl+S"))save();

                ImGui::EndMenu();
            }

            if(ImGui::BeginMenu("Edit")){
                if(MenuItem("Delete Node", "Del"))
                    history::act(new history::DeleteNode());

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
        else if(PRESSED(ImGuiKey_Delete))
            history::act(new history::DeleteNode());
        else if(CTRL(ImGuiKey_C)){}
        else if(CTRL(ImGuiKey_V)){}
        else if(CTRL(ImGuiKey_Z))history::undo();
        else if(CTRL(ImGuiKey_Y))history::redo();
    }

    namespace tree_view{
        struct node_info{
            bool collapsed;
        };

        unordered_map<Node *, node_info> data;

        void update(shared_ptr<Node> n = node_root){
            if(data.find(n.get()) == data.end())
                data[n.get()] = {1};
            
            for(auto &i : n->children)
                update(i);
        }

        void drawCol1(shared_ptr<Node> n = node_root, unsigned int depth = 1){
            ImGui::SetCursorPosX(depth * 32);

            ImGui::PushID(id_fix++);
            if(ImGui::Button(data[n.get()].collapsed ? ">" : "v", {24, 24}))
                data[n.get()].collapsed = !data[n.get()].collapsed;
            ImGui::SameLine();
            ImGui::Text(n->name.c_str());
            if(ImGui::IsItemClicked())
                selection = n;
            ImGui::PopID();

            if(!data[n.get()].collapsed)
                for(auto &i : n->children)
                    drawCol1(i, depth + 1);
        }
        void drawCol2(shared_ptr<Node> n = node_root){

            ImGui::PushID(id_fix++);
            if(ImGui::Checkbox("", &n->visible)){
                n->visible = !n->visible;
            }
            ImGui::PopID();

            if(!data[n.get()].collapsed)
                for(auto &i : n->children)
                    drawCol2(i);
        }
    };

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
            tree_view::update();

            tree_view_width = ImGui::GetWindowSize().x;
            ImGui::Columns(2);
        
            tree_view::drawCol1();
            ImGui::NextColumn();
            
            tree_view::drawCol2();

            ImGui::End();
        }


        
    } // void treeViewProc(float &dt)

    v2f box_size, origin;
    size_t curr_frame = 0;
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

        if(!(sprite_edit_window_open && 
            ImGui::Begin(
                "Edit Animations", 
                &sprite_edit_window_open, 
                ImGuiWindowFlags_HorizontalScrollbar
                ))
        )return;
        
        // cout << "trying to dynamic cast...\n";
        Sprite *s = dynamic_cast<Sprite*>(selection.get());
        // cout << "casted\n";

        if(!s){
            ImGui::End();
            return;
        }

        // cout << "is s\n";

        if(ImGui::Button("Add")){
            s->anim.push_back(Animation());
        }

        // cout << "added animation\n";

        if(s->anim.empty()){
            ImGui::End();
            return;
        }
        // cout << "isnt empty\n";

        string items; size_t n = 0;
        for(auto &i : s->anim)
            items += "Anim" + std::to_string(n++) + '\0';
        items += '\0';
        int curr_item = s->current_animation_index;
        Animation *curr_anim = &s->anim[curr_item];
        // cout << "items composed\n";

        ImGui::SameLine();
        if(ImGui::Button("Delete") && s->anim.size() > 1){
            s->anim.erase(s->anim.begin() + curr_item);
        }

        ImGui::SameLine();

        ImGui::SetNextItemWidth(128.f);
        ImGui::PushID(id_fix++);
        if(ImGui::Combo("", &curr_item, items.c_str())){
            s->current_animation_index = curr_item;
            curr_anim = &s->anim[curr_item];
        }
        ImGui::PopID();

        // ImGui::SameLine();
        
        ImGui::SetNextItemWidth(128.f);
        editFieldFloat("FPS", curr_anim->fps);

        ImGui::SameLine();

        ImGui::SetNextItemWidth(128.f);
        editFieldSprite(curr_anim->sheet);

        if(ImGui::Button("Play"))
            curr_anim->play();
        ImGui::SameLine();
        if(ImGui::Button("Pause"))
            curr_anim->pause();
        ImGui::SameLine();
        if(ImGui::Button("Stop"))
            curr_anim->stop();
        
        ImGui::SameLine();
        ImGui::SetNextItemWidth(128.f);
        editFieldFloat("Frame", curr_anim->time);
        
        
        ImVec2 image_cursor = ImGui::GetCursorPos();
        if(curr_anim->sheet){
            // if(ImGui::BeginChild(12312456)){
                ImGui::Image(*curr_anim->sheet);
                // ImGui::EndChild();
            // }
        }

        size_t frame_n = curr_anim->frame_origins.size();

        if(curr_frame >= frame_n)
            curr_frame = 0;

        for(size_t i = 0; i < frame_n; i++){
            ImGui::PushID(id_fix++);
            if(ImGui::Button("", {24,24}))
                // cout << frame_n << '\n';
                curr_frame = i;
            ImGui::PopID();

            
            if(i != frame_n - 1)
                ImGui::SameLine();

            auto curr_origin = curr_anim->frame_origins[i];
            auto curr_rect = curr_anim->frame_rects[i];

            sf::FloatRect f = {
                v2f(curr_rect.position), 
                v2f(curr_rect.size)
            };
            sf::FloatRect p = {
                curr_origin - v2f(2.f, 2.f) + v2f(curr_rect.position), 
                {4.f, 4.f}
            };
            sf::Color c = curr_frame == i ? sf::Color::Yellow : sf::Color::Red;

            ImVec2 temp = ImGui::GetCursorPos();
            ImGui::SetCursorPos(image_cursor);
            ImGui::DrawRect(f, c, 0.f, 0.f);
            ImGui::SetCursorPos(image_cursor);
            ImGui::DrawRect(p, c, 0.f, 0.f);
            ImGui::SetCursorPos(temp);

            auto mousepos = v2f(sf::Mouse::getPosition(viewport::wind));
            if(
                ImGui::IsMouseClicked(ImGuiMouseButton_Left)
                && 
                (
                    isIntersecting(mousepos, p.position + v2f(2.f,2.f))
                    || isIntersecting(mousepos, v2f(curr_rect.position) + v2f(image_cursor.x, image_cursor.y))
                    || isIntersecting(mousepos, v2f(curr_rect.position) + v2f(0.f,curr_rect.size.y) + v2f(image_cursor.x, image_cursor.y))
                    || isIntersecting(mousepos, v2f(curr_rect.position) + v2f(curr_rect.size.x, 0.f) + v2f(image_cursor.x, image_cursor.y))
                    || isIntersecting(mousepos, v2f(curr_rect.position + curr_rect.size) + v2f(image_cursor.x, image_cursor.y))
                )
            )
                curr_frame = i;
            
            
        }

        ImGui::SetNextItemWidth(256.f);
        editFieldV2F("Box size", box_size);
        ImGui::SetNextItemWidth(256.f);
        editFieldV2F("Origin", origin);

        if(ImGui::Button("Generate Frames") && box_size != v2f())
            curr_anim->generate(v2i(box_size), origin);

        ImGui::End();
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
    controls::init();
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

static void exit(){
    viewport::exit();
    editor::exit();
    assets::exit();

    std::exit(0);
}

int main(){
    init();

    sf::Clock dt_clock;
    sf::Time dt_time;
    float dt = 1.f/60.f;

    ImGuiIO& io = ImGui::GetIO();
    ImFont *def = io.Fonts->AddFontFromFileTTF("assets/main/fonts/courier.ttf", 20.f, NULL, io.Fonts->GetGlyphRangesCyrillic());

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
            editor::feedEvent(ev);
            controls::feedEvent(ev);
        }

        //physics
        // viewport::bg_color = editor::any_editor_window_focused ? sf::Color(63,63,0) : sf::Color::Black;

        viewport::cam_pos = editor::cam_pos;
        //render

        // cout << "node processing...\n";
        editor::node_root->process(dt);
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