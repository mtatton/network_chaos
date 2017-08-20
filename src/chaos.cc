#include "logic.h"

int main(int argc, char const *argv[]) {
    ui::loading_screen();
    int number_of_players = ui::title_screen();
    for(int i = 1; i <= number_of_players; ++i) {
        auto player = ui::player_entry(i);
        logic::add_player(player);
    }
    // logic::add_player({"Fred", 0, bright_red});
    // logic::add_player({"Joe", 1, bright_green});
    logic::start_game();
    return 0;
}
