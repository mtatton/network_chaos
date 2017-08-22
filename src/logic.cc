#include "logic.h"
#include "event.h"
#include "arena.h"
#include "audio.h"

struct Turn {
    std::shared_ptr<Wizard> wizard;
    ChosenSpell chosen_spell;
    bool alive = true;

    Turn(const Player& player) {
        wizard = std::shared_ptr<Wizard>(new Wizard(player.name, 0, player.sprite_index, player.rgb));
    }
};

namespace logic {
    std::vector<Turn> turns;

    void disbelieve(const std::shared_ptr<Wizard>& wizard) {
        auto wizard_coords = arena::get_wizard_coords(wizard);
        ui::cursor = wizard_coords;
        while(true) {
            auto option = ui::select_tile_for_spell();
            auto& tile = arena::tiles[ui::cursor.x][ui::cursor.y];
            switch(option) {
            case CursorOption::select:
                if(tile.creation && tile.creation->owner != wizard) {
                    gfx::spell_beam_animation(wizard_coords, ui::cursor);
                    arena::draw();
                    if(tile.creation->illusion) {
                        event::delay(15);
                        audio::play_explosion();
                        gfx::draw_explosion(ui::cursor);
                        tile.remove_creation();
                        arena::draw();
                        gfx::draw_spell_succeeds();
                    } else {
                        gfx::draw_spell_fails();
                    }
                    event::wait_for_key(40);
                    return;
                }
                break;
            case CursorOption::cancel:
                return;
                break;
            default:
                break;
            }
        }
    }

    void creation_spell(const std::shared_ptr<Wizard>& wizard, const Spell& spell, const bool& illusion) {
        auto wizard_coords = arena::get_wizard_coords(wizard);
        std::shared_ptr<Creation> creation;
        ui::cursor = wizard_coords;
        while(true) {
            auto option = ui::select_tile_for_spell();
            if(option == CursorOption::cancel)
                return;
            if(option == CursorOption::select) {
                auto& tile = arena::tiles[ui::cursor.x][ui::cursor.y];
                if(!tile.wizard && !tile.creation) {
                    if(!spell.within_range(wizard_coords, ui::cursor)) {
                        gfx::draw_out_of_range(bright_cyan);
                        event::delay(30);
                    } else {
                        gfx::spell_beam_animation(wizard_coords, ui::cursor);
                        if(illusion || spell.cast()) {
                            arena::world_alignment += spell.alignment;
                            creation = std::make_shared<Creation>(wizard::generate_creation_from_id(spell.id));
                            creation->owner = wizard;
                            creation->illusion = illusion;
                            arena::add(creation, ui::cursor);
                            arena::draw();
                            gfx::draw_spell_succeeds();
                        } else {
                            gfx::draw_spell_fails();
                        }
                        event::wait_for_key(40);
                        return;
                    }
                }
            }
        }
    }

    void alignment_spell(const std::shared_ptr<Wizard>& wizard, const Spell& spell) {
        if(spell.cast()) {
            arena::world_alignment += spell.alignment;
            gfx::draw_spell_succeeds();
        } else {
            gfx::draw_spell_fails();
        }
        event::wait_for_key(60);
    }

    void magic_shield_spell(const std::shared_ptr<Wizard>& wizard, const Spell& spell) {
        auto xy = arena::get_wizard_coords(wizard);
        gfx::spell_beam_animation(xy, xy);
        arena::draw();
        if(spell.cast()) {
            arena::world_alignment += spell.alignment;
            wizard->gain_magic_shield();
            arena::draw();
            gfx::draw_spell_succeeds();
        } else {
            gfx::draw_spell_fails();
        }
        event::wait_for_key(60);
    }

    void magic_armour_spell(const std::shared_ptr<Wizard>& wizard, const Spell& spell) {
        auto xy = arena::get_wizard_coords(wizard);
        gfx::spell_beam_animation(xy, xy);
        arena::draw();
        if(spell.cast()) {
            arena::world_alignment += spell.alignment;
            wizard->gain_magic_armour();
            arena::draw();
            gfx::draw_spell_succeeds();
        } else {
            gfx::draw_spell_fails();
        }
        event::wait_for_key(60);
    }

