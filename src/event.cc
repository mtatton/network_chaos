#include "event.h"
#include <SDL2/SDL.h>

SDL_Event sdl_event;

namespace event {
    const Uint8 *keys;

    bool loop() {
        while(SDL_PollEvent(&sdl_event)) {
            switch(sdl_event.type) {
            case SDL_QUIT:
                exit(0);
            case SDL_KEYDOWN:
                keys = SDL_GetKeyboardState(NULL);
                return true;
            }
        }
        return false;
    }

    int get_alphabet() {
        if(event::keys[SDL_SCANCODE_A])
            return 1;
        if(event::keys[SDL_SCANCODE_B])
            return 2;
        if(event::keys[SDL_SCANCODE_C])
            return 3;
        if(event::keys[SDL_SCANCODE_D])
            return 4;
        if(event::keys[SDL_SCANCODE_E])
            return 5;
        if(event::keys[SDL_SCANCODE_F])
            return 6;
        if(event::keys[SDL_SCANCODE_G])
            return 7;
        if(event::keys[SDL_SCANCODE_H])
            return 8;
        if(event::keys[SDL_SCANCODE_I])
            return 9;
        if(event::keys[SDL_SCANCODE_J])
            return 10;
        if(event::keys[SDL_SCANCODE_K])
            return 11;
        if(event::keys[SDL_SCANCODE_L])
            return 12;
        if(event::keys[SDL_SCANCODE_M])
            return 13;
        if(event::keys[SDL_SCANCODE_N])
            return 14;
        if(event::keys[SDL_SCANCODE_O])
            return 15;
        if(event::keys[SDL_SCANCODE_P])
            return 16;
        if(event::keys[SDL_SCANCODE_Q])
            return 17;
        if(event::keys[SDL_SCANCODE_R])
            return 18;
        if(event::keys[SDL_SCANCODE_S])
            return 19;
        if(event::keys[SDL_SCANCODE_T])
            return 20;
        return 0;
    }
}
