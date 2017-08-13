#pragma once
#include <SDL2/SDL.h>

#define WINDOW_WIDTH 256 * 3
#define WINDOW_HEIGHT 192 * 3

struct SDL_Window {
    SDL_Window();
    ~SDL_Window();
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* buffer;
};

extern SDL_Window window;

namespace sdl {
    void clear();
    void refresh();
}