    void magic_sword_spell(const std::shared_ptr<Wizard>& wizard, const Spell& spell) {
        auto xy = arena::get_wizard_coords(wizard);
        gfx::spell_beam_animation(xy, xy);
        arena::draw();
        if(spell.cast()) {
            arena::world_alignment += spell.alignment;
            wizard->gain_magic_sword();
            arena::draw();
            gfx::draw_spell_succeeds();
        } else {
            gfx::draw_spell_fails();
        }
        event::wait_for_key(60);
    }

    void magic_knife_spell(const std::shared_ptr<Wizard>& wizard, const Spell& spell) {
        auto xy = arena::get_wizard_coords(wizard);
        gfx::spell_beam_animation(xy, xy);
        arena::draw();
        if(spell.cast()) {
            arena::world_alignment += spell.alignment;
            wizard->gain_magic_knife();
            arena::draw();
            gfx::draw_spell_succeeds();
        } else {
            gfx::draw_spell_fails();
        }
        event::wait_for_key(60);
    }

    void magic_bow_spell(const std::shared_ptr<Wizard>& wizard, const Spell& spell) {
        auto xy = arena::get_wizard_coords(wizard);
        gfx::spell_beam_animation(xy, xy);
        arena::draw();
        if(spell.cast()) {
            arena::world_alignment += spell.alignment;
            wizard->gain_magic_bow();
            arena::draw();
            gfx::draw_spell_succeeds();
        } else {
            gfx::draw_spell_fails();
        }
        event::wait_for_key(60);
    }

    void magic_wings_spell(const std::shared_ptr<Wizard>& wizard, const Spell& spell) {
        auto xy = arena::get_wizard_coords(wizard);
        gfx::spell_beam_animation(xy, xy);
        arena::draw();
        if(spell.cast()) {
            arena::world_alignment += spell.alignment;
            wizard->gain_magic_wings();
            arena::draw();
            gfx::draw_spell_succeeds();
        } else {
            gfx::draw_spell_fails();
        }
        event::wait_for_key(60);
    }

    void shadow_form_spell(const std::shared_ptr<Wizard>& wizard, const Spell& spell) {
        auto xy = arena::get_wizard_coords(wizard);
        gfx::spell_beam_animation(xy, xy);
        arena::draw();
        if(spell.cast()) {
            arena::world_alignment += spell.alignment;
            wizard->gain_shadow_form();
            arena::draw();
            gfx::draw_spell_succeeds();
        } else {
            gfx::draw_spell_fails();
        }
        event::wait_for_key(60);
    }

    void subversion_spell(const std::shared_ptr<Wizard>& wizard, const Spell& spell) {
        auto wizard_coords = arena::get_wizard_coords(wizard);
        std::shared_ptr<Creation> creation;
        ui::cursor = wizard_coords;
        while(true) {
            auto option = ui::select_tile_for_spell();
            if(option == CursorOption::cancel)
                return;
            if(option == CursorOption::select) {
                auto& tile = arena::tiles[ui::cursor.x][ui::cursor.y];
                if(tile.creation && tile.creation->owner != wizard && !tile.wizard) {
                    if(!spell.within_range(wizard_coords, ui::cursor)) {
                        gfx::draw_out_of_range(bright_cyan);
                        event::delay(30);
                    } else if(!arena::line_of_sight(wizard_coords, ui::cursor)) {
                        gfx::no_line_of_sight();
                        event::delay(30);
                    } else {
                        gfx::spell_beam_animation(wizard_coords, ui::cursor);
                        if(!tile.creation->illusion && tile.creation->subverted()) {
                            tile.creation->owner = wizard;
                            gfx::draw_spell_succeeds();
                        } else {
                            gfx::draw_spell_fails();
                        }
                        event::wait_for_key(40);
                        return;
                    }
                }
            }
        }
    }

