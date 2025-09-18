#include <console.h>

namespace console{
    bool is_open = 0;
    bool is_destroying = 0;

    HWND window;

    thread parallel_input;

    void help(){
        cout << "COMMANDS LIST:\n" <<
        "   help - prints this menu\n" <<
        "   find-node [path] - finds a node from the written path.\n" <<
        "   make-node [class] [parent path] [arg 1] [arg 2] ... - creates a node\nof a specific class and adds it to an existing node.\n" <<
        '\n';
    }
    void init(){
        AllocConsole();

        window = FindWindowA("ConsoleWindowClass", NULL);

        freopen("CONOUT$", "w", stdout);
        freopen("CONIN$", "r", stdin);
        freopen("CONOUT$", "w", stderr);

        ShowWindow(window,0);

        parallel_input = thread(inputHandle);

        cout << "GOVNO-ENGINE debug console.\n\n";
    }
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