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

void Node::printTree(int spaces){
    for(int i = 0; i < spaces; i++) std::cout << "  ";
    std::cout << name << '\n';

    for(auto& i : children) i->printTree(spaces+1);
}

Node::~Node(){
    std::cout << "Node " << name << " destructed." << '\n';
}