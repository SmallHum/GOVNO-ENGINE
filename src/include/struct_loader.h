// #pragma once

#ifndef GVE_STRUCT_LOADER_H
#define GVE_STRUCT_LOADER_H

#include <structs/aabb.h>
#include <structs/label.h>

namespace factory{
    extern unordered_map<StructId, function<shared_ptr<Node>()>> creators;

    void init();

    template<typename T>
    static void reg(const StructId& id){
        creators[id] = []{ return make_shared<T>(); };
    }

    shared_ptr<Node> create(const StructId& id);

    shared_ptr<Node> copy(weak_ptr<Node> target);

    const string names[] = {
        "Node",
        "Spatial",
        "AABB",
        "Label",
        "Sprite"
    };
};

shared_ptr<Node> constructFromFile(string file);

shared_ptr<Node> constructFromStream(fstream &stream);

#endif