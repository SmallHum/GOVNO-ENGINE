#include <fstream_opers.h>

string fstreamRead(fstream& f){
    string result;
    size_t size;
    f.read(reinterpret_cast<char *>(&size), sizeof(size));

    char *buffer = new char[size + 1];
    f.read(buffer, size);

    buffer[size] = '\0';
    result = buffer;

    delete[] buffer;

    return result;
}

void fstreamWrite(fstream &f, string &s){
    size_t size = s.size();
    f.write(reinterpret_cast<const char *>(&size), sizeof(size));

    f.write(s.c_str(), size);
}