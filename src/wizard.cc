#include "wizard.h"
#include "arena.h"
#include <random>

const std::vector<RGB> wizard_rgbs = {bright_red, bright_purple, bright_green, bright_cyan, yellow, bright_yellow, white, bright_white};

int Spell::world_cast_chance() const {
    int world_alignment = arena::world_alignment;
    if ((world_alignment < 0 && alignment < 0) || (world_alignment > 0 && alignment > 0))
        return std::min(int(std::floor(std::abs(world_alignment) / 4) + cast_chance), 9);
    return cast_chance;
}

bool Spell::within_range(const Coords& sxy, const Coords& dxy) const {
    auto diff_xy = sxy.compute_diff_with(dxy);
    int doubled_max = std::max(diff_xy.x, diff_xy.y) * 2;
    return doubled_max < doubled_cast_range;
}

const Spell disbelieve = {"DISBELIEVE", 93, 9, 255, 0, 0, Spell::disbelieve};
const Spell king_cobra = {"KING COBRA", 2, 8, 3, 1, 18, Spell::creation};
const Spell dire_wolf = {"DIRE WOLF", 3, 8, 3, -1, 18, Spell::creation};
const Spell goblin = {"GOBLIN", 4, 8, 3, -1, 18, Spell::creation};
const Spell crocodile = {"CROCODILE", 5, 7, 3, 0, 18, Spell::creation};
const Spell faun = {"FAUN", 6, 7, 3, -1, 18, Spell::creation};
const Spell lion = {"LION", 7, 5, 3, 1, 18, Spell::creation};
const Spell elf = {"ELF", 8, 6, 3, 2, 18, Spell::creation};
const Spell orc = {"ORC", 9, 9, 3, -1, 18, Spell::creation};
const Spell bear = {"BEAR", 10, 5, 3, 1, 18, Spell::creation};
const Spell gorilla = {"GORILLA", 11, 6, 3, 0, 18, Spell::creation};
const Spell ogre = {"OGRE", 12, 6, 3, -1, 18, Spell::creation};
const Spell hydra = {"HYDRA", 13, 4, 3, -1, 18, Spell::creation};
const Spell giant_rat = {"GIANT RAT", 14, 9, 3, 0, 18, Spell::creation};
const Spell giant = {"GIANT", 15, 3, 3, 1, 18, Spell::creation};
const Spell horse = {"HORSE", 16, 8, 3, 1, 18, Spell::creation};
const Spell unicorn = {"UNICORN", 17, 5, 3, 2, 18, Spell::creation};
const Spell centaur = {"CENTAUR", 18, 6, 3, 1, 18, Spell::creation};
const Spell pegasus = {"PEGASUS", 19, 5, 3, 2, 18, Spell::creation};
const Spell gryphon = {"GRYPHON", 20, 5, 3, 1, 18, Spell::creation};
const Spell manticore = {"MANTICORE", 21, 3, 3, -1, 18, Spell::creation};
const Spell bat = {"BAT", 22, 7, 3, -1, 18, Spell::creation};
const Spell green_dragon = {"GREEN DRAGON", 23, 0, 3, -1, 18, Spell::creation};
const Spell red_dragon = {"RED DRAGON", 24, 0, 3, -1, 18, Spell::creation};
const Spell golden_dragon = {"GOLDEN DRAGON", 25, 0, 3, 2, 18, Spell::creation};
const Spell harpy = {"HARPY", 26, 5, 3, -1, 18, Spell::creation};
const Spell eagle = {"EAGLE", 27, 6, 3, 1, 18, Spell::creation};
const Spell vampire = {"VAMPIRE", 28, 1, 3, -2, 18, Spell::creation};
const Spell ghost = {"GHOST", 29, 4, 3, -1, 18, Spell::creation};
const Spell spectre = {"SPECTRE", 30, 5, 3, -1, 18, Spell::creation};
const Spell wraith = {"WRAITH", 31, 4, 3, -1, 18, Spell::creation};
const Spell skeleton = {"SKELETON", 32, 6, 3, -1, 18, Spell::creation};
const Spell zombie = {"ZOMBIE", 33, 8, 3, -1, 18, Spell::creation};
const Spell gooey_blob = {"GOOEY BLOB", 97, 8, 13, -1, 18, Spell::gooey_blob};
const Spell magic_fire = {"MAGIC FIRE", 96, 7, 13, -1, 18, Spell::magic_fire};
const Spell magic_wood = {"MAGIC WOOD", 36, 7, 17, 1, 17, Spell::shelter};
const Spell shadow_wood = {"SHADOW WOOD", 37, 3, 17, -1, 18, Spell::shelter};
const Spell magic_castle = {"MAGIC CASTLE", 38, 4, 17, 1, 23, Spell::shelter};
const Spell dark_citadel = {"DARK CITADEL", 39, 4, 17, -1, 23, Spell::shelter};
const Spell wall = {"WALL", 40, 7, 13, 0, 12, Spell::wall};
const Spell magic_bolt = {"MAGIC BOLT", 112, 9, 13, 0, 18, Spell::elemental};
const Spell lightning = {"LIGHTNING", 98, 9, 9, 0, 18, Spell::elemental};
const Spell vengeance = {"VENGEANCE", 99, 7, 30, -1, 12, Spell::magic};
const Spell decree = {"DECREE", 101, 7, 30,  12, Spell::magic};
const Spell dark_power = {"DARK POWER", 100, 4, 30, -2, 11, Spell::magic};
const Spell justice = {"JUSTICE", 102, 4, 30, 2, 11, Spell::magic};
const Spell magic_shield = {"MAGIC SHIELD", 107, 6, 0, 1, 23, Spell::magic_shield};
const Spell magic_armour = {"MAGIC ARMOUR", 108, 3, 0, 1, 23, Spell::magic_armour};
const Spell magic_sword = {"MAGIC SWORD", 109, 3, 0, 1, 16, Spell::magic_sword};
const Spell magic_knife = {"MAGIC KNIFE", 110, 6, 0, 1, 16, Spell::magic_knife};
const Spell magic_bow = {"MAGIC BOW", 111, 4, 0, 1, 17, Spell::magic_bow};
const Spell magic_wings = {"MAGIC WINGS", 123, 4, 0, 0, 14, Spell::magic_wings};
const Spell law_1 = {"LAW-1", 103, 7, 0, 2, 5, Spell::change_alignment};
const Spell law_2 = {"LAW-2", 104, 5, 0, 4, 6, Spell::change_alignment};
const Spell chaos_1 = {"CHAOS-1", 105, 7, 0, -2, 5, Spell::change_alignment};
const Spell chaos_2 = {"CHAOS-2", 106, 5, 0, -4, 6, Spell::change_alignment};
const Spell shadow_form = {"SHADOW FORM", 125, 6, 0, 0, 18, Spell::shadow_form};
const Spell subversion = {"SUBVERSION", 117, 9, 15, 0, 18, Spell::subversion};
const Spell raise_dead = {"RAISE DEAD", 115, 4, 9, -1, 18, Spell::raise_dead};
const Spell turmoil = {"TURMOIL", 118, 9, 20, -1, 17, Spell::turmoil};