    void raise_dead_spell(const std::shared_ptr<Wizard>& wizard, const Spell& spell) {
        auto wizard_coords = arena::get_wizard_coords(wizard);
        std::shared_ptr<Creation> creation;
        ui::cursor = wizard_coords;
        while(true) {
            auto option = ui::select_tile_for_spell();
            if(option == CursorOption::cancel)
                return;
            if(option == CursorOption::select) {
                auto& tile = arena::tiles[ui::cursor.x][ui::cursor.y];
                if(tile.corpse && !tile.creation && !tile.wizard) {
                    if(!spell.within_range(wizard_coords, ui::cursor)) {
                        gfx::draw_out_of_range(bright_cyan);
                        event::delay(30);
                    } else if(!arena::line_of_sight(wizard_coords, ui::cursor)) {
                        gfx::no_line_of_sight();
                        event::delay(30);
                    } else {
                        gfx::spell_beam_animation(wizard_coords, ui::cursor);
                        if(spell.cast()) {
                            tile.raise_dead(wizard);
                            arena::draw();
                            gfx::draw_spell_succeeds();
                        } else {
                            gfx::draw_spell_fails();
                        }
                        event::wait_for_key(40);
                        return;
                    }
                }
            }
        }
    }

    void spell_turn(const std::shared_ptr<Wizard>& wizard, const ChosenSpell& chosen_spell) {
        if(chosen_spell.spell_index != -1) {
            Spell spell = wizard->spellbook[chosen_spell.spell_index];
            if(spell.type != Spell::disbelieve) {
                wizard->spellbook.erase(wizard->spellbook.begin() + chosen_spell.spell_index);
                wizard->number_of_spells -= 1;
            }
            arena::draw();
            gfx::cast_spell_text(*wizard, spell);
            switch(spell.type) {
            case Spell::disbelieve:
                disbelieve(wizard);
                break;
            case Spell::creation:
                creation_spell(wizard, spell, chosen_spell.illusion);
                break;
            case Spell::magic_shield:
                magic_shield_spell(wizard, spell);
                break;
            case Spell::magic_armour:
                magic_armour_spell(wizard, spell);
                break;
            case Spell::magic_sword:
                magic_sword_spell(wizard, spell);
                break;
            case Spell::magic_knife:
                magic_knife_spell(wizard, spell);
                break;
            case Spell::magic_bow:
                magic_bow_spell(wizard, spell);
                break;
            case Spell::magic_wings:
                magic_wings_spell(wizard, spell);
                break;
            case Spell::change_alignment:
                alignment_spell(wizard, spell);
                break;
            case Spell::shadow_form:
                shadow_form_spell(wizard, spell);
                break;
            case Spell::subversion:
                subversion_spell(wizard, spell);
                break;
            case Spell::raise_dead:
                raise_dead_spell(wizard, spell);
                break;
            default:
                break;
            }
        }
    }

    void perform_wizard_death(const std::shared_ptr<Wizard> wizard) {
        auto xy = arena::get_wizard_coords(wizard);
        arena::tiles[xy.x][xy.y].remove_wizard();
        gfx::draw_wizard_death(wizard->sprite_index, xy);
        for(auto& turn:turns)
            if(turn.wizard == wizard)
                turn.alive = false;
        auto coords = arena::coords_for_all_creations(wizard);
        for(const auto& xy:coords)
            arena::kill_creation(xy);
        gfx::draw_exploding_circles(coords);
    }

    bool perform_attack(const std::shared_ptr<Creation>& creation, const Coords& xy) {
        auto& dest = arena::tiles[xy.x][xy.y];
        audio::play_attack();
        gfx::draw_attack(xy);
        if(dest.creation) {
             if(!wizard::attack(*creation, *dest.creation))
                 return false;
             arena::kill_creation(xy);
        } else if(dest.wizard) {
             if(!wizard::attack(*creation, *dest.wizard))
                 return false;
             perform_wizard_death(dest.wizard);
        }
        if(!dest.wizard) {
            arena::tiles[ui::cursor.x][ui::cursor.y].move_creation_to(arena::tiles[xy.x][xy.y]);
            arena::draw();
            audio::play_move();
        }
        return true;
    }

    void attack_turn(const std::shared_ptr<Creation>& creation) {
        auto xy = ui::cursor;
        while(true) {
            auto option = ui::attack(xy);
            if(option == CursorOption::cancel) {
                audio::play_cancel();
                return;
            }
            if(option == CursorOption::move) {
                auto dest = arena::tiles[xy.x][xy.y];
                if(dest.creation && dest.creation->owner != creation->owner) {
                    if(dest.creation->undead && !creation->undead) {
                        gfx::undead_cannot_be_attacked();
                        audio::play_undead();
                    } else {
                        if(perform_attack(creation, xy))
                            ui::cursor = xy;
                        return;
                    }
                } else if(dest.wizard && dest.wizard != creation->owner) {
                    if(perform_attack(creation, xy))
                        ui::cursor = xy;
                    return;
                } else {
                    xy = ui::cursor;
                }
            }
        }
    }

