#pragma once
#include "gfx.h"
#include <string>

struct Server {
    bool host = false;
    std::string address;
};

struct Player {
    std::string name;
    int sprite_index = 0;
    int colour_index = 0;
};

namespace ui {
    void loading_screen();
    Server title_screen();
    Player player_entry();
    void menu(Wizard& wizard);
}
