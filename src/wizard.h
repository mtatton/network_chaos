#pragma once
#include "rgb.h"
#include "coords.h"
#include <cstdlib>
#include <string>
#include <memory>

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
    bool turn = false;
    bool disengage(const Unit& opponent) const;
    bool within_ranged_combat_range(const Coords& sxy, const Coords& dxy) const;
};

struct Spell {
    std::string name;
    int id = 0;
    int cast_chance = 0;
    int doubled_cast_range = 0;
    int alignment = 0;
    int cast_priority = 0;
    enum SpellType {
        disbelieve, creation, gooey_blob, magic_fire, shelter, wall, elemental, magic, magic_shield, magic_armour, magic_sword, magic_knife, magic_bow, magic_wings, change_alignment, shadow_form, subversion, raise_dead, turmoil
    } type;

    int world_cast_chance() const;
    bool cast() const;
    bool within_range(const Coords& sxy, const Coords& dxy) const;
};

typedef std::vector<Spell> Spellbook;

struct Wizard : public Unit {
    Wizard(const std::string& name, const int& ability, const int& sprite_index, const RGB& rgb);
    Wizard(const std::string& name, const int& combat, const int& defence, const int& manoeuvre, const int& magical_resistance, const int& chaos_law_ability, const int& number_of_spells, const int& ability, const int& sprite_index, const RGB& rgb);
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
    bool magic_bow = false;
    bool shadow_form = false;
    enum class LastChange {
        none, magic_sword, magic_knife, magic_armour, magic_shield, magic_wings, magic_bow
    } last_change = LastChange::none;
    bool obtain_random_spell();
    void gain_magic_sword();
    void gain_magic_knife();
    void gain_magic_armour();
    void gain_magic_shield();
    void gain_magic_wings();
    void gain_magic_bow();
    void gain_shadow_form();
    bool within_flying_range(const Coords& sxy, const Coords& dxy) const;
};

struct Creation : public Unit {
    Creation(const std::string& name, const int& id, const int& combat, const int& ranged_combat, const int& range, const int& defence, const int& movement, const int& manoeuvre, const int& magical_resistance, const int& casting_chance, const int& alignment, const int& anim_timing, const bool& mount, const bool& flying, const bool& undead, const bool& transparent, const bool& subvertable, const bool& shelter);
    int id = 0;
    int casting_chance = 0;
    int alignment = 0;
    bool mount = false;
    bool flying = false;
    bool undead = false;
    bool transparent = false;
    bool subvertable = false;
    bool shelter = false;
    bool illusion = false;
    std::shared_ptr<Wizard> owner;
    bool within_flying_range(const Coords& sxy, const Coords& dxy) const;
    bool subverted() const;
};

namespace wizard {
    Creation generate_creation_from_id(const int& creation_id);
    bool attack(const Creation& attacker, const Creation& defender);
    bool attack(const Creation& attacker, const Wizard& defender);
    bool attack(const Wizard& attacker, const Creation& defender);
    bool attack(const Wizard& attacker, const Wizard& defender);
    bool ranged_attack(const Creation& attacker, const Creation& defender);
    bool ranged_attack(const Creation& attacker, const Wizard& defender);
    bool ranged_attack(const Wizard& attacker, const Creation& defender);
    bool ranged_attack(const Wizard& attacker, const Wizard& defender);
}

extern const std::vector<RGB> wizard_rgbs;