    bool perform_attack(const std::shared_ptr<Wizard>& wizard, const Coords& xy) {
        auto& dest = arena::tiles[xy.x][xy.y];
        audio::play_attack();
        gfx::draw_attack(xy);
        if(dest.creation) {
             if(!wizard::attack(*wizard, *dest.creation))
                 return false;
             arena::kill_creation(xy);
        } else if(dest.wizard) {
             if(!wizard::attack(*wizard, *dest.wizard))
                 return false;
             perform_wizard_death(dest.wizard);
        }
        if(!dest.wizard) {
            arena::tiles[ui::cursor.x][ui::cursor.y].move_wizard_to(arena::tiles[xy.x][xy.y]);
            arena::draw();
            audio::play_move();
        }
        return true;
    }

    void attack_turn(const std::shared_ptr<Wizard>& wizard) {
        auto xy = ui::cursor;
        while(true) {
            auto option = ui::attack(xy);
            if(option == CursorOption::cancel) {
                audio::play_cancel();
                return;
            }
            if(option == CursorOption::move) {
                auto dest = arena::tiles[xy.x][xy.y];
                if(dest.creation && dest.creation->owner != wizard) {
                    if(dest.creation->undead && !(wizard->magic_knife | wizard->magic_sword)) {
                        gfx::undead_cannot_be_attacked();
                        audio::play_undead();
                    } else {
                        if(perform_attack(wizard, xy))
                            ui::cursor = xy;
                        return;
                    }
                } else if(dest.wizard && dest.wizard != wizard) {
                    if(perform_attack(wizard, xy))
                        ui::cursor = xy;
                    return;
                } else {
                    xy = ui::cursor;
                }
            }
        }
    }

    void ranged_attack_turn(const std::shared_ptr<Creation>& creation) {
        auto start = ui::cursor;
        arena::draw(bright_purple);
        gfx::draw_ranged_combat(creation->range);
        audio::play_ranged_attack();
        while(true) {
            auto option = ui::select_tile_for_ranged_combat();
            Tile& dest = arena::tiles[ui::cursor.x][ui::cursor.y];
            switch(option) {
            case CursorOption::cancel:
                audio::play_cancel();
                return;
            case CursorOption::select:
                if(!creation->within_ranged_combat_range(start, ui::cursor)) {
                    gfx::draw_out_of_range(bright_cyan);
                    event::delay(30);
                    break;
                }
                if(!arena::line_of_sight(start, ui::cursor)) {
                    gfx::no_line_of_sight();
                    event::delay(30);
                    break;
                }
                if(creation->id >= 23 && creation->id <= 25)
                    gfx::burn_beam_animation(start, ui::cursor);
                else
                    gfx::simple_beam_animation(bright_white, start, ui::cursor, 8);
                if(dest.creation) {
                    if(dest.creation->undead && !creation->undead) {
                        gfx::undead_cannot_be_attacked();
                        audio::play_undead();
                    } else if(wizard::ranged_attack(*creation, *dest.creation)) {
                        arena::kill_creation(ui::cursor);
                    }
                } else if(dest.wizard) {
                    if(wizard::ranged_attack(*creation, *dest.wizard))
                        perform_wizard_death(dest.wizard);
                }
                return;
            default:
            break;
            }
        }
    }

    void ranged_attack_turn(const std::shared_ptr<Wizard>& wizard) {
        auto start = ui::cursor;
        arena::draw(bright_purple);
        gfx::draw_ranged_combat(wizard->range);
        audio::play_ranged_attack();
        while(true) {
            auto option = ui::select_tile_for_ranged_combat();
            Tile& dest = arena::tiles[ui::cursor.x][ui::cursor.y];
            switch(option) {
            case CursorOption::cancel:
                audio::play_cancel();
                return;
            case CursorOption::select:
                if(!wizard->within_ranged_combat_range(start, ui::cursor)) {
                    gfx::draw_out_of_range(bright_cyan);
                    event::delay(30);
                    break;
                }
                if(!arena::line_of_sight(start, ui::cursor)) {
                    gfx::no_line_of_sight();
                    event::delay(30);
                    break;
                }
                gfx::simple_beam_animation(bright_white, start, ui::cursor, 8);
                if(dest.creation) {
                    if(wizard::ranged_attack(*wizard, *dest.creation))
                        arena::kill_creation(ui::cursor);
                } else if(dest.wizard) {
                    if(wizard::ranged_attack(*wizard, *dest.wizard))
                        perform_wizard_death(dest.wizard);
                }
                return;
            default:
            break;
            }
        }
    }

