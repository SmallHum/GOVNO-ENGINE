#pragma once

#include <structs/spatial.h>

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

string fstreamRead(fstream& f);

void fstreamWrite(fstream &f, string &s);

shared_ptr<Node> constructFromFile(string file);

shared_ptr<Node> constructFromStream(fstream &stream);