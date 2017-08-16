#include "ui.h"
#include "arena.h"
#include "audio.h"

ChosenSpell::operator bool() const {
    return spell_index != -1;
}

namespace ui {
    Coords cursor;

    int select_spell_index(const std::shared_ptr<Wizard>& wizard) {
        audio::play_menu();
        gfx::draw_spellbook(*wizard);
        while(true) {
            int num = event::get_alphabet();
            if(num < wizard->number_of_spells)
                return num;
        }
    }

    void examine_spells(const std::shared_ptr<Wizard>& wizard) {
        while(true) {
            gfx::draw_spellbook(*wizard);
            int num = select_spell_index(wizard);
            if(num == -1)
                return;
            audio::play_menu();
            gfx::draw_spell(wizard->spellbook[num]);
            event::wait_for_key();
        }
    }

    void examine_tile(const Tile& tile) {
        if(tile.creation) {
            audio::play_menu();
            if(tile.wizard)
                gfx::draw_stats(*tile.creation, tile.wizard->name);
            else
                gfx::draw_stats(*tile.creation);
            event::wait_for_key();
        }
        if(tile.wizard) {
            audio::play_menu();
            gfx::draw_stats(*tile.wizard);
            event::wait_for_key();
        }
        if(tile.corpse && !tile.creation && !tile.wizard) {
            audio::play_menu();
            gfx::draw_stats(*tile.corpse);
            event::wait_for_key();
        }
    }

    void examine_board(Coords& cursor) {
        while(true) {
            switch(event::cursor_movement(cursor)) {
            case CursorOption::info:
                examine_tile(arena::tiles[cursor.x][cursor.y]);
                break;
            case CursorOption::ok:
                return;
            default:
                arena::draw();
                gfx::draw_spell_cursor(bright_cyan, cursor);
                gfx::draw_info_text(arena::tiles[cursor.x][cursor.y]);
            }
        }
    }

    void loading_screen() {
        gfx::draw_loading_screen();
        event::wait_for_key(300);
    }

    Server title_screen() {
        Server server;
        gfx::draw_title_screen();
        server.host = event::get_yes_or_no(bright_white, black, {13, 9});
        if(!server.host) {
            event::delay(10);
            gfx::draw_enter_server();
            server.address = event::text_input_server(bright_white, {2, 16});
        }
        event::delay(10);
        return server;
    }

    Player player_entry() {
        Player player;
        gfx::draw_player_entry_screen();
        player.name = event::text_input_name(bright_cyan, {2, 6});
        gfx::draw_computer_controlled_question();
        gfx::draw_which_character_question();
        player.sprite_index = event::number_input(1, 8, bright_white, black, {19, 11});
        player.sprite_index -= 1;
        gfx::draw_wizard_xy(player.sprite_index, bright_white, {20 * 8, 11 * 8});
        event::delay(10);
        gfx::draw_which_colour_question(player.sprite_index);
        player.rgb_index = event::number_input(1, 8, bright_white, black, {16, 16});
        player.rgb_index -= 1;
        gfx::draw_wizard_xy(player.sprite_index, wizard_rgbs[player.rgb_index], {17 * 8, 16 * 8});
        event::delay(10);
        return player;
    }

    ChosenSpell menu(const std::shared_ptr<Wizard>& wizard) {
        ChosenSpell chosen_spell;
        while(true) {
            audio::play_menu();
            gfx::draw_main_menu(wizard->name, arena::world_alignment);
            switch(event::menu_option()) {
            case 1:
                examine_spells(wizard);
                break;
            case 2:
                chosen_spell.spell_index = select_spell_index(wizard);
                if(chosen_spell.spell_index != -1) {
                    if(wizard->spellbook[chosen_spell.spell_index].type == Spell::creation) {
                        gfx::draw_illusion_question();
                        chosen_spell.illusion = event::get_yes_or_no();
                    }
                }
                break;
            case 3:
                examine_board(cursor);
                break;
            case 4:
                return chosen_spell;
            }
        }
    }

    CursorOption select_tile_for_spell() {
        CursorOption option;
        while(true) {
            option = event::cursor_movement(cursor);
            switch(option) {
            case CursorOption::info:
                examine_tile(arena::tiles[cursor.x][cursor.y]);
                break;
            case CursorOption::select:
                return option;
            case CursorOption::cancel:
                audio::play_cancel();
                event::delay(5);
                return option;
            default:
                arena::draw();
                gfx::draw_spell_cursor(bright_cyan, cursor);
                gfx::draw_info_text(arena::tiles[cursor.x][cursor.y]);
            }
        }
    }

    CursorOption select_tile_for_movement() {
        CursorOption option;
        while(true) {
            option = event::cursor_movement(cursor);
            switch(option) {
            case CursorOption::info:
                examine_tile(arena::tiles[cursor.x][cursor.y]);
                break;
            case CursorOption::select:
            case CursorOption::ok:
                return option;
            default:
                arena::draw();
                gfx::draw_box_cursor(bright_cyan, cursor);
                gfx::draw_info_text(arena::tiles[cursor.x][cursor.y]);
            }
        }
    }
}
