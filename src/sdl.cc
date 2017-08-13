#include "sdl.h"

SDL_Window::SDL_Window() {
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Chaos: The Battle of Wizards", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_INPUT_FOCUS);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE);
    buffer = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, WINDOW_WIDTH, WINDOW_HEIGHT);
}

SDL_Window::~SDL_Window() {
    SDL_DestroyTexture(buffer);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

namespace sdl {
    void clear() {
        SDL_SetRenderTarget(window.renderer, window.buffer);
        SDL_RenderClear(window.renderer);
    }

    void refresh() {
        SDL_SetRenderTarget(window.renderer, NULL);
        SDL_RenderClear(window.renderer);
        SDL_RenderCopy(window.renderer, window.buffer, NULL, NULL);
        SDL_RenderPresent(window.renderer);
    }
}
