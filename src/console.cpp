#include <console.h>

namespace console{
    bool is_open = 0;
    bool is_destroying = 0;

    HWND window;

    weak_ptr<Node> root;

    map<string,function<void()>> commands;
    map<string,string> command_descriptions;

    thread parallel_input;

    void initCommands(){
        commands["help"] = help;
        commands["find-node"] = findNode;
        commands["make-node"] = makeNode;
        commands["delete-node"] = deleteNode;
        commands["info"] = info;
        commands["edit"] = edit;
        commands["print-tree"] = printTree;
        commands["debug"] = debug;
        commands["cd"] = cd;

        command_descriptions["help"] = "\tPrints this list.";
        command_descriptions["find-node"] = "[path] \tFinds a node from root.";
        command_descriptions["make-node"] = "[path] [class] [arg 1] [arg 2] ... \tMakes a node.";
        command_descriptions["delete-node"] = "[path] \tDeletes a node.";
        command_descriptions["info"] = "[path] \tShows node's variables.";
        command_descriptions["edit"] = "[path] [var name] [new value] \tEdit node's variables.";
        command_descriptions["print-tree"] = "\tPrints tree from root.";
        command_descriptions["debug"] = "[option] \tFlips the debug info display flags.";
        command_descriptions["cd"] = "[path] \tChanges directory.";
    }
    