// const Spellbook selection = {
//     disbelieve, king_cobra, dire_wolf, goblin, crocodile, faun, lion, elf, orc, bear, gorilla, ogre, hydra, giant_rat, giant, horse, unicorn, centaur, pegasus, gryphon, manticore, bat, green_dragon, red_dragon, golden_dragon, harpy, eagle, vampire, ghost, spectre, wraith, skeleton, zombie, gooey_blob, magic_fire, magic_wood, shadow_wood, magic_castle, dark_citadel, wall, magic_bolt, magic_bolt, lightning, lightning, vengeance, decree, dark_power, justice, magic_shield, magic_armour, magic_sword, magic_knife, magic_bow, magic_wings, law_1, law_2, chaos_1, chaos_2, shadow_form, subversion, subversion, raise_dead, raise_dead
// };

const Spellbook selection = {
    disbelieve, king_cobra, dire_wolf, goblin, crocodile, faun, lion, elf, orc, bear, gorilla, ogre, hydra, giant_rat, giant, horse, unicorn, centaur, pegasus, gryphon, manticore, bat, green_dragon, red_dragon, golden_dragon, harpy, eagle, vampire, ghost, spectre, wraith, skeleton, zombie
};

Creation::Creation(const std::string& name, const int& id, const int& combat, const int& ranged_combat, const int& range, const int& defence, const int& movement, const int& manoeuvre, const int& magical_resistance, const int& casting_chance, const int& alignment, const int& anim_timing, const bool& mount, const bool& flying, const bool& undead, const bool& transparent, const bool& subvertable, const bool& shelter) {
    this->name = name;
    this->id = id;
    this->combat = combat;
    this->ranged_combat = ranged_combat;
    this->range = range;
    this->defence = defence;
    this->movement = movement;
    this->manoeuvre = manoeuvre;
    this->magical_resistance = magical_resistance;
    this->casting_chance = casting_chance;
    this->alignment = alignment;
    this->anim_timing = anim_timing;
    this->mount = mount;
    this->flying = flying;
    this->undead = undead;
    this->transparent = transparent;
    this->subvertable = subvertable;
    this->shelter = shelter;
}

