#pragma once
#include <SDL2/SDL.h>

namespace event {
    extern const Uint8 *keys;

    bool loop();
    int get_alphabet();
}
