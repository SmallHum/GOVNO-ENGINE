#pragma once

#include <structs/font.h>
#include <structs/spatial.h>
#include <core/assets.h>

struct Label: Spatial{
    Font *font;
    string text;

    Label();

    Label(string name,
        v2f pos, float angle = 0.f, v2f scale = {1.f, 1.f},
        Font *font = new Font(getTexture("main:default_font")),
        string text = "Text here.",
        sf::Color debug_color = sf::Color::White);
};

// ☺☻♥♦♣♠•◘○