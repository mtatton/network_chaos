#include "sdl.h"
#include <SDL2/SDL_mixer.h>

Texture::Texture(const int &width, const int &height) : pixels(width * height * 4) {
    texture = SDL_CreateTexture(sdl::window.renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, width, height);
    this->width = width;
    this->height = height;
}

Texture::~Texture() {
    SDL_DestroyTexture(texture);
}

void Texture::bytes_to_texture_text(const std::string &bytes, const RGB &fg, const RGB &bg) {
    auto bits = get_bits_8(bytes);
    for(int y = 0; y < height; ++y) {
        for(int x = 0; x < width; ++x) {
            if(bits[y][(width - 1) - x] == 1)
                point(x, y, fg);
            else
                point(x, y, bg);
        }
    }
    update();
}

void Texture::bytes_to_texture_gfx(const std::string &bytes, const RGB &fg, const RGB &bg) {
    auto bits = get_bits_16(bytes);
    for(int y = 0; y < height; ++y) {
        for(int x = 0; x < width; ++x) {
            if(bits[y][(width - 1) - x] == 1)
                point(x, y, fg);
            else
                point(x, y, bg);
        }
    }
    update();
}

void Texture::bitmask_to_texture(const std::vector<std::vector<bool>> &bit_mask, const RGB &rgb) {
    for(int y = 0; y < height; ++y) {
        for(int x = 0; x < width; ++x) {
            if(bit_mask[y][x])
                point(x, y, rgb);
            else
                point(x, y, alpha);
        }
    }
    update();
}

void Texture::point(const int& x, const int& y, const RGB &rgb) {
    const int i = (y * width + x) * 4;
    pixels[i + 0] = rgb.blue;
    pixels[i + 1] = rgb.green;
    pixels[i + 2] = rgb.red;
    pixels[i + 3] = rgb.alpha;
}

void Texture::update() {
    SDL_UpdateTexture(texture, NULL, pixels.data(), width * 4);
}

std::vector<std::bitset<8>> Texture::get_bits_8(const std::string &bytes) {
    std::vector<std::bitset<8>> bits;
    for(int i = 0; i < bytes.size() / 2; ++i) {
        auto hex = bytes.substr(i * 2, 2);
        auto dec = stoul(hex, 0, 16);
        std::bitset<8> b(dec);
        bits.push_back(b);
    }
    return bits;
}

std::vector<std::bitset<16>> Texture::get_bits_16(const std::string &bytes) {
    std::vector<std::bitset<16>> bits;
    for(int i = 0; i < 16; ++i) {
        auto hex = bytes.substr(i * 4, 4);
        auto dec = stoul(hex, 0, 16);
        std::bitset<16> b(dec);
        bits.push_back(b);
    }
    return bits;
}

SDL_Window::SDL_Window() {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    window = SDL_CreateWindow("Chaos: The Battle of Wizards", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_INPUT_FOCUS);
    SDL_ShowCursor(SDL_DISABLE);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    buffer = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, WINDOW_WIDTH, WINDOW_HEIGHT);
    SDL_SetRenderTarget(renderer, buffer);
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 1, 4096);
}

SDL_Window::~SDL_Window() {
    Mix_CloseAudio();
    SDL_DestroyTexture(buffer);
    SDL_DestroyRenderer(renderer);
    SDL_ShowCursor(SDL_ENABLE);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

namespace sdl {
    SDL_Window window;
    SDL_Rect src_rect;
    SDL_Rect dst_rect;

    void clear() {
        SDL_RenderClear(window.renderer);
    }

    void present() {
        SDL_RenderPresent(window.renderer);
    }

    void refresh() {
        SDL_SetRenderTarget(window.renderer, NULL);
        SDL_RenderCopy(window.renderer, window.buffer, NULL, NULL);
        SDL_RenderPresent(window.renderer);
        SDL_SetRenderTarget(window.renderer, window.buffer);
    }

    void draw(const Texture &texture, const int& x, const int& y, const int &sx, const int &sy, const int &width, const int &height) {
        src_rect.x = sx;
        src_rect.y = sy;
        src_rect.w = width;
        src_rect.h = height;
        dst_rect.x = x * 3;
        dst_rect.y = y * 3;
        dst_rect.w = width * 3;
        dst_rect.h = height * 3;
        SDL_RenderCopy(window.renderer, texture.texture, &src_rect, &dst_rect);
    }

    void draw(const Texture &texture, const int& x, const int& y) {
        dst_rect.x = x * 3;
        dst_rect.y = y * 3;
        dst_rect.w = texture.width * 3;
        dst_rect.h = texture.height * 3;
        SDL_RenderCopy(window.renderer, texture.texture, NULL, &dst_rect);
    }

    SDL_Rect rect;

    void clear(const int& x, const int& y, const int& width, const int& height) {
        rect.x = x;
        rect.y = y;
        rect.w = width;
        rect.h = height;
        SDL_RenderFillRect(window.renderer, &rect);
    }
}
