#include "audio.h"
#include <SDL2/SDL_mixer.h>
#include <memory>
#include "samples/key_wav.h"
#include "samples/menu_wav.h"
#include "samples/spell_beam_wav.h"
#include "samples/spell_end_wav.h"
#include "samples/spell_text_wav.h"
#include "samples/turn_over_wav.h"
#include "samples/cancel_wav.h"

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
    }

    void play_cancel() {
        play(cancel, cancel_wav, cancel_wav_len);
    }
}
