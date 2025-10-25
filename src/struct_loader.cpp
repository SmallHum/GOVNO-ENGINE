#include <struct_loader.h>

namespace factory{
    unordered_map<StructId, function<shared_ptr<Node>()>> creators;

    void init(){
        factory::reg<Node>(StructId::Node);
        factory::reg<Spatial>(StructId::Spatial);
    }

    shared_ptr<Node> create(const StructId& id){
        auto it = creators.find(id);
        return it != creators.end() ? it->second() : nullptr;
    }

    shared_ptr<Node> copy(weak_ptr<Node> target){
        shared_ptr<Node> result = factory::create(target.lock()->getStructId());
        result->copy(target.lock());
        return result;
    }
};

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

shared_ptr<Node> constructFromFile(string file){
    fstream stream(file, std::ios::in | std::ios::binary);

    if(!stream.is_open()){
        cout << "ERROR: couldn't read file " << file << "\n";
        return make_shared<Node>(Node());
    }
    shared_ptr<Node> result = constructFromStream(stream);

    stream.close();

    return result;
}

shared_ptr<Node> constructFromStream(fstream &stream){

    StructId id = fstreamRead<StructId>(stream);
    shared_ptr<Node> result = factory::create(id);

    result->reader(stream);

    size_t children_count = fstreamRead<size_t>(stream);

    for(int i = 0; i < children_count; i++){
        result->addChild(constructFromStream(stream));
    }

    return result;
}