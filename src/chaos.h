#ifndef CHAOS_H_16DD0A0E
#define CHAOS_H_16DD0A0E

#include <SDL2/SDL.h>

#define WINDOW_WIDTH 256 * 3
#define WINDOW_HEIGHT 192 * 3

struct SDL_Window {
    SDL_Window() {
        SDL_Init(SDL_INIT_VIDEO);
        window = SDL_CreateWindow("Open Chaos: The Battle of Wizards", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_INPUT_FOCUS);
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE);
        buffer = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, WINDOW_WIDTH, WINDOW_HEIGHT);

    }
    ~SDL_Window() {
        SDL_DestroyTexture(buffer);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* buffer;
};

extern SDL_Window window;

#endif /* end of include guard: CHAOS_H_16DD0A0E */
