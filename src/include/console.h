#pragma once

#include <config.h>

namespace console{
    extern bool is_open;

    extern HWND window;

    void init();
    void open();
    void close();
    void flip();
    void destroy();
};