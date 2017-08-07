#ifndef GFX_H_56B34357
#define GFX_H_56B34357

#include <string>

struct RGB {
    unsigned char red, green, blue;
    RGB(const unsigned char& red, const unsigned char& green, const unsigned char& blue);
    int get_24_bit_index() const;
};

namespace sdl {
    void refresh();
}

extern const RGB black, blue, red, purple, green, cyan, yellow, white, bright_black, bright_blue, bright_red, bright_purple, bright_green, bright_cyan, bright_yellow, bright_white;

namespace gfx {
    void draw_menu_border();
    void draw_spell_border();
    void draw_info_border();
    void draw_arena_border();

    void draw_spell_cursor(const RGB& fg, const int& x, const int& y);
    void draw_box_cursor(const RGB& fg, const int& x, const int& y);
    void draw_wings_cursor(const RGB& fg, const int& x, const int& y);
    void draw_ranged_cursor(const RGB& fg, const int& x, const int& y);

    void draw_text(std::string, const RGB& fg, const RGB& bg, const int& x, const int& y);

    void draw_king_cobra(const int& x, const int& y, const int& sprite_index);
    void draw_king_cobra_corpse(const int& x, const int& y);
    void draw_dire_wolf(const int& x, const int& y, const int& sprite_index);
    void draw_dire_wolf_corpse(const int& x, const int& y);
    void draw_goblin(const int& x, const int& y, const int& sprite_index);
    void draw_goblin_corpse(const int& x, const int& y);
    void draw_crocodile(const int& x, const int& y, const int& sprite_index);
    void draw_crocodile_corpse(const int& x, const int& y);
    void draw_faun(const int& x, const int& y, const int& sprite_index);
    void draw_faun_corpse(const int& x, const int& y);
    void draw_lion(const int& x, const int& y, const int& sprite_index);
    void draw_lion_corpse(const int& x, const int& y);
    void draw_elf(const int& x, const int& y, const int& sprite_index);
    void draw_elf_corpse(const int& x, const int& y);
    void draw_orc(const int& x, const int& y, const int& sprite_index);
    void draw_orc_corpse(const int& x, const int& y);
    void draw_bear(const int& x, const int& y, const int& sprite_index);
    void draw_bear_corpse(const int& x, const int& y);
    void draw_gorilla(const int& x, const int& y, const int& sprite_index);
    void draw_gorilla_corpse(const int& x, const int& y);
    void draw_ogre(const int& x, const int& y, const int& sprite_index);
    void draw_ogre_corpse(const int& x, const int& y);
    void draw_hydra(const int& x, const int& y, const int& sprite_index);
    void draw_hydra_corpse(const int& x, const int& y);
    void draw_giant_rat(const int& x, const int& y, const int& sprite_index);
    void draw_giant_rat_corpse(const int& x, const int& y);
    void draw_giant(const int& x, const int& y, const int& sprite_index);
    void draw_giant_corpse(const int& x, const int& y);
    void draw_horse(const int& x, const int& y, const int& sprite_index);
    void draw_horse_corpse(const int& x, const int& y);
    void draw_unicorn(const int& x, const int& y, const int& sprite_index);
    void draw_unicorn_corpse(const int& x, const int& y);
    void draw_centaur(const int& x, const int& y, const int& sprite_index);
    void draw_centaur_corpse(const int& x, const int& y);
    void draw_pegasus(const int& x, const int& y, const int& sprite_index);
    void draw_pegasus_corpse(const int& x, const int& y);
    void draw_gryphon(const int& x, const int& y, const int& sprite_index);
    void draw_gryphon_corpse(const int& x, const int& y);
    void draw_manticore(const int& x, const int& y, const int& sprite_index);
    void draw_manticore_corpse(const int& x, const int& y);
    void draw_bat(const int& x, const int& y, const int& sprite_index);
    void draw_bat_corpse(const int& x, const int& y);
    void draw_green_dragon(const int& x, const int& y, const int& sprite_index);
    void draw_green_dragon_corpse(const int& x, const int& y);
    void draw_red_dragon(const int& x, const int& y, const int& sprite_index);
    void draw_red_dragon_corpse(const int& x, const int& y);
    void draw_golden_dragon(const int& x, const int& y, const int& sprite_index);
    void draw_golden_dragon_corpse(const int& x, const int& y);
    void draw_harpy(const int& x, const int& y, const int& sprite_index);
    void draw_harpy_corpse(const int& x, const int& y);
    void draw_eagle(const int& x, const int& y, const int& sprite_index);
    void draw_eagle_corpse(const int& x, const int& y);
    void draw_vampire(const int& x, const int& y, const int& sprite_index);
    void draw_ghost(const int& x, const int& y, const int& sprite_index);
    void draw_spectre(const int& x, const int& y, const int& sprite_index);
    void draw_wraith(const int& x, const int& y, const int& sprite_index);
    void draw_skeleton(const int& x, const int& y, const int& sprite_index);
    void draw_zombie(const int& x, const int& y, const int& sprite_index);
    void draw_blob(const int& x, const int& y, const int& sprite_index);
    void draw_fire(const int& x, const int& y, const int& sprite_index);
    void draw_magic_wood(const int& x, const int& y, const int& sprite_index);
    void draw_shadow_wood(const int& x, const int& y, const int& sprite_index);
    void draw_magic_castle(const int& x, const int& y, const int& sprite_index);
    void draw_dark_citadel(const int& x, const int& y, const int& sprite_index);
    void draw_wall(const int& x, const int& y, const int& sprite_index);

    void draw_wizard(const int& sprite_index, const RGB& rgb, const int& x, const int& y);

    void draw_magic_sword(const int& sprite_index, const RGB& rgb, const int& x, const int& y);
    void draw_magic_knife(const int& sprite_index, const RGB& rgb, const int& x, const int& y);
    void draw_magic_armour(const int& sprite_index, const RGB& rgb, const int& x, const int& y);
    void draw_magic_shield(const int& sprite_index, const RGB& rgb, const int& x, const int& y);
    void draw_magic_wings(const int& sprite_index, const RGB& rgb, const int& x, const int& y);
    void draw_magic_bow(const int& sprite_index, const RGB& rgb, const int& x, const int& y);

    void draw_exploding_circle(const int& sprite_index, const int& x, const int& y);
    void draw_twirl(const int& sprite_index, const int& x, const int& y);
    void draw_explosion(const int& sprite_index, const int& x, const int& y);
    void draw_dragon_burn(const int& sprite_index, const int& x, const int& y);
    void draw_attack(const int& sprite_index, const int& x, const int& y);

    void draw_simple_beam(const RGB& rgb, const int& x, const int& y);
    void draw_spell_beam(const RGB& rgb, const int& x, const int& y);
    void draw_burn_beam(const RGB& rgb, const int& x, const int& y);
}

#endif /* end of include guard: GFX_H_56B34357 */
