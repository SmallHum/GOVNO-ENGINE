#pragma once

#include <config.h>
#include <core/enums.h>

struct Node : std::enable_shared_from_this<Node>{
    vector<shared_ptr<Node>> children;
    weak_ptr<Node> parent;

    string name;

    // Determines if this tree needs to be processed.
    // ```process``` and ```physProcess``` functions, to be exact.
    bool active = 1;

    // Determines if this tree needs to be drawn.
    bool visible = 1;

    // Index in parent's children array.
    // Set to ```-1``` by default, or if it has no parent.
    size_t parent_index = -1;

    // Empty constructor.
    Node();

    Node(string name);

    void addChild(shared_ptr<Node> node);

    void removeChild(shared_ptr<Node> node);
    void removeChild(size_t index);

    // Struct ID for saving and loading node tree
    virtual StructId getStructId();

    // Inherit functions

    virtual void onCreation();

    virtual void process();

    virtual void physProcess(float delta);

    virtual void onDeletion();

    virtual void draw();
    virtual void drawDebug();

    // Finds a node in child tree via ```"path/to/the/node"```.
    // Returns ```nullptr``` if couldn't find the node.
    shared_ptr<Node> find(string path);

    void printTree(int spaces = 0);
    // virtual void printInfo(std::ostream& s);

    // Save node tree to file. Usually that's what you'd want to use.
    // ```writeToStream``` is used for recursive calls.
    void writeToFile(string file);

    // Inherited function to read everything that isn't ```struct id``` or ```children```;
    virtual void reader(fstream &stream);

    // Inherited function to write everything that isn't ```struct id``` or ```children```;
    virtual void writer(fstream &stream);
    
    void writeToStream(fstream &stream);

    // Copy tree
    virtual void copy(weak_ptr<Node> node);

    ~Node();
};