    bool check_for_engaged(const std::shared_ptr<Wizard>& wizard, const Coords& xy, const bool& skip_engage_check) {
        auto neighbouring_enemies = arena::obtain_enemies_on_neighbouring_tiles(wizard, xy);
        if(neighbouring_enemies.size()) {
            if(skip_engage_check) {
                gfx::draw_engaged_to_enemy();
                sdl::refresh();
                audio::play_engaged_to_enemy();
                return true;
            }
            for(auto& other_tile:neighbouring_enemies) {
                if(!wizard->disengage(other_tile->creation ? static_cast<Unit>(*other_tile->creation) : static_cast<Unit>(*other_tile->wizard))) {
                    gfx::draw_engaged_to_enemy();
                    sdl::refresh();
                    audio::play_engaged_to_enemy();
                    return true;
                }
            }
        }
        return false;
    }

    bool check_for_engaged(const std::shared_ptr<Creation>& creation, const Coords& xy, const bool& skip_engage_check) {
        auto neighbouring_enemies = arena::obtain_enemies_on_neighbouring_tiles(creation->owner, xy);
        if(neighbouring_enemies.size()) {
            if(skip_engage_check) {
                gfx::draw_engaged_to_enemy();
                sdl::refresh();
                audio::play_engaged_to_enemy();
                return true;
            }
            for(auto& other_tile:neighbouring_enemies) {
                if(!creation->disengage(other_tile->creation ? static_cast<Unit>(*other_tile->creation) : static_cast<Unit>(*other_tile->wizard))) {
                    gfx::draw_engaged_to_enemy();
                    sdl::refresh();
                    audio::play_engaged_to_enemy();
                    return true;
                }
            }
        }
        return false;
    }

    void fly_turn(const std::shared_ptr<Creation>& creation) {
        auto start = ui::cursor;
        int movement = creation->movement;
        while(true) {
            auto option = ui::select_tile_for_flying(movement);
            Tile& dest = arena::tiles[ui::cursor.x][ui::cursor.y];
            if(option == CursorOption::cancel) {
                arena::tiles[start.x][start.y].end_turn();
                audio::play_cancel();
                return;
            }
            if(option == CursorOption::select) {
                if(creation->within_flying_range(start, ui::cursor)) {
                    if(dest.creation && dest.creation->owner != creation->owner) {
                        audio::play_landing();
                        if(perform_attack(creation, ui::cursor)) {
                            if(!dest.wizard) {
                                arena::tiles[start.x][start.y].move_creation_to(dest);
                                arena::draw();
                            }
                        }
                        return;
                    } else if(dest.wizard && dest.wizard != creation->owner) {
                        audio::play_landing();
                        if(perform_attack(creation, ui::cursor)) {
                            arena::tiles[start.x][start.y].move_creation_to(dest);
                            arena::draw();
                        }
                        return;
                    } else if(!dest.creation && !dest.wizard) {
                        audio::play_landing();
                        arena::tiles[start.x][start.y].move_creation_to(dest);
                        arena::draw();
                        if(check_for_engaged(creation, ui::cursor, true))
                            attack_turn(creation);
                        return;
                    }
                }
                gfx::draw_out_of_range(bright_yellow);
                event::wait_for_key(50);
            }
        }
    }

