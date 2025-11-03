// #pragma once

#ifndef GVE_LABEL_H
#define GVE_LABEL_H

#include <structs/font.h>
#include <structs/spatial.h>
#include <core/assets.h>

struct Label: Spatial{
    shared_ptr<GVEFont> font;
    string text;

    vector<sf::Sprite *> letters_to_draw;

    Label();

    Label(string name,
        v2f pos, float angle = 0.f, v2f scale = {1.f, 1.f},
        shared_ptr<GVEFont> font = make_shared<GVEFont>(getTexture("main:default_font")),
        string text = "Text here.",
        sf::Color debug_color = sf::Color::White);

    void setText(string text);

    virtual StructId getStructId() override{
        return StructId::Label;
    }

    virtual void draw() override{
        for(auto &i : text){
            letters_to_draw.push_back(font->getGlyph(i));
        }
        Spatial::draw();
    }

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