const int generate_random_int(const int& start, const int& end) {
    std::random_device generator;
    std::uniform_int_distribution<int> distribution(start, end);
    return distribution(generator);
}

Wizard::Wizard(const std::string& name, const int& ability, const int& sprite_index, const RGB& rgb) {
    this->name = name;
    combat = 1 + generate_random_int(0, 4) + floor(float(ability) / 2);
    ranged_combat = 0;
    range = 0;
    defence = 1 + generate_random_int(0, 4) + floor(float(ability) / 2);
    movement = 1;
    manoeuvre = 3 + generate_random_int(0, 4) + floor(float(ability) / 4);
    magical_resistance = 6 + generate_random_int(0, 2) + floor(float(ability) / 4);
    anim_timing = 30;
    if(generate_random_int(0, 9) >= 5 - floor(float(ability) / 2)) {
        chaos_law_ability = generate_random_int(0, 2);
    } else {
        chaos_law_ability = 0;
    }
    number_of_spells = std::min(20, 11 + generate_random_int(0, 4) + ability);
    spellbook.push_back(disbelieve);
    for(int i = 1; i < number_of_spells; ++i) {
        int spell_index = generate_random_int(1, selection.size() - 1);
        spellbook.push_back(selection[spell_index]);
    }
    this->ability = ability;
    this->sprite_index = sprite_index;
    this->rgb = rgb;
}

Wizard::Wizard(const std::string& name, const int& combat, const int& defence, const int& manoeuvre, const int& magical_resistance, const int& chaos_law_ability, const int& number_of_spells, const int& ability, const int& sprite_index, const RGB& rgb) {
    this->name = name;
    this->combat = combat;
    ranged_combat = 0;
    range = 0;
    this->defence = defence;
    movement = 1;
    this->manoeuvre = manoeuvre;
    this->magical_resistance = magical_resistance;
    anim_timing = 30;
    this->chaos_law_ability = chaos_law_ability;
    this->number_of_spells = number_of_spells;
    this->ability = ability;
    this->sprite_index = sprite_index;
    this->rgb = rgb;
}

bool Wizard::obtain_random_spell() {
    if(number_of_spells < 20) {
        int spell_index = generate_random_int(0, selection.size() - 1);
        if(spell_index == 0)
            spellbook.push_back(turmoil);
        else
            spellbook.push_back(selection[spell_index]);
        number_of_spells += 1;
        return true;
    }
    return false;
}

