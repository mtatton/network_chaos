#include "audio.h"
#include "event.h"
#include <SDL2/SDL_mixer.h>
#include <memory>
#include "samples/key_wav.h"
#include "samples/menu_wav.h"
#include "samples/spell_beam_wav.h"
#include "samples/spell_end_wav.h"
#include "samples/spell_text_wav.h"
#include "samples/turn_over_wav.h"
#include "samples/cancel_wav.h"
#include "samples/start_move_wav.h"
#include "samples/move_wav.h"
#include "samples/landing_wav.h"
#include "samples/engaged_to_enemy_wav.h"
#include "samples/attack_wav.h"
#include "samples/wizard_death_wav.h"
#include "samples/ranged_attack_wav.h"
#include "samples/shot_wav.h"
#include "samples/hit_wav.h"
#include "samples/burn_beam_wav.h"
#include "samples/burn_wav.h"
#include "samples/explosion_wav.h"
#include "samples/undead_wav.h"

class Audio {
public:
    Audio(unsigned char * bytes, const int& length) {
        music = Mix_LoadMUS_RW(SDL_RWFromMem(bytes, length), 0);
    }

    ~Audio() {
        Mix_FreeMusic(music);
    }

    void play() {
        Mix_PlayMusic(music, 0);
    }

    void play_loop() {
        Mix_PlayMusic(music, -1);
    }

private:
    Mix_Music * music;
};

namespace audio {
    std::unique_ptr<Audio> key;
    std::unique_ptr<Audio> menu;
    std::unique_ptr<Audio> spell_beam;
    std::unique_ptr<Audio> spell_end;
    std::unique_ptr<Audio> spell_text;
    std::unique_ptr<Audio> turn_over;
    std::unique_ptr<Audio> cancel;
    std::unique_ptr<Audio> start_move;
    std::unique_ptr<Audio> move;
    std::unique_ptr<Audio> landing;
    std::unique_ptr<Audio> engaged_to_enemy;
    std::unique_ptr<Audio> attack;
    std::unique_ptr<Audio> wizard_death;
    std::unique_ptr<Audio> ranged_attack;
    std::unique_ptr<Audio> shot;
    std::unique_ptr<Audio> hit;
    std::unique_ptr<Audio> burn_beam;
    std::unique_ptr<Audio> burn;
    std::unique_ptr<Audio> explosion;
    std::unique_ptr<Audio> undead;

    void play(std::unique_ptr<Audio>& audio, unsigned char * bytes, const int& length) {
        if(!audio)
            audio = std::unique_ptr<Audio>(new Audio(bytes, length));
         audio->play();
    }

    void play_loop(std::unique_ptr<Audio>& audio, unsigned char * bytes, const int& length) {
        if(!audio)
            audio = std::unique_ptr<Audio>(new Audio(bytes, length));
         audio->play_loop();
    }

    void play_key() {
        play(key, key_wav, key_wav_len);
    }

    void play_menu() {
        play(menu, menu_wav, menu_wav_len);
    }

    void play_spell_beam() {
        play_loop(spell_beam, spell_beam_wav, spell_beam_wav_len);
    }

    void play_spell_end() {
        Mix_HaltMusic();
        play(spell_end, spell_end_wav, spell_end_wav_len);
    }

    void play_spell_text() {
        play(spell_text, spell_text_wav, spell_text_wav_len);
    }

    void play_turn_over() {
        play(turn_over, turn_over_wav, turn_over_wav_len);
        event::delay(50);
    }

    void play_cancel() {
        play(cancel, cancel_wav, cancel_wav_len);
        event::delay(15);
    }

    void play_start_move() {
        play(start_move, start_move_wav, start_move_wav_len);
        event::delay(20);
    }

    void play_move() {
        play(move, move_wav, move_wav_len);
    }

    void play_landing() {
        play(landing, landing_wav, landing_wav_len);
        event::delay(13);
    }

    void play_engaged_to_enemy() {
        play(engaged_to_enemy, engaged_to_enemy_wav, engaged_to_enemy_wav_len);
        event::delay(40);
    }

    void play_attack() {
        play(attack, attack_wav, attack_wav_len);
    }

    void play_wizard_death() {
        play(wizard_death, wizard_death_wav, wizard_death_wav_len);
    }

    void play_ranged_attack() {
        play(ranged_attack, ranged_attack_wav, ranged_attack_wav_len);
        event::delay(75);
    }

    void play_shot() {
        play(shot, shot_wav, shot_wav_len);
    }

    void play_hit() {
        Mix_HaltMusic();
        play(hit, hit_wav, hit_wav_len);
    }

    void play_burn_beam() {
        play(burn_beam, burn_beam_wav, burn_beam_wav_len);
    }

    void play_burn() {
        Mix_HaltMusic();
        play(burn, burn_wav, burn_wav_len);
    }

    void play_explosion() {
        play(explosion, explosion_wav, explosion_wav_len);
    }

    void play_undead() {
        play(undead, undead_wav, undead_wav_len);
        event::delay(50);
    }
}
