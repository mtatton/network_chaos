#pragma once
#include "gfx.h"
#include "event.h"
#include <string>

struct Server {
    bool host = false;
    std::string address;
};

struct Player {
    std::string name;
    int sprite_index = 0;
    RGB rgb;
};

struct ChosenSpell {
    int spell_index = -1;
    bool illusion = false;
    operator bool() const;
};

namespace ui {
    extern Coords cursor;

    void set_cursor_to_wizard(const std::shared_ptr<Wizard>& wizard);
    void loading_screen();
    Server title_screen_network();
    int title_screen();
    Player player_entry(const int& player_number = 0);
    void set_cursor_to_wizard(const std::shared_ptr<Wizard>& wizard);
    ChosenSpell menu(const std::shared_ptr<Wizard>& wizard);
    CursorOption select_tile_for_spell();
    CursorOption select_tile_for_movement();
    CursorOption select_tile_for_flying(const int& movement);
    CursorOption select_tile_for_ranged_combat();
    CursorOption walking(Coords& xy, const int& movement, const bool& wizard_moving = false);
    CursorOption attack(Coords& xy);
}
