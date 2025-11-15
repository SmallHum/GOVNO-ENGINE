// #pragma once

#ifndef GVE_LABEL_H
#define GVE_LABEL_H

#include <structs/font.h>
#include <structs/spatial.h>

struct Label: Spatial{
    GVEFont *font = nullptr;
    string text;

    // Depth value used to specify the order of drawn sprites.
    int z;

    Label();

    Label(string name,
        v2f pos, float angle = 0.f, v2f scale = {1.f, 1.f},
        string text = "Text here.",
        int z = 0,
        GVEFont &font = getFont("main:font_default"),
        sf::Color debug_color = sf::Color::White);

    // void setText(string text);

    virtual StructId getStructId() override;

    virtual void draw() override;

    // virtual void drawDebug() override;

    // virtual void process() override;

    virtual void reader(fstream &stream) override;
    virtual void writer(fstream &stream) override;
    
    // Copy tree.
    // Make sure the node in pointer is a label.
    virtual void copy(weak_ptr<Node> node) override;
};

// ☺☻♥♦♣♠•◘○

#endif