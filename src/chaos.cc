#include "logic.h"

int main(int argc, char const *argv[]) {
    // ui::loading_screen();
    // Server server = ui::title_screen();
    // Player player = ui::player_entry();
    Server server;
    Player player;
    logic::start_game(server, player);
    return 0;
}