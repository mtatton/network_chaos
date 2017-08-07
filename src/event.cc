#include "event.h"

SDL_Event sdl_event;

namespace event {
    const Uint8 *keys;

    bool loop() {
        while(SDL_PollEvent(&sdl_event)) {
            switch(sdl_event.type) {
            case SDL_QUIT:
                return false;
                break;
            }
        }
        keys = SDL_GetKeyboardState(NULL);
        if(keys[SDL_SCANCODE_ESCAPE]) {
            return false;
        }
        return true;
    }
}
