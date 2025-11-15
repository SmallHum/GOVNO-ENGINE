// #pragma once

#ifndef GVE_SPATIAL_H
#define GVE_SPATIAL_H

#include <structs/node.h>

struct Spatial : Node{
    mat3 local_transform;
    mat3 global_transform;

    sf::Color debug_color;

    v2f pos;
    float angle;
    v2f scale;

    // Empty constructor.
    Spatial();

    Spatial(string name,
        v2f pos, float angle = 0.f, v2f scale = {1.f,1.f},
        sf::Color debug_color = sf::Color::White
    );

    void updateLocalTransform();

    void updateTransform();

    mat3 getGlobalTransform();

    v2f getGlobalPos();

    virtual StructId getStructId() override;

    virtual void drawDebug() override;

    virtual void process() override;

    virtual void reader(fstream &stream) override;
    virtual void writer(fstream &stream) override;
    
    // virtual void printInfo(std::ostream& s) override;
    
    // Copy tree.
    // Make sure the node in pointer is a spatial.
    virtual void copy(weak_ptr<Node> node) override;
};

#endif