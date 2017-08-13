#include "ui.h"
#include "event.h"
#include "arena.h"
#include <iostream>

namespace ui {
    int cursor_x = 0;
    int cursor_y = 0;
    int selected_spell_index = 0;

    int select_spell_index(const Wizard& wizard) {
        while(true) {
            gfx::draw_spellbook(wizard);
            int num = event::get_alphabet();
            if(num <= wizard.number_of_spells)
                return num;
        }
    }

    void examine_spells(const Wizard& wizard) {
        while(true) {
            gfx::draw_spellbook(wizard);
            int num = select_spell_index(wizard);
            if(num == 0)
                return;
            num -= 1;
            gfx::draw_spell(wizard.spellbook[num]);
            event::wait_for_keydown();
        }
    }

    void examine_board() {
        while(!event::cursor_movement(cursor_x, cursor_y)) {
            if(event::keys[SDL_SCANCODE_I]) {
                auto& tile = arena::tiles[cursor_x][cursor_y];
                if(tile.creation) {
                    gfx::draw_stats(*tile.creation);
                    event::wait_for_keydown();
                }
                if(tile.wizard) {
                    gfx::draw_stats(*tile.wizard);
                    event::wait_for_keydown();
                }
                if(tile.corpse && !tile.creation && !tile.wizard) {
                    gfx::draw_stats(*tile.corpse);
                    event::wait_for_keydown();
                }
            }
            arena::draw();
            gfx::draw_spell_cursor(bright_cyan, cursor_x, cursor_y);
            gfx::draw_info_text(arena::tiles[cursor_x][cursor_y]);
        }
    }

    void loading_screen() {
        gfx::draw_loading_screen();
        event::wait_for_keydown(300);
    }

    Server title_screen() {
        Server server;
        gfx::draw_title_screen();
        server.host = event::get_yes_or_no(bright_white, black, 13, 9);
        if(!server.host) {
            SDL_Delay(500);
            gfx::draw_enter_server();
            server.address = event::text_input_server(bright_white, 2, 16);
        }
        SDL_Delay(500);
        return server;
    }

    Player player_entry() {
        Player player;
        gfx::draw_player_entry_screen();
        player.name = event::text_input_name(bright_cyan, 2, 6);
        gfx::draw_computer_controlled_question();
        gfx::draw_which_character_question();
        player.sprite_index = event::number_input(1, 8, bright_white, black, 19, 11);
        player.sprite_index -= 1;
        SDL_Delay(50);
        gfx::draw_wizard_xy(player.sprite_index, bright_white, 20 * 8, 11 * 8);
        sdl::refresh();
        SDL_Delay(500);
        gfx::draw_which_colour_question(player.sprite_index);
        player.colour_index = event::number_input(1, 8, bright_white, black, 16, 16);
        player.colour_index -= 1;
        SDL_Delay(50);
        gfx::draw_wizard_xy(player.sprite_index, wizard_rgbs[player.colour_index], 17 * 8, 16 * 8);
        sdl::refresh();
        SDL_Delay(500);
        return player;
    }

    void menu(Wizard& wizard) {
        while(true) {
            gfx::draw_main_menu(wizard.name);
            switch(event::menu_option()) {
            case 1:
                examine_spells(wizard);
                break;
            case 2:
                selected_spell_index = select_spell_index(wizard);
                selected_spell_index -= 1;
                if(selected_spell_index > 0) {
                    wizard.spellbook.erase(wizard.spellbook.begin() + selected_spell_index);
                    wizard.number_of_spells -= 1;
                }
                break;
            case 3:
                examine_board();
                break;
            case 4:
                return;
            }
        }
    }
}
