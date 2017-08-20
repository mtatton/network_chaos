#pragma once
#include <SDL2/SDL.h>
#include "rgb.h"
#include "coords.h"
#include <string>

enum class CursorOption {
    none, info, select, ok, cancel, move
};

namespace event {
    extern const Uint8 *keys;

    bool loop();
    int get_alphabet();
    bool get_yes_or_no();
    bool get_yes_or_no(const Coords& xy, const RGB& fg, const RGB& bg = black);
    void wait_for_key(const int& frame_delay = 0);
    std::string text_input_name(const RGB& fg, const Coords& xy);
    std::string text_input_server(const RGB& fg, const Coords& xy);
    int number_input(const int& min, const int& max, const Coords& xy, const RGB& fg, const RGB& bg = black);
    CursorOption cursor_movement(Coords& xy);
    int menu_option();
    void delay(const int& frames = 1);
}
