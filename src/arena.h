#pragma once
#include "wizard.h"
#include <map>

struct Tile {
    int frame_count = 0;
    int frame_index = 0;
    std::shared_ptr<Wizard> wizard;
    std::shared_ptr<Creation> creation;
    std::shared_ptr<Wizard> creation_owner;
    bool illusion = false;
    std::shared_ptr<Creation> corpse;
    bool turn = false;
    void reset();
    void tic(const int& anim_timing);
    void clear_wizard();
    void clear_creation();
    void add(const std::shared_ptr<Wizard>& wizard);
    void add(const std::shared_ptr<Creation>& creation, const std::shared_ptr<Wizard>& creation_owner, const bool& illusion);
    void move_wizard_to(Tile& tile);
    void move_creation_to(Tile& tile);
};

namespace arena {
    extern std::map<int, std::map<int, Tile>> tiles;
    extern int world_alignment;

    void add(const std::shared_ptr<Wizard>& wizard);
    void add(const std::shared_ptr<Creation>& creation, const std::shared_ptr<Wizard>& creation_owner, const bool& illusion, const Coords& xy);
    Coords get_wizard_coords(const std::shared_ptr<Wizard>& wizard);
    void kill(const Coords& xy);
    void draw();
    bool line_of_sight(const Coords& sxy, const Coords& dxy);
    void set_turn_flags(const std::shared_ptr<Wizard>& wizard);
    void clear_turn_flags(const std::shared_ptr<Wizard>& wizard);
}