    void fly_turn(const std::shared_ptr<Wizard>& wizard) {
        auto start = ui::cursor;
        while(true) {
            auto option = ui::select_tile_for_flying(6);
            Tile& dest = arena::tiles[ui::cursor.x][ui::cursor.y];
            if(option == CursorOption::cancel) {
                arena::tiles[start.x][start.y].end_turn();
                audio::play_cancel();
                return;
            }
            if(option == CursorOption::select) {
                if(wizard->within_flying_range(start, ui::cursor)) {
                    if(dest.creation && dest.creation->owner == wizard && dest.creation->mount) {
                        dest.creation->turn = false;
                        audio::play_landing();
                        arena::tiles[start.x][start.y].move_wizard_to(dest);
                        arena::draw();
                        if(dest.creation->range)
                            ranged_attack_turn(dest.creation);
                        return;
                    } else if(dest.creation && dest.creation->owner != wizard) {
                        audio::play_landing();
                        if(perform_attack(wizard, ui::cursor)) {
                            if(!dest.wizard) {
                                arena::tiles[start.x][start.y].move_wizard_to(dest);
                                arena::draw();
                            }
                        }
                        return;
                    } else if(dest.wizard && dest.wizard != wizard) {
                        audio::play_landing();
                        if(perform_attack(wizard, ui::cursor)) {
                            arena::tiles[start.x][start.y].move_wizard_to(dest);
                            arena::draw();
                        }
                        return;
                    } else if(!dest.creation && !dest.wizard) {
                        audio::play_landing();
                        arena::tiles[start.x][start.y].move_wizard_to(dest);
                        arena::draw();
                        if(check_for_engaged(wizard, ui::cursor, true))
                            attack_turn(wizard);
                        return;
                    }
                }
                gfx::draw_out_of_range(bright_yellow);
                event::wait_for_key(50);
            }
        }
    }

    void walk_turn(const std::shared_ptr<Creation>& creation) {
        auto xy = ui::cursor;
        int movement = creation->movement;
        while(true) {
            auto option = ui::walking(xy, movement);
            if(option == CursorOption::cancel) {
                arena::tiles[ui::cursor.x][ui::cursor.y].end_turn();
                audio::play_cancel();
                return;
            }
            if(option == CursorOption::move) {
                auto dest = arena::tiles[xy.x][xy.y];
                if(dest.creation) {
                    if(dest.creation->owner != creation->owner) {
                        if(perform_attack(creation, xy) && !dest.wizard) {
                            arena::tiles[ui::cursor.x][ui::cursor.y].move_creation_to(arena::tiles[xy.x][xy.y]);
                            ui::cursor = xy;
                        } else {
                            xy = ui::cursor;
                        }
                        return; 
                    }
                } else if(dest.wizard) {
                    if(dest.wizard != creation->owner) {
                        if(perform_attack(creation, xy)) {
                            arena::tiles[ui::cursor.x][ui::cursor.y].move_creation_to(arena::tiles[xy.x][xy.y]);
                            ui::cursor = xy;
                        } else {
                            xy = ui::cursor;
                        }
                        return; 
                    }
                } else {
                    arena::tiles[ui::cursor.x][ui::cursor.y].move_creation_to(arena::tiles[xy.x][xy.y]);
                    arena::draw();
                    audio::play_move();
                    ui::cursor = xy;
                    if(check_for_engaged(creation, xy, true)) {
                        attack_turn(creation);
                        return;
                    }
                    movement -= 1;
                    if(movement == 0)
                        return;
                }
            }
            xy = ui::cursor;
        }
    }

