#include "arena.h"
#include "gfx.h"
#include "audio.h"
#include "event.h"

void Tile::reset_frame_count() {
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

void Tile::add(const std::shared_ptr<Wizard>& wizard) {
    this->wizard = wizard;
    reset_frame_count();
}

void Tile::add(const std::shared_ptr<Creation>& creation) {
    this->creation = creation;
    reset_frame_count();
}

void Tile::remove_creation() {
    if(creation)
        creation.reset();
}

void Tile::remove_wizard() {
    if(wizard)
        wizard.reset();
}

void Tile::end_turn() {
    if(wizard)
        wizard->turn = false;
    if(creation)
        creation->turn = false;
}

void Tile::move_wizard_to(Tile& tile) {
    if(this != &tile) {
        if(wizard) {
            end_turn();
            tile.wizard = wizard;
            wizard.reset();
            tile.reset_frame_count();
        }
    }
}

void Tile::move_creation_to(Tile& tile) {
    if(this != &tile) {
        if(creation) {
            end_turn();
            tile.creation = creation;
            creation.reset();
            if(wizard)
                move_wizard_to(tile);
            tile.reset_frame_count();
        }
    }
}

void Tile::turn_creation_to_corpse() {
    if(creation) {
        if(corpse)
            corpse.reset();
        corpse = creation;
        creation.reset();
    }
}

bool Tile::has_enemy(const std::shared_ptr<Wizard>& opposing_wizard) {
    if(wizard && wizard != opposing_wizard)
        return true;
    if(creation && creation->owner != opposing_wizard)
        return true;
    return false;
}

void Tile::raise_dead(const std::shared_ptr<Wizard>& wizard) {
    if(corpse) {
        creation = corpse;
        corpse.reset();
        creation->undead = true;
        creation->owner = wizard;
    }
}

namespace arena {
    int number_of_wizards = 0;

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
        if(number_of_wizards > 0) {
            for(int i = 0; i < number_of_wizards; ++i) {
                auto old_coords = initial_positions[number_of_wizards - 1][i];
                auto new_coords = initial_positions[number_of_wizards][i];
                tiles[old_coords.x][old_coords.y].move_wizard_to(tiles[new_coords.x][new_coords.y]);
            }
        }
        auto coords = initial_positions[number_of_wizards][number_of_wizards];
        tiles[coords.x][coords.y].add(wizard);
        number_of_wizards += 1;
    }

    void add(const std::shared_ptr<Creation>& creation, const Coords& xy) {
        tiles[xy.x][xy.y].add(creation);
    }

    Coords get_wizard_coords(const std::shared_ptr<Wizard>& wizard) {
        for(int y = 0; y < 10; ++y)
            for(int x = 0; x < 15; ++x)
                if(tiles[x][y].wizard == wizard)
                    return {x, y};
        return {0, 0};
    }

    void kill_creation(const Coords& xy) {
        auto& tile = tiles[xy.x][xy.y];
        if(tile.creation->illusion || tile.creation->undead)
            tile.remove_creation();
        else
            tile.turn_creation_to_corpse();
    }

    std::vector<Coords> coords_for_all_creations(const std::shared_ptr<Wizard>& wizard) {
        std::vector<Coords> coords;
        for(int y = 0; y < 10; ++y) {
            for(int x = 0; x < 15; ++x) {
                auto& tile = tiles[x][y];
                if(tile.creation && tile.creation->owner == wizard)
                    coords.push_back({x, y});
            }
        }
        return coords;
    }

    void draw(const RGB& rgb) {
        gfx::draw_arena_border(rgb);
        for(int y = 0; y < 10; ++y) {
            for(int x = 0; x < 15; ++x) {
                auto& tile = tiles[x][y];
                if(tile.creation) {
                    auto& creation = *tiles[x][y].creation;
                    gfx::draw_creation(creation, {x, y}, tile.frame_index);
                    tile.tic(creation.anim_timing);
                } else if(tile.wizard) {
                    auto& wizard = *tiles[x][y].wizard;
                    if(!wizard.shadow_form || tile.frame_count >= 15)
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
                if(tile.creation && tile.creation->owner == wizard)
                    tile.creation->turn = value;
                else if(tile.wizard && tile.wizard == wizard)
                    tile.wizard->turn = value;
            }
        }
    }

    void set_turn_flags(const std::shared_ptr<Wizard>& wizard) {
        change_turn_flags(wizard, true);
    }

    void clear_turn_flags(const std::shared_ptr<Wizard>& wizard) {
        change_turn_flags(wizard, false);
    }

    void display_tile_over_player_units(const std::shared_ptr<Wizard>& wizard) {
        for(int y = 0; y < 10; ++y) {
            for(int x = 0; x < 15; ++x) {
                auto& tile = tiles[x][y];
                if((tile.creation && tile.creation->owner == wizard) || (tile.wizard && tile.wizard == wizard))
                    gfx::draw_box_cursor(wizard->rgb, {x, y});
            }
        }
    }

    std::vector<Tile *> obtain_enemies_on_neighbouring_tiles(const std::shared_ptr<Wizard>& wizard, const Coords& xy) {
        std::vector<Tile *> neighbouring_enemies;
        if(xy.y > 0) {
            if(xy.x > 0 && tiles[xy.x - 1][xy.y - 1].has_enemy(wizard))
                neighbouring_enemies.push_back(&tiles[xy.x - 1][xy.y - 1]);
            if(tiles[xy.x][xy.y - 1].has_enemy(wizard))
                neighbouring_enemies.push_back(&tiles[xy.x][xy.y - 1]);
            if(xy.x < 14 && tiles[xy.x + 1][xy.y - 1].has_enemy(wizard))
                neighbouring_enemies.push_back(&tiles[xy.x + 1][xy.y - 1]);
        }
        if(xy.x > 0 && tiles[xy.x - 1][xy.y].has_enemy(wizard))
            neighbouring_enemies.push_back(&tiles[xy.x - 1][xy.y]);
        if(xy.x < 14 && tiles[xy.x + 1][xy.y].has_enemy(wizard))
            neighbouring_enemies.push_back(&tiles[xy.x + 1][xy.y]);
        if(xy.y < 9) {
            if(xy.x > 0 && tiles[xy.x - 1][xy.y + 1].has_enemy(wizard))
                neighbouring_enemies.push_back(&tiles[xy.x - 1][xy.y + 1]);
            if(tiles[xy.x][xy.y + 1].has_enemy(wizard))
                neighbouring_enemies.push_back(&tiles[xy.x][xy.y + 1]);
            if(xy.x < 14 && tiles[xy.x + 1][xy.y + 1].has_enemy(wizard))
                neighbouring_enemies.push_back(&tiles[xy.x + 1][xy.y + 1]);
        }
        return neighbouring_enemies;
    }

    void clear() {
        number_of_wizards = 0;
        world_alignment = 0;
        for(int y = 0; y < 10; ++y) {
            for(int x = 0; x < 15; ++x) {
                auto& tile = tiles[x][y];
                tile.remove_creation();
                tile.remove_wizard();
                tile.reset_frame_count();
            }
        }
    }
}
