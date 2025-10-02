#pragma once

#include <config.h>

struct Node;

namespace console{
    extern bool is_open, is_destroying;

    //Console window handle
    extern HWND window;

    extern weak_ptr<Node> root;

    extern map<string,function<void()>> commands;
    extern map<string,string> command_descriptions;

    extern thread parallel_input;

    //Command functions

    //Prints out the help menu
    extern function<void()> help;

    //Finds a node from root
    extern function<void()> findNode;

    //Makes a node and adds it to a parent from path/from/root//to/parent
    extern function<void()> makeNode;

    //Delets a node from path/to/node
    extern function<void()> deleteNode;

    //Shows info from node
    extern function<void()> info;

    //Edit node's data
    extern function<void()> edit;

    //Print root tree
    extern function<void()> printTree;

    //Switch debug flags
    extern function<void()> debug;

    //Change directory
    extern function<void()> cd;

    void inputHandle();

    void init(shared_ptr<Node> root_node);

    shared_ptr<Node> fromPath();

    void open();
    void close();
    void flip();
    void destroy();
};