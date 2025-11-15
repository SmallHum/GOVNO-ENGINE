#include <struct_loader.h>

namespace factory{
    unordered_map<StructId, function<shared_ptr<Node>()>> creators;

    void init(){
        factory::reg<Node>(StructId::Node);
        factory::reg<Spatial>(StructId::Spatial);
        factory::reg<AABB>(StructId::AABB);
        factory::reg<Label>(StructId::Label);
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

void Node::copy(weak_ptr<Node> node){
    if(auto n = node.lock()){
        name = n->name;
        active = n->active;
        visible = n->visible;
        for(auto &i : n->children){
            shared_ptr<Node> result = factory::create(i->getStructId());
            result->copy(i);
            addChild(result);
        }
    }
    else{
        cout << "Node NOT constructed due to invalid pointer.\n";
    }
}