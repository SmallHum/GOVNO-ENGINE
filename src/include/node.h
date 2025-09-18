#pragma once

#include <config.h>

struct Node : std::enable_shared_from_this<Node>{
    vector<shared_ptr<Node>> children;
    weak_ptr<Node> parent;

    //Index in parent's children array.
    //Set to -1 by default, or if it has no parent.
    size_t parent_index = -1;

    string name;

    Node(string name = "Node");

    void addChild(shared_ptr<Node> node);

    void removeChild(shared_ptr<Node> node);
    void removeChild(size_t index);

    //Finds a node in child tree via "path/to/the/node"
    //Returns nullptr if couldn't find the node
    shared_ptr<Node> find(string path);

    void printTree(int spaces = 0);

    ~Node();
};