    void walk_turn(const std::shared_ptr<Wizard>& wizard) {
        auto xy = ui::cursor;
        int movement = wizard->shadow_form ? 3 : wizard->movement;
        while(true) {
            auto option = ui::walking(xy, movement, true);
            if(option == CursorOption::cancel) {
                arena::tiles[ui::cursor.x][ui::cursor.y].end_turn();
                audio::play_cancel();
                return;
            }
            if(option == CursorOption::move) {
                auto dest = arena::tiles[xy.x][xy.y];
                if(dest.creation) {
                    if(dest.creation->owner != wizard) {
                        if(perform_attack(wizard, xy) && !dest.wizard) {
                            arena::tiles[ui::cursor.x][ui::cursor.y].move_wizard_to(arena::tiles[xy.x][xy.y]);
                            ui::cursor = xy;
                        } else {
                            xy = ui::cursor;
                        }
                        return; 
                    } else if(dest.creation->mount) {
                        dest.creation->turn = false;
                        arena::tiles[ui::cursor.x][ui::cursor.y].move_wizard_to(arena::tiles[xy.x][xy.y]);
                        arena::draw();
                        audio::play_move();
                        ui::cursor = xy;
                        if(dest.creation->range)
                            ranged_attack_turn(dest.creation);
                        return;
                    }
                } else if(dest.wizard) {
                    if(dest.wizard != wizard) {
                        if(perform_attack(wizard, xy)) {
                            arena::tiles[ui::cursor.x][ui::cursor.y].move_wizard_to(arena::tiles[xy.x][xy.y]);
                            ui::cursor = xy;
                        } else {
                            xy = ui::cursor;
                        }
                        return; 
                    }
                } else {
                    arena::tiles[ui::cursor.x][ui::cursor.y].move_wizard_to(arena::tiles[xy.x][xy.y]);
                    arena::draw();
                    audio::play_move();
                    ui::cursor = xy;
                    if(check_for_engaged(wizard, xy, true)) {
                        attack_turn(wizard);
                        return;
                    }
                    movement -= 1;
                    if(movement == 0)
                        return;
                }
            }
            xy = ui::cursor;
        }
    }

    void turn(const std::shared_ptr<Wizard> wizard) {
        auto xy = ui::cursor;
        audio::play_start_move();
        if(check_for_engaged(wizard, xy, false)) {
            attack_turn(wizard);
            return;
        }
        if(wizard->magic_wings)
            fly_turn(wizard);
        else
            walk_turn(wizard);
        if(wizard->magic_bow)
            ranged_attack_turn(wizard);
    }

    void turn(const std::shared_ptr<Creation> creation) {
        auto xy = ui::cursor;
        audio::play_start_move();
        if(check_for_engaged(creation, xy, true)) {
            attack_turn(creation);
            return;
        }
        if(creation->flying)
            fly_turn(creation);
        else
            walk_turn(creation);
        if(creation->range)
            ranged_attack_turn(creation);
    }

    void movement_turn(const std::shared_ptr<Wizard>& wizard) {
        arena::draw();
        arena::display_tile_over_player_units(wizard);
        gfx::draw_turn_text(wizard->name);
        event::wait_for_key(50);
        arena::set_turn_flags(wizard);
        ui::cursor = arena::get_wizard_coords(wizard);
        while(true) {
            auto option = ui::select_tile_for_movement();
            Tile& tile = arena::tiles[ui::cursor.x][ui::cursor.y];
            if(option == CursorOption::ok)
                break;
            if(option == CursorOption::select) {
                if(tile.creation && tile.creation->turn) {
                    if(tile.wizard) {
                        gfx::unmount_wizard();
                        if(event::get_yes_or_no()) {
                            tile.creation->turn = false;
                            turn(wizard);
                        } else {
                            turn(tile.creation);
                        }
                    } else {
                        turn(tile.creation);
                    }
                } else if(tile.wizard && tile.wizard->turn) {
                    turn(wizard);
                }
            }
        }
        audio::play_turn_over();
        arena::clear_turn_flags(wizard);
    }

    bool check_for_winning_condition() {
        int surviving_wizards = 0;
        for(auto& turn:turns)
            if(turn.alive)
                surviving_wizards += 1;
        return surviving_wizards == 1;
    }

    void add_player(const Player& player) {
        Turn turn(player);
        turns.push_back(turn);
    }

    void start_game(const Server& server) {
        // Network stuff
    }

    void start_game() {
        for(auto& turn:turns)
            arena::add(turn.wizard);
        int number_of_rounds = turns.size() * 2 + 15;
        for(int i = 0; i < number_of_rounds; ++i) {
            for(auto& turn:turns)
                if(turn.alive)
                    turn.chosen_spell = ui::menu(turn.wizard);
            for(auto& turn:turns)
                if(turn.alive && turn.chosen_spell)
                    spell_turn(turn.wizard, turn.chosen_spell);
            for(auto& turn:turns)
                if(turn.alive)
                    movement_turn(turn.wizard);
            if(check_for_winning_condition())
                break;
        }
        std::vector<std::string> wizard_names;
        for(auto& turn:turns)
            if(turn.alive)
                wizard_names.push_back(turn.wizard->name);
        gfx::results(wizard_names);
        arena::clear();
        turns.erase(turns.begin(), turns.end());
    }
}
