#include "logic.h"
#include "event.h"
#include "arena.h"
#include "audio.h"
#include <iostream>

struct Turn {
    std::shared_ptr<Wizard> wizard;
    ChosenSpell chosen_spell;
    bool alive = true;

    Turn(const Player& player) {
        wizard = std::shared_ptr<Wizard>(new Wizard(player.name, 0, player.sprite_index, wizard_rgbs[player.rgb_index]));
    }
};

namespace logic {
    std::vector<Turn> turns;

    void disbelieve(const std::shared_ptr<Wizard>& wizard) {
        auto wizard_coords = arena::get_wizard_coords(wizard);
        ui::cursor = wizard_coords;
        while(true) {
            auto option = ui::select_tile_for_spell();
            switch(option) {
            case CursorOption::select:
                gfx::spell_beam_animation(wizard_coords, ui::cursor);
                event::delay(15);
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
            auto& tile = arena::tiles[ui::cursor.x][ui::cursor.y];
            switch(option) {
            case CursorOption::select:
                if(tile.wizard || tile.creation)
                    break;
                if(!spell.within_range(wizard_coords, ui::cursor)) {
                    gfx::draw_out_of_range(bright_cyan);
                    event::delay(15);
                    break;
                }
                gfx::spell_beam_animation(wizard_coords, ui::cursor);
                creation = std::make_shared<Creation>(wizard::generate_creation_from_id(spell.id));
                arena::add(creation, wizard, illusion, ui::cursor);
                arena::draw();
                event::delay(15);
                return;
            case CursorOption::cancel:
                return;
            default:
                break;
            }
        }
    }

    void spell_turn(const std::shared_ptr<Wizard>& wizard, const ChosenSpell& chosen_spell) {
        if(chosen_spell.spell_index != -1) {
            Spell spell = wizard->spellbook[chosen_spell.spell_index];
            arena::draw();
            gfx::cast_spell_text(*wizard, spell);
            switch(spell.type) {
            case Spell::disbelieve:
                disbelieve(wizard);
                break;
            case Spell::creation:
                creation_spell(wizard, spell, chosen_spell.illusion);
                break;
            default:
                break;
            }
        }
    }

    void movement_turn(const std::shared_ptr<Wizard>& wizard) {
        arena::draw();
        gfx::draw_turn_text(wizard->name);
        event::wait_for_key(50);
        arena::set_turn_flags(wizard);
        ui::cursor = arena::get_wizard_coords(wizard);
        while(true) {
            auto option = ui::select_tile_for_movement();
            auto& tile = arena::tiles[ui::cursor.x][ui::cursor.y];
            if(option == CursorOption::ok)
                break;
            if(option == CursorOption::select) {
                std::cout << ui::cursor.x << ", " << ui::cursor.y << std::endl;
            }
        }
        audio::play_turn_over();
        event::delay(30);
        arena::clear_turn_flags(wizard);
    }

    bool check_for_winning_condition() {
        int surviving_wizards = 0;
        for(auto& turn:turns)
            if(turn.alive)
                surviving_wizards += 1;
        return surviving_wizards == 1;
    }

    void start_game(const Server& server, const Player& player) {
        turns.push_back(Turn({"Fred", 0, 0}));
        turns.push_back(Turn({"Joe", 1, 1}));
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
    }
}
