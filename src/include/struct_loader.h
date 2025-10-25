#pragma once

#include <structs/spatial.h>

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
        "Spatial"
    };
};

template <typename T>
static T fstreamRead(fstream& f){
    T result;
    f.read(reinterpret_cast<char *>(&result), sizeof(result));

    return result;
}

template <typename T>
static void fstreamWrite(fstream &f, T &s){
    f.write(reinterpret_cast<const char *>(&s), sizeof(s));
}

template <typename T>
static sf::Vector2<T> fstreamReadV2(fstream& f){
    sf::Vector2<T> result;

    f.read(reinterpret_cast<char *>(&result.x), sizeof(result.x));
    f.read(reinterpret_cast<char *>(&result.y), sizeof(result.y));

    return result;
}

template <typename T>
static void fstreamWriteV2(fstream& f, sf::Vector2<T> &s){
    f.write(reinterpret_cast<char *>(&s.x), sizeof(s.x));
    f.write(reinterpret_cast<char *>(&s.y), sizeof(s.y));
}

string fstreamRead(fstream& f);

void fstreamWrite(fstream &f, string &s);

shared_ptr<Node> constructFromFile(string file);

shared_ptr<Node> constructFromStream(fstream &stream);

#define fstreamReadV2f fstreamReadV2<float>
#define fstreamReadV2i fstreamReadV2<int>
#define fstreamReadV2u fstreamReadV2<unsigned int>

#define fstreamWriteV2f fstreamWriteV2<float>
#define fstreamWriteV2i fstreamWriteV2<int>
#define fstreamWriteV2u fstreamWriteV2<unsigned int>