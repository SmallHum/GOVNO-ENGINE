#include <node.h>

Node::Node(string name): name(name){
    std::cout << "Node " << name << " constructed." << '\n';
}

void Node::addChild(shared_ptr<Node> node){
    if(!node)return;
    if(auto p = node->parent.lock()){
        p->removeChild(node->parent_index);
    }

    node->parent = shared_from_this();
    node->parent_index = children.size();
    children.push_back(node);
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
    size_t index = path.find_first_of('/');

    string searched_name = path.substr(0,index),
    next_path = path.substr(index + 1, path.size() - index);

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
    for(int i = 0; i < spaces; i++) std::cout << "  ";
    std::cout << icon << ' ' << name << '\n';

    for(auto& i : children) i->printTree(spaces+1);
}
void Node::printInfo(){
    cout << "Node:\n" <<
    " name: " << name << '\n' <<
    " active: " << (active ? "True" : "False") << '\n' <<
    " visible: " << (visible ? "True" : "False") << '\n';
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

Node::~Node(){
    std::cout << "Node " << name << " destructed." << '\n';
}