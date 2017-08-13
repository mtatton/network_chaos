#pragma once
#include "wizard.h"
#include <map>

struct Tile {
    int frame_count = 0;
    int frame_index = 0;
    std::shared_ptr<Wizard> wizard;
    std::shared_ptr<Creation> creation;
    std::shared_ptr<Wizard> creation_owner;
    std::shared_ptr<Creation> corpse;
    void reset();
    void tic(const int& anim_timing);
    void clear_wizard();
    void clear_creation();
    void add(std::shared_ptr<Wizard> wizard);
    void add(std::shared_ptr<Creation> creation, std::shared_ptr<Wizard> creation_owner);
    void move_wizard_to(Tile& tile);
    void move_creation_to(Tile& tile);
};

namespace arena {
    extern std::map<int, std::map<int, Tile>> tiles;

    void add(std::shared_ptr<Wizard> wizard);
    void add(std::shared_ptr<Creation> creation, std::shared_ptr<Wizard> creation_owner, const int& x, const int&y);
    void kill(const int& x, const int&y);
    void draw();
    Wizard * get_wizard(const int& x, const int&y);
    Creation * get_creation(const int& x, const int&y);
}
