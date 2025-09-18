#pragma once

#include <config.h>

namespace console{
    extern bool is_open, is_destroying;

    extern HWND window;

    extern thread parallel_input;

    //Command functions

    //Prints out the help menu
    void help();

    //Finds a node from root
    void findNode();

    //Makes a node and adds it to a node
    void makeNode();

    void inputHandle();

    void init();

    void open();
    void close();
    void flip();
    void destroy();
};