namespace wizard {
    Creation generate_creation_from_id(const int& creation_id) {
        switch(creation_id) {
        case 2: return Creation("KING COBRA", 2, 4, 0, 0, 1, 1, 6, 1, 8, 1, 30, false, false, false, false, true, false);
        case 3: return Creation("DIRE WOLF", 3, 3, 0, 0, 2, 3, 7, 2, 8, -1, 12, false, false, false, false, true, false);
        case 4: return Creation("GOBLIN", 4, 2, 0, 0, 4, 1, 4, 4, 8, -1, 12, false, false, false, false, true, false);
        case 5: return Creation("CROCODILE", 5, 5, 0, 0, 6, 1, 2, 2, 7, 0, 34, false, false, false, false, true, false);
        case 6: return Creation("FAUN", 6, 3, 0, 0, 2, 1, 7, 8, 7, -1, 20, false, false, false, false, true, false);
        case 7: return Creation("LION", 7, 6, 0, 0, 4, 4, 8, 3, 5, 1, 38, false, false, false, false, true, false);
        case 8: return Creation("ELF", 8, 1, 2, 6, 2, 1, 5, 7, 6, 2, 26, false, false, false, false, true, false);
        case 9: return Creation("ORC", 9, 2, 0, 0, 1, 1, 4, 4, 9, -1, 21, false, false, false, false, true, false);
        case 10: return Creation("BEAR", 10, 6, 0, 0, 7, 2, 6, 2, 5, 1, 23, false, false, false, false, true, false);
        case 11: return Creation("GORILLA", 11, 6, 0, 0, 5, 1, 4, 2, 6, 0, 18, false, false, false, false, true, false);
        case 12: return Creation("OGRE", 12, 4, 0, 0, 7, 1, 3, 6, 6, -1, 23, false, false, false, false, true, false);
        case 13: return Creation("HYDRA", 13, 7, 0, 0, 8, 1, 4, 6, 4, -1, 36, false, false, false, false, true, false);
        case 14: return Creation("GIANT RAT", 14, 1, 0, 0, 1, 3, 8, 2, 9, 0, 13, false, false, false, false, true, false);
        case 15: return Creation("GIANT", 15, 9, 0, 0, 7, 2, 6, 5, 3, 1, 23, false, false, false, false, true, false);
        case 16: return Creation("HORSE", 16, 1, 0, 0, 3, 4, 8, 1, 8, 1, 21, true, false, false, false, true, false);
        case 17: return Creation("UNICORN", 17, 5, 0, 0, 4, 4, 9, 7, 5, 2, 16, true, false, false, false, true, false);
        case 18: return Creation("CENTAUR", 18, 1, 2, 4, 3, 4, 5, 5, 6, 1, 23, true, false, false, false, true, false);
        case 19: return Creation("PEGASUS", 19, 2, 0, 0, 4, 5, 6, 7, 5, 2, 16, true, true, false, false, true, false);
        case 20: return Creation("GRYPHON", 20, 3, 0, 0, 5, 5, 5, 6, 5, 1, 10, true, true, false, false, true, false);
        case 21: return Creation("MANTICORE", 21, 3, 1, 3, 6, 5, 6, 8, 3, -1, 13, true, true, false, false, true, false);
        case 22: return Creation("BAT", 22, 1, 0, 0, 1, 5, 9, 4, 7, -1, 8, false, true, false, false, true, false);
        case 23: return Creation("GREEN DRAGON", 23, 5, 4, 6, 8, 3, 4, 4, 0, -1, 32, false, true, false, false, true, false);
        case 24: return Creation("RED DRAGON", 24, 7, 3, 5, 9, 3, 4, 5, 0, -2, 34, false, true, false, false, true, false);
        case 25: return Creation("GOLDEN DRAGON", 25, 9, 5, 4, 9, 3, 5, 5, 0, 2, 27, false, true, false, false, true, false);
        case 26: return Creation("HARPY", 26, 4, 0, 0, 2, 5, 8, 5, 5, -1, 13, false, true, false, false, true, false);
        case 27: return Creation("EAGLE", 27, 3, 0, 0, 3, 6, 8, 2, 6, 1, 14, false, true, false, false, true, false);
        case 28: return Creation("VAMPIRE", 28, 6, 0, 0, 8, 4, 6, 5, 1, -2, 40, false, true, true, false, true, false);
        case 29: return Creation("GHOST", 29, 1, 0, 0, 3, 2, 9, 6, 4, -1, 15, false, true, true, true, true, false);
        case 30: return Creation("SPECTRE", 30, 4, 0, 0, 2, 1, 6, 4, 5, -1, 15, false, false, true, false, true, false);
        case 31: return Creation("WRAITH", 31, 5, 0, 0, 5, 2, 4, 5, 4, -1, 10, false, false, true, true, true, false);
        case 32: return Creation("SKELETON", 32, 3, 0, 0, 2, 1, 3, 4, 6, -1, 17, false, false, true, false, true, false);
        case 33: return Creation("ZOMBIE", 33, 1, 0, 0, 1, 1, 2, 3, 8, -1, 25, false, false, true, false, true, false);
        case 34: return Creation("BLOB", 34, 0, 0, 0, 0, 0, 0, 0, 0, 0, 40, false, false, false, false, false, false);
        case 35: return Creation("FIRE", 35, 0, 0, 0, 0, 0, 0, 0, 0, 0, 12, false, false, false, true, false, false);
        case 36: return Creation("MAGIC WOOD", 36, 0, 0, 0, 5, 0, 0, 9, 6, 2, 250, false, false, false, false, false, true);
        case 37: return Creation("SHADOW WOOD", 37, 2, 0, 0, 4, 0, 0, 9, 4, -2, 30, false, false, false, false, false, true);
        case 38: return Creation("MAGIC CASTLE", 38, 0, 0, 0, 0, 0, 0, 0, 0, 0, 50, false, false, false, false, false, true);
        case 39: return Creation("DARK CITADEL", 39, 0, 0, 0, 0, 0, 0, 0, 0, 0, 50, false, false, false, false, false, true);
        case 40: return Creation("WALL", 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 30, false, false, false, false, false, false);
        }
        throw 0;
    }

    bool attack(const Creation& attacker, const Creation& defender) {
        return false;
    }

    bool attack(const Creation& attacker, const Wizard& defender) {
        return false;
    }

    bool attack(const Wizard& attacker, const Creation& defender) {
        return false;
    }

    bool attack(const Wizard& attacker, const Wizard& defender) {
        return false;
    }

    bool magic_bolt_attack(const Creation& defender) {
        return false;
    }

    bool magic_bolt_attack(const Wizard& defender) {
        return false;
    }

    bool lightning_attack(const Creation& defender) {
        return false;
    }

    bool lightning_attack(const Wizard& defender) {
        return false;
    }
}
