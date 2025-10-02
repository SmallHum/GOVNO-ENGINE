#pragma once

#include <node.h>

struct Spatial : Node{
    char icon = '*';

    mat3 local_transform;
    mat3 global_transform;

    sf::Color debug_color;

    v2f pos;
    float angle;
    v2f scale;

    Spatial(string name, v2f pos, float angle = 0.f, v2f scale = {1.f,1.f}, sf::Color debug_color = sf::Color::White);

    void updateLocalTransform();

    void updateTransform();

    mat3 getGlobalTransform();

    v2f getGlobalPos();

    virtual void drawDebug() override;
};