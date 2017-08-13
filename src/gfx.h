#pragma once
#include "arena.h"
#include <vector>

namespace sdl {
    void refresh();
}

namespace gfx {
    void draw_text(std::string, const RGB &fg, const RGB &bg, const int &x, const int &y);

    void draw_menu_border();
    void draw_player_entry_border();
    void draw_spell_border();
    void draw_info_border();
    void draw_arena_border();

    void draw_spell_cursor(const RGB &fg, const int &x, const int &y);
    void draw_box_cursor(const RGB &fg, const int &x, const int &y);
    void draw_wings_cursor(const RGB &fg, const int &x, const int &y);
    void draw_ranged_cursor(const RGB &fg, const int &x, const int &y);

    void draw_creation(const Creation& creation, const int &x, const int &y, const int &sprite_index);
    void draw_creation_corpse(const Creation& creation, const int &x, const int &y);

    void draw_wizard(const int &sprite_index, const RGB &rgb, const int &x, const int &y);
    void draw_wizard_xy(const int &sprite_index, const RGB &rgb, const int &x, const int &y);

    void draw_wizard(const Wizard wizard, const int &x, const int &y, const int &frame_index);

    void draw_exploding_circle(const int &sprite_index, const int &x, const int &y);
    void draw_twirl(const int &sprite_index, const int &x, const int &y);
    void draw_explosion(const int &sprite_index, const int &x, const int &y);
    void draw_dragon_burn(const int &sprite_index, const int &x, const int &y);
    void draw_attack(const int &sprite_index, const int &x, const int &y);

    void draw_simple_beam(const RGB &rgb, const int &x, const int &y);
    void draw_spell_beam(const RGB &rgb, const int &x, const int &y);
    void draw_burn_beam(const RGB &rgb, const int &x, const int &y);

    void draw_loading_screen();

    void draw_stats(const Creation& creation);
    void draw_stats(const Wizard& wizard);
    void draw_spell(const Spell& spell);

    void draw_spellbook(const Wizard& wizard);

    void draw_title_screen();
    void draw_enter_server();
    void draw_main_menu(const std::string& player_name);
    void draw_player_entry_screen();
    void draw_computer_controlled_question();
    void draw_which_character_question();
    void draw_which_colour_question(const int& sprite_index);

    void draw_info_text(const Tile& tile);
}
