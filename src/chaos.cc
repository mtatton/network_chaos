#include "chaos.h"
#include <SDL2/SDL.h>
#include "gfx.h"
#include "event.h"

int main (int argc, char const *argv[]) {
    gfx::draw_info_border();
    gfx::draw_golden_dragon_corpse(1, 1);
    gfx::draw_wizard(2, bright_green, 0, 0);
    gfx::draw_magic_shield(0, bright_red, 2, 2);
    gfx::draw_twirl(4, 3, 3);
    gfx::draw_text("Hello world", bright_white, black, 12, 8);
    gfx::draw_burn_beam(bright_red, 10, 10);
    while(event::loop()) {
        sdl::refresh();
    }
    return 0;
}