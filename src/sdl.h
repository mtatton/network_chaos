#pragma once
#include <SDL2/SDL.h>
#include <vector>
#include <bitset>
#include "rgb.h"

#define WINDOW_WIDTH 256 * 3
#define WINDOW_HEIGHT 192 * 3

struct Texture {
    Texture(const int &width, const int &height);
    ~Texture();

    int width = 0;
    int height = 0;
    SDL_Texture* texture;
protected:
    std::vector<unsigned char> pixels;

    void bytes_to_texture_text(const std::string &bytes, const RGB &fg, const RGB &bg);
    void bytes_to_texture_gfx(const std::string &bytes, const RGB &fg, const RGB &bg);
    void bitmask_to_texture(const std::vector<std::vector<bool>> &bit_mask, const RGB &rgb);
    void point(const int& x, const int& y, const RGB &rgb);
    void update();
    std::vector<std::bitset<8>> get_bits_8(const std::string &bytes);
    std::vector<std::bitset<16>> get_bits_16(const std::string &bytes);
};

struct SDL_Window {
    SDL_Window();
    ~SDL_Window();
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* buffer;
};

namespace sdl {
    extern SDL_Window window;

    void clear();
    void present();
    void refresh();
    void draw(const Texture &texture, const int& x, const int& y, const int &sx, const int &sy, const int &width, const int &height);
    void draw(const Texture &texture, const int& x, const int& y);
    void clear(const int& x, const int& y, const int& width, const int& height);
}
