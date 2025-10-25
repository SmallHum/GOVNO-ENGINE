#include <structs/node.h>
#include <struct_loader.h>

Node::Node(){
    name = "Node";
}

Node::Node(string name): name(name){
    std::cout << "Node " << name << " constructed." << '\n';
}

StructId Node::getStructId(){
    return StructId::Node;
}

void Node::addChild(shared_ptr<Node> node){
    if(!node)return;
    if(auto p = node->parent.lock()){
        p->removeChild(node->parent_index);
    }

    node->parent = shared_from_this();
    node->parent_index = children.size();
    children.push_back(node);

    Spatial *spat_node = dynamic_cast<Spatial*>(node.get());
    if(spat_node)
        spat_node->updateTransform();
}

void Node::removeChild(shared_ptr<Node> node){
    size_t n = children.size();
    for(int i = 0; i < n; i++)
        if(node == children[i])
            removeChild(i);
}

void Node::removeChild(size_t index){
    if(index >= children.size())return;
    children[index]->parent.reset();
    children.erase(children.begin() + index);
}

shared_ptr<Node> Node::find(string path){
    if(path.empty())
        return shared_from_this();

    size_t index = path.find_first_of('/');
    string searched_name = path.substr(0,index),

    next_path = path.substr(index + 1, path.size() - index);

    if(searched_name == ".."){
        return parent.lock()->find(next_path);
    }

    for(auto &i : children){
        if(i->name == searched_name){
            if(index == -1)
                return i;
            return i->find(next_path);
        }
    }

    return shared_ptr<Node>();
}

void Node::printTree(int spaces){
    for(int i = 0; i < spaces; i++) cout << "  ";
    cout << name << '\n';
    for(auto& i : children) i->printTree(spaces+1);
}
void Node::printInfo(std::ostream& s){
    s << (int)getStructId() << '\n';
    s << "Node:\n" <<
    " name: " << name << '\n' <<
    " active: " << prettyBool(active) << '\n' <<
    " visible: " << prettyBool(visible) << '\n';
}

void Node::onCreation(){}

void Node::process(){
    for(auto& i : children)
        if(i->active)
            i->process();
}

void Node::physProcess(float delta){
    for(auto& i : children)
        if(i->active)
            i->physProcess(delta);
}

void Node::onDeletion(){}

void Node::draw(){
    for(auto& i : children)
        if(i->visible)
            i->draw();
}

void Node::drawDebug(){
    for(auto& i : children)
        i->drawDebug();
}

void Node::writeToFile(string file){
    fstream stream(file, std::ios::out | std::ios::binary);

    if(!stream.is_open()){
        cout << "ERROR: couldn't write to file " << file << "\n";
        return;
    }

    writeToStream(stream);

    stream.close();
}

void Node::writer(fstream &stream){
    fstreamWrite(stream,name);
    fstreamWrite<bool>(stream,active);
    fstreamWrite<bool>(stream,visible);
}

void Node::reader(fstream& stream){
    name = fstreamRead(stream);
    active = fstreamRead<bool>(stream);
    visible = fstreamRead<bool>(stream);
}

void Node::writeToStream(fstream &stream){

    unsigned int id = (unsigned int)getStructId();
    fstreamWrite<unsigned int>(stream, id);

    writer(stream);

    size_t size = children.size();
    fstreamWrite<size_t>(stream,size);

    for(auto &i : children)
        i->writeToStream(stream);
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

Node::~Node(){
    std::cout << "Node " << name << " destructed." << '\n';
}