#include "chaos.h"
#include <SDL2/SDL.h>
#include "gfx.h"
#include "event.h"
#include "wizard.h"
#include <iostream>

void examine_spells(const Wizard& wizard) {
    gfx::draw_spellbook(wizard);
    while(true) {
        if(event::loop()) {
            if(event::keys[SDL_SCANCODE_0])
                break;
            int num = event::get_alphabet();
            if(num && (num <= wizard.number_of_spells)) {
                gfx::draw_spell(*wizard.spellbook[num - 1]);
                while(!event::loop())
                    sdl::refresh();
                gfx::draw_spellbook(wizard);
            }
        }
        sdl::refresh();
    }
}

const Spell * select_spell(const Wizard& wizard) {
    gfx::draw_spellbook(wizard);
    while(true) {
        if(event::loop()) {
            if(event::keys[SDL_SCANCODE_0])
                break;
            int num = event::get_alphabet();
            if(num && (num <= wizard.number_of_spells))
                return wizard.spellbook[num - 1];
        }
        sdl::refresh();
    }
    return nullptr;
}

void main_menu(Wizard& wizard) {
    const Spell *selected_spell;
    gfx::draw_main_menu(wizard.name);
    while(true) {
        if(event::loop()) {
            if(event::keys[SDL_SCANCODE_1]) {
                examine_spells(wizard);
                gfx::draw_main_menu(wizard.name);
            }
            if(event::keys[SDL_SCANCODE_2]) {
                selected_spell = select_spell(wizard);
                if(selected_spell) {
                    if(selected_spell->type != Spell::disbelieve) {
                        auto spell = std::find(wizard.spellbook.begin(), wizard.spellbook.end(), selected_spell);
                        wizard.spellbook.erase(spell);
                        wizard.number_of_spells -= 1;
                    }
                }
                gfx::draw_main_menu(wizard.name);
            }
            if(event::keys[SDL_SCANCODE_3]) {
                // Examine board
            }
            if(event::keys[SDL_SCANCODE_4]) {
                // Continue with game
            }
        }
        sdl::refresh();
    }
}

int main(int argc, char const *argv[]) {
    auto wizard = wizard::generate("Andy", 0, 0, bright_red);
    main_menu(wizard);
    return 0;
}