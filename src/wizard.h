#pragma once
#include <string>
#include <vector>
#include "rgb.h"

struct Unit {
    std::string name;
    int combat = 0;
    int ranged_combat = 0;
    int range = 0;
    int defence = 0;
    int movement = 0;
    int manoeuvre = 0;
    int magical_resistance = 0;
    int anim_timing = 0;
};

struct Creation : public Unit {
    int id = 0;
    int casting_chance = 0;
    int alignment = 0;
    bool mount = false;
    bool flying = false;
    bool undead = false;
    bool transparent = false;
    bool subvertable = false;
    bool shelter = false;
};

struct Spell {
    const std::string name;
    const int id;
    const int cast_chance;
    const int doubled_cast_range;
    const int alignment;
    const int cast_priority;
    enum {
        disbelieve,
        creation,
        shelter,
        wall,
        elemental,
        magic,
        magic_shield,
        magic_armour,
        magic_sword,
        magic_knife,
        magic_bow,
        magic_wings,
        change_alignment,
        shadow_form,
        subversion,
        raise_dead,
        turmoil
    } type;

    int world_cast_chance() const;
};

typedef std::vector<const Spell *> Spellbook;

struct Wizard : public Unit {
    int chaos_law_ability = 0;
    int number_of_spells = 0;
    int ability = 0;
    Spellbook spellbook;
    int sprite_index = 0;
    RGB rgb;
    bool magic_sword = false;
    bool magic_knife = false;
    bool magic_armour = false;
    bool magic_shield = false;
    bool magic_wings = false;
    bool shadow_form = false;
};

namespace wizard {
    Wizard generate(const std::string& wizard_name, const int& level, const int& sprite_index, const RGB& rgb);
    Wizard generate_wizard_from_stats(const std::string& wizard_name, const int& combat, const int& defence, const int& manoeuvre, const int& magical_resistance, const int& chaos_law_ability, const int& number_of_spells, const int& ability, const int& sprite_index, const RGB& rgb);
    const Spell * obtain_random_spell();
    Creation generate_creation_from_id(const int& creation_id);
    bool attack(const Creation& attacker, const Creation& defender);
    bool attack(const Creation& attacker, const Wizard& defender);
    bool attack(const Wizard& attacker, const Creation& defender);
    bool attack(const Wizard& attacker, const Wizard& defender);
}
