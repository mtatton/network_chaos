#pragma once
#include "arena.h"
#include <vector>

namespace sdl {
    void refresh();
}

namespace gfx {
    void draw_text(std::string, const Coords& xy, const RGB &fg, const RGB &bg = black);
    void draw_illusion_question();
    void draw_spell_succeeds();
    void draw_spell_fails();
    void draw_out_of_range(const RGB &rgb);
    void no_line_of_sight();
    void draw_turn_text(const std::string& name);
    void draw_movement_range(const int& movement, const bool& flying = false);
    void draw_movement_points_left(const int& movement);
    void draw_ranged_combat(const int& range);
    void draw_engaged_to_enemy();

    void draw_arena_border(const RGB& rgb = bright_blue);

    void draw_spell_cursor(const RGB &fg, const Coords& xy);
    void draw_box_cursor(const RGB &fg, const Coords& xy);
    void draw_wings_cursor(const RGB &fg, const Coords& xy);
    void draw_ranged_cursor(const RGB &fg, const Coords& xy);

    void draw_creation(const Creation& creation, const Coords& xy, const int &sprite_index);
    void draw_creation_corpse(const Creation& creation, const Coords& xy);

    void draw_wizard_xy(const int &sprite_index, const RGB &rgb, const Coords& xy);
    void draw_wizard(const Wizard wizard, const Coords& xy, const int &frame_index);

    void draw_exploding_circle(const Coords& xy);
    void draw_exploding_circles(const std::vector<Coords> coords);
    void draw_explosion(const Coords& xy);
    void draw_attack(const Coords& xy);

    void simple_beam_animation(const RGB &rgb, const Coords& sxy, const Coords& dxy, const int& length);
    void spell_beam_animation(const Coords& sxy, const Coords& dxy);
    void burn_beam_animation(const Coords& sxy, const Coords& dxy);

    void draw_loading_screen();

    void draw_stats(const Creation& creation, const std::string& wizard_name = "");
    void draw_stats(const Wizard& wizard);
    void draw_spell(const Spell& spell);

    void draw_spellbook(const Wizard& wizard);

    void draw_title_screen_network();
    void draw_title_screen();
    void draw_enter_server();
    void draw_main_menu(const std::string& player_name, const int& world_alignment);
    void draw_player_entry_screen(const int& player_number);
    void draw_computer_controlled_question();
    void draw_which_character_question();
    void draw_which_colour_question(const int& sprite_index);

    void draw_info_text(const Tile& tile);
    void cast_spell_text(const Wizard& wizard, const Spell& spell);

    void draw_wizard_death(const int& sprite_index, const Coords& wizard_xy);
}
