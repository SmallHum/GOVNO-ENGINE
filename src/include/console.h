#pragma once

#include <config.h>

namespace console{
    extern bool is_open, is_destroying;

    extern HWND window;

    extern thread parallel_input;

    void init();
    void open();
    void close();
    void flip();
    void destroy();
};