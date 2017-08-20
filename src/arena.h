#pragma once
#include "wizard.h"
#include <map>

struct Tile {
    int frame_count = 0;
    int frame_index = 0;
    std::shared_ptr<Wizard> wizard;
    std::shared_ptr<Creation> creation;
    std::shared_ptr<Creation> corpse;
    void reset_frame_count();
    void tic(const int& anim_timing);
    void add(const std::shared_ptr<Wizard>& wizard);
    void add(const std::shared_ptr<Creation>& creation);
    void remove_wizard();
    void remove_creation();
    void end_turn();
    void move_wizard_to(Tile& tile);
    void move_creation_to(Tile& tile);
    void turn_creation_to_corpse();
    bool has_enemy(const std::shared_ptr<Wizard>& opposing_wizard);
};

namespace arena {
    extern std::map<int, std::map<int, Tile>> tiles;
    extern int world_alignment;

    void add(const std::shared_ptr<Wizard>& wizard);
    void add(const std::shared_ptr<Creation>& creation, const Coords& xy);
    Coords get_wizard_coords(const std::shared_ptr<Wizard>& wizard);
    void kill_creation(const Coords& xy);
    std::vector<Coords> coords_for_all_creations(const std::shared_ptr<Wizard>& wizard);
    void draw(const RGB& rgb = bright_blue);
    bool line_of_sight(const Coords& sxy, const Coords& dxy);
    void set_turn_flags(const std::shared_ptr<Wizard>& wizard);
    void clear_turn_flags(const std::shared_ptr<Wizard>& wizard);
    void display_tile_over_player_units(const std::shared_ptr<Wizard>& wizard);
    std::vector<Tile *> obtain_enemies_on_neighbouring_tiles(const std::shared_ptr<Wizard>& wizard, const Coords& xy);
}
