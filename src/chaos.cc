#include "wizard.h"
#include "ui.h"
#include "event.h"
#include "arena.h"

int main(int argc, char const *argv[]) {
    ui::loading_screen();
    Server server = ui::title_screen();
    Player player = ui::player_entry();
    auto fred = std::shared_ptr<Wizard>(new Wizard("Fred", 0, 0, bright_red));
    auto joe = std::shared_ptr<Wizard>(new Wizard("Joe", 0, 0, bright_green));
    auto golden_dragon = std::make_shared<Creation>(wizard::generate_creation_from_id(25));
    arena::add(fred);
    arena::add(joe);
    arena::add(golden_dragon, fred, 2, 4);
    arena::kill(2, 4);
    ui::menu(*fred);
    return 0;
}