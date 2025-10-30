#pragma once

#include <structs/spatial.h>

struct AABB: Spatial{
    // Sides of a rectangle.
    // ```top``` and ```bottom``` correspond to y axis bounds
    //```left``` and ```right``` to x axis bounds
    // Since i suck at coding, it's all axis aligned
    // and starts from object's global position
    float top, bottom, left, right;

    // Empty constructor.
    AABB();

    AABB(string name,
        v2f pos, float angle, v2f scale,
        float top, float bottom, float left, float right,
        sf::Color debug_color = sf::Color::White
    );

    bool intersectPoint(shared_ptr<Spatial> spatial);
    bool intersectPoint(v2f pos);

    bool intersectAABB(shared_ptr<AABB> other);

    virtual StructId getStructId() override;

    virtual void drawDebug() override;

    virtual void process() override;

    virtual void reader(fstream &stream) override;
    virtual void writer(fstream &stream) override;
    
    // virtual void printInfo(std::ostream& s) override;
    
    // Copy tree.
    // Make sure the node in pointer is an AABB.
    virtual void copy(weak_ptr<Node> node) override;


};