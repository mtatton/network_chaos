#include "arena.h"
#include "gfx.h"
#include "audio.h"
#include "event.h"

void Tile::reset() {
    frame_count = 0;
    frame_index = 0;
}

void Tile::tic(const int& anim_timing) {
    if(frame_count == anim_timing) {
        frame_count = 0;
        frame_index += 1;
        if(frame_index == 4)
            frame_index = 0;
    }
    frame_count += 1;
}

void Tile::clear_wizard() {
    reset();
    wizard.reset();
    illusion = false;
    turn = false;
}

void Tile::clear_creation() {
    reset();
    creation.reset();
    creation_owner.reset();
    illusion = false;
    turn = false;
}

void Tile::add(const std::shared_ptr<Wizard>& wizard) {
    reset();
    this->wizard = wizard;
}

void Tile::add(const std::shared_ptr<Creation>& creation, const std::shared_ptr<Wizard>& creation_owner, const bool& illusion) {
    reset();
    this->creation = creation;
    this->creation_owner = creation_owner;
    this->illusion = illusion;
}

void Tile::move_wizard_to(Tile& tile) {
    if(this != &tile) {
        if(wizard) {
            tile.wizard = wizard;
            tile.turn = false;
            tile.reset();
            clear_wizard();
        }
    }
}

void Tile::move_creation_to(Tile& tile) {
    if(this != &tile) {
        if(creation) {
            tile.creation = creation;
            tile.creation_owner = creation_owner;
            tile.illusion = illusion;
            tile.reset();
            if(wizard)
                move_wizard_to(tile);
            clear_creation();
        }
    }
}

namespace arena {
    std::vector<std::shared_ptr<Wizard>> wizards;
    std::vector<std::shared_ptr<Creation>> creations;

    std::vector<std::vector<Coords>> initial_positions = {
        {{1, 4}},
        {{1, 4}, {13, 4}},
        {{7, 1}, {1, 8}, {13, 8}},
        {{1, 1}, {13, 1}, {1, 8}, {13, 8}},
        {{7, 0}, {0, 3}, {14, 3}, {3, 9}, {11, 9}},
        {{7, 0}, {0, 1}, {14, 1}, {0, 8}, {7, 9}, {14, 8}},
        {{7, 0}, {1, 1}, {13, 1}, {0, 6}, {14, 6}, {4, 9}, {10, 9}},
        {{0, 0}, {7, 0}, {14, 0}, {0, 4}, {14, 4}, {0, 9}, {7, 9}, {14, 9}}
    };

    std::map<int, std::map<int, Tile>> tiles;
    int world_alignment = 0;


    void add(const std::shared_ptr<Wizard>& wizard) {
        auto number_of_wizards = wizards.size();
        if(number_of_wizards > 0) {
            for(int i = 0; i < number_of_wizards; ++i) {
                auto old_coords = initial_positions[number_of_wizards - 1][i];
                auto new_coords = initial_positions[number_of_wizards][i];
                tiles[old_coords.x][old_coords.y].move_wizard_to(tiles[new_coords.x][new_coords.y]);
            }
        }
        auto coords = initial_positions[number_of_wizards][number_of_wizards];
        tiles[coords.x][coords.y].add(wizard);
        wizards.push_back(wizard);
    }

    void add(const std::shared_ptr<Creation>& creation, const std::shared_ptr<Wizard>& creation_owner, const bool& illusion, const Coords& xy) {
        tiles[xy.x][xy.y].add(creation, creation_owner, illusion);
        creations.push_back(creation);
    }

    Coords get_wizard_coords(const std::shared_ptr<Wizard>& wizard) {
        for(int y = 0; y < 10; ++y)
            for(int x = 0; x < 15; ++x)
                if(tiles[x][y].wizard == wizard)
                    return {x, y};
        return {0, 0};
    }

    void kill(const Coords& xy) {
        auto& tile = tiles[xy.x][xy.y];
        if(tile.creation) {
            tile.corpse = tile.creation;
            tile.clear_creation();
        }
    }

    void draw() {
        gfx::draw_arena_border();
        for(int y = 0; y < 10; ++y) {
            for(int x = 0; x < 15; ++x) {
                auto& tile = tiles[x][y];
                if(tile.creation) {
                    auto& creation = *tiles[x][y].creation;
                    gfx::draw_creation(creation, {x, y}, tile.frame_index);
                    tile.tic(creation.anim_timing);
                } else if(tile.wizard) {
                    auto& wizard = *tiles[x][y].wizard;
                    gfx::draw_wizard(wizard, {x, y}, tile.frame_index);
                    tile.tic(wizard.anim_timing);
                } else if(tile.corpse) {
                    auto& corpse = *tiles[x][y].corpse;
                    gfx::draw_creation_corpse(corpse, {x, y});
                }
            }
        }
    }

    bool line_of_sight(const Coords& sxy, const Coords& dxy) {
        auto coords = sxy.line_to(dxy);
        for(auto& coord:coords) {
            if(tiles[coord.x][coord.y].creation)
                if(!tiles[coord.x][coord.y].creation->transparent)
                    return false;
            if(tiles[coord.x][coord.y].corpse)
                if(!tiles[coord.x][coord.y].corpse->transparent)
                    return false;
            if(tiles[coord.x][coord.y].wizard)
                return false;
        }
        return true;
    }

    void change_turn_flags(const std::shared_ptr<Wizard>& wizard, const bool& value) {
        for(int y = 0; y < 10; ++y) {
            for(int x = 0; x < 15; ++x) {
                auto& tile = tiles[x][y];
                if(tile.creation && tile.creation_owner == wizard)
                    tile.turn = value;
                else if(tile.wizard && tile.wizard == wizard)
                    tile.turn = value;
            }
        }
    }

    void set_turn_flags(const std::shared_ptr<Wizard>& wizard) {
        change_turn_flags(wizard, true);
    }

    void clear_turn_flags(const std::shared_ptr<Wizard>& wizard) {
        change_turn_flags(wizard, false);
    }
}
