#include <console.h>

namespace console{
    bool is_open = 0;
    bool is_destroying = 0;

    HWND window;

    shared_ptr<Node> root;

    map<string,function<void()>> commands;
    map<string,string> command_descriptions;

    thread parallel_input;

    void initCommands(){
        commands["help"] = help;
        commands["find-node"] = findNode;
        commands["make-node"] = makeNode;
        commands["delete-node"] = deleteNode;
        commands["info"] = info;

        command_descriptions["help"] = "\tprints this list.";
        command_descriptions["find-node"] = "[path] \tfinds a node from root.";
        command_descriptions["make-node"] = "[class] [parent path] [arg 1] [arg 2] ... \tmakes a node.";
        command_descriptions["delete-node"] = "[path] \tdeletes a node.";
        command_descriptions["info"] = "[path] \tShows node's variables.";
    }
    
    void init(shared_ptr<Node> root_node){
        root.reset(root_node.get());

        initCommands();

        AllocConsole();

        window = FindWindowA("ConsoleWindowClass", NULL);

        freopen("CONOUT$", "w", stdout);
        freopen("CONIN$", "r", stdin);
        freopen("CONOUT$", "w", stderr);

        ShowWindow(window,0);

        parallel_input = thread(inputHandle);

        cout << "GOVNO-ENGINE debug console.\n\n";
    }
    void inputHandle(){
        string command;
        while(!is_destroying){
            root->printTree();
            cout << "> ";
            cin >> command;
            if(commands.find(command) == commands.end())
                cout << "ERROR: unknown command. Type 'help' to list existing commands.\n\n";
            else
                commands[command]();
        }
    }
    function<void()> help = [](){
        cout << "COMMANDS LIST:\n";
        for(auto &i : command_descriptions){
            cout << i.first << ' ' << i.second << '\n';
        }
        cout << '\n';
    };
    function<void()> findNode = [](){
        string path;
        cin >> path;
        shared_ptr<Node> result = root->find(path);
        if(result)
            cout << "Node found successfully! (" << result->name << ")\n\n";
        else cout << "ERROR: invalid path.\n\n";
    };
    function<void()> makeNode = [](){
        string _class, parent_path, name;

        cin >> _class >> parent_path >> name;

        shared_ptr<Node> node, parent = root->find(parent_path);

        if(!parent) {
            cout << "ERROR: invalid parent path.\n\n"; return;
        }

        if(_class == "Node"){
            node.reset(new Node(name));
            parent->addChild(node);
        }
        else {
            cout << "ERROR: invalid class.\n\n"; return;
        }

        cout << "Node " << name << " created successfully!\n\n";
    };
    function<void()> deleteNode = [](){
        string path;

        cin >> path;

        shared_ptr<Node> node = root->find(path);

        if(!node){
            cout << "ERROR: invalid path.\n\n"; return;
        }

        node->parent.lock()->removeChild(node);

        cout << "Node " << node->name << " removed successfully!\n\n";
    };
    function<void()> info = [](){
        string path;

        cin >> path;

        shared_ptr<Node> node = root->find(path);

        if(!node){
            cout << "ERROR: invalid path.\n\n"; return;
        }

        node->printInfo();
        cout << '\n';
    };
    void destroy(){
        is_destroying = 1;
        parallel_input.join();
        FreeConsole();
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