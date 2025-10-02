#pragma once

#include <config.h>

struct Node : std::enable_shared_from_this<Node>{
    vector<shared_ptr<Node>> children;
    weak_ptr<Node> parent;

    string name;

    //Icon to display in print node tree.
    char icon = 'Y';

    //Determines if this tree needs to be processed.
    //```process``` and ```physProcess``` functions, to be exact.
    bool active = 0;

    //Determines if this tree needs to be drawn.
    bool visible = 1;

    //Index in parent's children array.
    //Set to ```-1``` by default, or if it has no parent.
    size_t parent_index = -1;

    Node(string name = "Node");

    void addChild(shared_ptr<Node> node);

    void removeChild(shared_ptr<Node> node);
    void removeChild(size_t index);

    //Inherit functions

    virtual void onCreation();

    virtual void process();

    virtual void physProcess(float delta);

    virtual void onDeletion();

    virtual void draw();
    virtual void drawDebug();

    //Finds a node in child tree via ```"path/to/the/node"```.
    //Returns ```nullptr``` if couldn't find the node.
    shared_ptr<Node> find(string path);

    void printTree(int spaces = 0);
    virtual void printInfo();

    ~Node();
};