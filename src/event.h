#pragma once
#include "sdl.h"
#include "rgb.h"
#include <string>

namespace event {
    extern const Uint8 *keys;

    bool loop();
    int get_alphabet();
    bool get_yes_or_no(const RGB& fg, const RGB& bg, const int& x, const int& y);
    void wait_for_keydown(const int& frame_delay = 0);
    std::string text_input_name(const RGB& fg, const int& x, const int& y);
    std::string text_input_server(const RGB& fg, const int& x, const int& y);
    int number_input(const int& min, const int& max, const RGB& fg, const RGB& bg, const int& x, const int& y);
    bool cursor_movement(int& x, int& y);
    int menu_option();
}