    void init(shared_ptr<Node> root_node){
        root = root_node;

        initCommands();

        AllocConsole();

        window = FindWindowA("ConsoleWindowClass", NULL);

        freopen("CONOUT$", "w", stdout);
        freopen("CONIN$", "r", stdin);
        freopen("CONOUT$", "w", stderr);

        ShowWindow(window,0);

        parallel_input = thread(inputHandle);

        // for(unsigned char i = 0; i < 255; i++)
        //     cout << (int)i << ' ' << i << '\n';
        // cout << '\n';

        cout << "GOVNO-ENGINE debug console.\n\n";
    }
    shared_ptr<Node> fromPath(){
        string path;
        cin >> path;
        if((path.front() == '"' && path.back() == '"') ||
        (path.front() == '\'' && path.back() == '\'')){
            path.pop_back();
            path.erase(path.begin());
        }
        return root.lock()->find(path);
    }
    void inputHandle(){
        string command;
        while(!is_destroying){
            cout << root.lock()->name << " > ";
            cin >> command;
            if(commands.find(command) == commands.end())
                cout << "ERROR: unknown command. Type 'help' to list existing commands.\n\n";
            else
                commands[command]();
        }
        std::terminate();
    }
    function<void()> help = [](){
        cout << "COMMANDS LIST:\n";
        for(auto &i : command_descriptions){
            cout << i.first << ' ' << i.second << '\n';
        }
        cout << '\n';
    };
    function<void()> findNode = [](){
        shared_ptr<Node> result = fromPath();
        if(!result){
            cout << "ERROR: invalid path.\n\n"; return;
        }
        cout << "Node found successfully! (" << result->name << ")\n\n";
    };
    function<void()> makeNode = [](){
        string _class, name;

        shared_ptr<Node> node, parent = fromPath();

        if(!parent) {
            cout << "ERROR: invalid parent path.\n\n"; return;
        }

        cin >> _class >> name;

        if(_class == "Node"){
            node.reset(new Node(name));
            parent->addChild(node);
        }
        if(_class == "Spatial"){
            float pos_x, pos_y, angle, scale_x, scale_y;
            int r, g, b;
            cin >> pos_x >> pos_y >> angle >> scale_x >> scale_y >> r >> g >> b;

            node.reset(new Spatial(
                name, 
                v2f(pos_x,pos_y), 
                angle, 
                v2f(scale_x, scale_y), 
                sf::Color(r,g,b)
            ));
            parent->addChild(node);
        }
        else {
            cout << "ERROR: invalid class.\n\n"; return;
        }

        cout << "Node " << name << " created successfully!\n\n";
    };
    function<void()> deleteNode = [](){
        shared_ptr<Node> node = fromPath();
        if(!node){
            cout << "ERROR: invalid path.\n\n"; return;
        }

        node->parent.lock()->removeChild(node);

        cout << "Node " << node->name << " removed successfully!\n\n";
    };
    function<void()> info = [](){
        shared_ptr<Node> node = fromPath();
        if(!node){
            cout << "ERROR: invalid path.\n\n"; return;
        }

        node->printInfo(cout);
        cout << '\n';
    };
    function<void()> edit = [](){
        shared_ptr<Node> node = fromPath();
        if(!node){
            cout << "ERROR: invalid path.\n\n"; return;
        }

        Spatial* spatial = dynamic_cast<Spatial*>(node.get());

        string var_name;
        cin >> var_name;

        //I couldn't think of other solution...

        //Node parameters
        if(var_name == "name"){
            string new_value;
            cin >> new_value;
            node->name = new_value;
        }
        else if(var_name == "active"){
            bool new_value;
            cin >> new_value;
            node->active = new_value;
        }
        else if(var_name == "visible"){
            bool new_value;
            cin >> new_value;
            node->visible = new_value;
        }
        else if(spatial){
            if(var_name == "pos"){
                float x,y;
                cin >> x >> y;
                spatial->pos = v2f(x,y);
            }
            else if(var_name == "angle"){
                float new_value;
                cin >> new_value;
                spatial->angle = new_value;
            }
            else if(var_name == "scale"){
                float x,y;
                cin >> x >> y;
                spatial->scale = v2f(x,y);
            }
            else if(var_name == "debug-color"){
                int r, g, b;
                cin >> r >> g >> b;
                spatial->debug_color = sf::Color(r,g,b);
            }
        }
        else{
            cout << "ERROR: invalid node variable.\n\n"; return;
        }
        

        cout << "Variable " << var_name << " was changed successfully!\n\n";
        
    };
    function<void()> printTree = [](){
        root.lock()->printTree();
    };
    function<void()> debug = [](){
        string option;
        cin >> option;
        if(option == "-")
            viewport::showAll();
        else if(option == "show-fps")
            viewport::showFps();
        else if(option == "show-spatial-name")
            viewport::showSpatialName();
        else if(option == "show-spatial-origin")
            viewport::showSpatialOrigin();
        else if(option == "show-picked-node-info")
            viewport::showPickedNodeInfo();
        else if(option == "show-sprite-bounds")
            viewport::showSpriteBounds();
        else if(option == "show-aabb-bounds")
            viewport::showAABBBounds();
        else{
            cout << "ERROR: invalid option.\n\n"; return;
        }
        cout << "Debug flag switched successfuly!\n\n";
    };
    function<void()> cd = [](){
        shared_ptr<Node> node = fromPath();
        if(!node){
            cout << "ERROR: invalid path.\n\n"; return;
        }

        root = node;
    };
    void destroy(){
        // PostMessage(window, WM_CLOSE, 0, 0);
        open();
        is_destroying = 1;
        parallel_input.detach();
        CloseWindow(window);

        // GetProcAddress
        DWORD pid = 0;
        GetWindowThreadProcessId(window, &pid);

        FreeConsole();

        HANDLE proc = OpenProcess(PROCESS_TERMINATE, FALSE, pid);
        if (proc) {
            TerminateProcess(proc, 0);
            CloseHandle(proc);
        }
    }

    void open(){
        is_open = 1;
        ShowWindow(window,1);
    }

    void close(){
        is_open = 0;
        ShowWindow(window,0);
    }

    void flip(){
        if(is_open)
            close();
        else open();
    }
}

/*

взяли 2 игральные кости и подкинули 20 раз
пусть событие А - выпадение чётной суммы очков
произошло 8 раз

А - 8/20

событие В - выпадение суммы очков кратное трём
произошло 6 раз

В = 6/20

событие С - выпадение суммы очков одновременно кратное и двум и трём
произошло 4 раза

С = 4/20

определить безусловные и условные частоты рассматриваемых событий А и Б и частоту их произведения


*/