#include "chaos.h"
#include "gfx.h"
#include <SDL2/SDL.h>
#include <string>
#include <vector>
#include <map>
#include <bitset>

#define WINDOW_WIDTH 256 * 3
#define WINDOW_HEIGHT 192 * 3

SDL_Window window;

RGB::RGB(const unsigned char& red, const unsigned char& green, const unsigned char& blue) {
    this->red = red;
    this->green = green;
    this->blue = blue;
}

int RGB::get_24_bit_index() const {
    return (red << 16) + (green << 8) + blue;
}

const RGB black(0, 0, 0);
const RGB blue(0, 0, 205);
const RGB red(205, 0, 0);
const RGB purple(205, 0, 205);
const RGB green(0, 205, 0);
const RGB cyan(0, 205, 205);
const RGB yellow(205, 205, 0);
const RGB white(205, 205, 205);
const RGB bright_black(0, 0, 0);
const RGB bright_blue(0, 0, 255);
const RGB bright_red(255, 0, 0);
const RGB bright_purple(255, 0, 255);
const RGB bright_green(0, 255, 0);
const RGB bright_cyan(0, 255, 255);
const RGB bright_yellow(255, 255, 0);
const RGB bright_white(255, 255, 255);

std::vector<std::bitset<8>> get_bits_text_character(std::string& bytes) {
    std::vector<std::bitset<8>> bits;
    for(int i = 0; i < 16; ++i) {
        auto hex = bytes.substr(i * 2, 2);
        auto dec = stoul(hex, 0, 16);
        std::bitset<8> b(dec);
        bits.push_back(b);
    }
    return bits;
}

std::vector<std::bitset<16>> get_bits_gfx(std::string& bytes) {
    std::vector<std::bitset<16>> bits;
    for(int i = 0; i < 16; ++i) {
        auto hex = bytes.substr(i * 4, 4);
        auto dec = stoul(hex, 0, 16);
        std::bitset<16> b(dec);
        bits.push_back(b);
    }
    return bits;
}

struct Texture {
    Texture(const int& width = 0, const int& height = 0) : pixels(width * height * 4) {
        texture = SDL_CreateTexture(window.renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, width, height);
        SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
        this->width = width;
        this->height = height;
    }

    Texture(std::string& bytes, const RGB& fg, const RGB& bg) {
        bool text_character = (bytes.size() == 32);
        if(text_character) {
            texture = SDL_CreateTexture(window.renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, 8, 16);
            this->width = 8;
            this->height = 16;
            pixels = std::vector<unsigned char>(this->width * this->height * 4);
            auto bits = get_bits_text_character(bytes);
            for(int y = 0; y < 16; ++y) {
                for(int x = 0; x < 8; ++x) {
                    if(bits[y][7 - x] == 1)
                        point(x, y, fg);
                    else
                        point(x, y, bg);
                }
            }
        } else {
            texture = SDL_CreateTexture(window.renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, 16, 16);
            this->width = 16;
            this->height = 16;
            pixels = std::vector<unsigned char>(this->width * this->height * 4);
            auto bits = get_bits_gfx(bytes);
            for(int y = 0; y < 16; ++y) {
                for(int x = 0; x < 16; ++x) {
                    if(bits[y][15 - x] == 1)
                        point(x, y, fg);
                    else
                        point(x, y, bg);
                }
            }
        }
        update();
    }

    Texture(std::string& bytes, const RGB& fg) {
        texture = SDL_CreateTexture(window.renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, 16, 16);
        SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
        this->width = 16;
        this->height = 16;
        pixels = std::vector<unsigned char>(this->width * this->height * 4);
        auto bits = get_bits_gfx(bytes);
        for(int y = 0; y < 16; ++y) {
            for(int x = 0; x < 16; ++x) {
                if(bits[y][15 - x] == 1)
                    point(x, y, fg);
                else
                    alpha_point(x, y);
            }
        }
        update();
    }

    ~Texture() {
        SDL_DestroyTexture(texture);
    }

    void point(const int& x, const int& y, const RGB& rgb) {
        const int i = (y * width + x) * 4;
        pixels[i + 0] = rgb.blue;
        pixels[i + 1] = rgb.green;
        pixels[i + 2] = rgb.red;
        pixels[i + 3] = 255;
    }

    void alpha_point(const int& x, const int& y) {
        const int i = (y * width + x) * 4;
        pixels[i + 0] = 0;
        pixels[i + 1] = 0;
        pixels[i + 2] = 0;
        pixels[i + 3] = 0;
    }

    void update() {
        SDL_UpdateTexture(texture, NULL, pixels.data(), width * 4);
    }

    int width = 0;
    int height = 0;
    SDL_Texture* texture;
private:
    std::vector<unsigned char> pixels;
};

SDL_Rect src_rect;
SDL_Rect dst_rect;

namespace sdl {
    void draw(const Texture& texture, const int& x, const int& y, const int& sx, const int& sy, const int& width, const int& height) {
        src_rect.x = sx;
        src_rect.y = sy;
        src_rect.w = width;
        src_rect.h = height;
        dst_rect.x = x * 3;
        dst_rect.y = y * 3;
        dst_rect.w = width * 3;
        dst_rect.h = height * 3;
        SDL_SetRenderTarget(window.renderer, window.buffer);
        SDL_RenderCopy(window.renderer, texture.texture, &src_rect, &dst_rect);
    }

    void draw(const Texture& texture, const int& x, const int& y) {
        dst_rect.x = x * 3;
        dst_rect.y = y * 3;
        dst_rect.w = texture.width * 3;
        dst_rect.h = texture.height * 3;
        SDL_SetRenderTarget(window.renderer, window.buffer);
        SDL_RenderCopy(window.renderer, texture.texture, NULL, &dst_rect);
    }

    void refresh() {
        SDL_SetRenderTarget(window.renderer, NULL);
        SDL_RenderClear(window.renderer);
        SDL_RenderCopy(window.renderer, window.buffer, NULL, NULL);
        SDL_RenderPresent(window.renderer);
    }
}

std::string corner_bytes = "ffffffffeaaff55bd087e42bd087ea0bd057e10bd427e10bdaaff557ffffffff";
std::string edge_bytes = "d057e40bd027ea0bd057e40bd027ea0bff00ff11aa44551188aa225588ff00ff";

void draw_border(const RGB& fg, const RGB& bg) {
    Texture corner(corner_bytes, fg, bg);
    Texture edge(edge_bytes, fg, bg);
    sdl::draw(corner, 0, 0, 0, 0, 8, 8);
    sdl::draw(corner, 31 * 8, 0, 8, 0, 8, 8);
    sdl::draw(corner, 0, 21 * 8, 0, 8, 8, 8);
    sdl::draw(corner, 31 * 8, 21 * 8, 8, 8, 8, 8);
    for(int x = 1; x <= 30; ++x) {
        sdl::draw(edge, x * 8, 0, 0, 8, 8, 8);
        sdl::draw(edge, x * 8, 21 * 8, 8, 8, 8, 8);
    }
    for(int y = 1; y <= 20; ++y) {
        sdl::draw(edge, 0, y * 8, 0, 0, 8, 8);
        sdl::draw(edge, 31 * 8, y * 8, 8, 0, 8, 8);
    }
}

namespace gfx {
    void draw_menu_border() {
        draw_border(bright_red, bright_yellow);
        draw_text("       PRESS KEYS 1 TO 4        ", bright_yellow, bright_red, 0, 22);
    }

    void draw_spell_border() {
        draw_border(bright_blue, bright_cyan);
        draw_text("         PRESS ANY KEY          ", bright_cyan, bright_blue, 0, 22);
    }

    void draw_info_border() {
        draw_border(bright_green, bright_black);
        draw_text("         PRESS ANY KEY          ", bright_black, bright_green, 0, 22);
    }

    void draw_arena_border() {
        draw_border(bright_blue, black);
    }
}

std::string spell_bytes = "ffff8001800187e18c3188118c0187e18031801188118c3187e180018001ffff";
std::string box_bytes = "ffffffffc003c003c003c003c003c003c003c003c003c003c003c003ffffffff";
std::string wings_bytes = "c003e007700e300c000000001c383e7c63c6018001800240300c700ee007c003";
std::string ranged_bytes = "f81fe007f00fb81d98190000000000000000000000009819b81df00fe007f83f";

std::map <int, Texture> spell_textures;
std::map <int, Texture> box_textures;
std::map <int, Texture> wings_textures;
std::map <int, Texture> ranged_textures;

void draw_cursor_texture(std::string& bytes, std::map<int, Texture>& textures, const RGB& rgb, const int& x, const int& y) {
    const auto rgb_index = rgb.get_24_bit_index(); 
    if(textures.find(rgb_index) == textures.end()) {
        auto texture = Texture(bytes, rgb);
        std::swap(textures[rgb_index], texture);
    }
    sdl::draw(textures[rgb_index], 8 + x * 16, 8 + y * 16);
}

namespace gfx {
    void draw_spell_cursor(const RGB& rgb, const int& x, const int& y) {
        draw_cursor_texture(spell_bytes, spell_textures, rgb, x, y);
    }

    void draw_box_cursor(const RGB& rgb, const int& x, const int& y) {
        draw_cursor_texture(box_bytes, box_textures, rgb, x, y);
    }

    void draw_wings_cursor(const RGB& rgb, const int& x, const int& y) {
        draw_cursor_texture(wings_bytes, wings_textures, rgb, x, y);
    }

    void draw_ranged_cursor(const RGB& rgb, const int& x, const int& y) {
        draw_cursor_texture(ranged_bytes, ranged_textures, rgb, x, y);
    }
}


std::map<std::string, std::string> character_set = {
    {" ", "00000000000000000000000000000000"},
    {"!", "30303030303030000030300000000000"},
    {"\"", "66662244000000000000000000000000"},
    {"#", "6c6cfefe6c6c6cfefe6c6c0000000000"},
    {"$", "107cfed6d0fc7e16d6fe7c1000000000"},
    {"%", "46a4ac48181030246a4ac40000000000"},
    {"&", "304848783072dacccefa720000000000"},
    {"'", "18180810000000000000000000000000"},
    {"(", "0c1818303030303018180c0000000000"},
    {")", "60303018181818183030600000000000"},
    {"*", "00001092543810385492100000000000"},
    {"+", "00001818187e7e181818000000000000"},
    {",", "00000000000000001818081000000000"},
    {"-", "0000000000fefe000000000000000000"},
    {".", "00000000000000000018180000000000"},
    {"/", "06040c08181030206040c00000000000"},
    {"0", "387ceec6ced6e6c6ee7c380000000000"},
    {"1", "1838781818181818187e7e0000000000"},
    {"2", "387cce060e3c78e0c0fefe0000000000"},
    {"3", "387cce06061c0606ce7c380000000000"},
    {"4", "060e1e3666c6fefe0606060000000000"},
    {"5", "fefec0f8fcce0606ce7c380000000000"},
    {"6", "387ce6c0f8fceec6ee7c380000000000"},
    {"7", "fefe06060c0c18183030300000000000"},
    {"8", "387ceec6ee7ceec6ee7c380000000000"},
    {"9", "387ceec6ee7e3e06ce7c380000000000"},
    {",", "00000000181800000018180000000000"},
    {";", "00000000181800000018180810000000"},
    {"<", "0000040c18306030180c040000000000"},
    {"=", "000000fefe0000fefe00000000000000"},
    {">", "0000406030180c183060400000000000"},
    {"?", "387cce060e1c18180018180000000000"},
    {"@", "387cc692aaaaaa94c07e3c0000000000"},
    {"A", "387ceec6c6fefec6c6c6c60000000000"},
    {"B", "fcfec6c6fcfec6c6c6fefc0000000000"},
    {"C", "387ce6c0c0c0c0c0e67c380000000000"},
    {"D", "f8fccec6c6c6c6c6cefcf80000000000"},
    {"E", "fefec0c0f8f8c0c0c0fefe0000000000"},
    {"F", "fefec0c0f8f8c0c0c0c0c00000000000"},
    {"G", "387ce6c0c0c0c0c6e67e3e0000000000"},
    {"H", "c6c6c6c6fefec6c6c6c6c60000000000"},
    {"I", "fcfc30303030303030fcfc0000000000"},
    {"J", "fefe18181818181838f0e00000000000"},
    {"K", "c2c6ccd8f0e0f0d8ccc6c20000000000"},
    {"L", "c0c0c0c0c0c0c0c0c0fefe0000000000"},
    {"M", "c6c6eefefed6c6c6c6c6c60000000000"},
    {"N", "c6c6e6e6f6d6dececec6c60000000000"},
    {"O", "387ceec6c6c6c6c6ee7c380000000000"},
    {"P", "f8fccec6c6cefcf8c0c0c00000000000"},
    {"Q", "387ceec6c6c6c6c6ee7c380e00000000"},
    {"R", "fcfec6c6fefccec6c6c6c60000000000"},
    {"S", "7cfec6c0f87c0e06c6fe7c0000000000"},
    {"T", "fefe1818181818181818180000000000"},
    {"U", "c6c6c6c6c6c6c6c6ee7c380000000000"},
    {"V", "c6c6c6c6c66c6c383810100000000000"},
    {"W", "c6c6c6c6c6d6fefeeec6c60000000000"},
    {"X", "c6c66c6c3838386c6cc6c60000000000"},
    {"Y", "ccccccccccfc78303030300000000000"},
    {"Z", "fefe060e1c3870e0c0fefe0000000000"},
    {"[", "7e7e606060606060607e7e0000000000"},
    {"\\", "c0406020301018080c04060000000000"},
    {"]", "7c7c0c0c0c0c0c0c0c7c7c0000000000"},
    {"^", "101038387c5410103838280000000000"},
    {"_", "000000000000000000000000ffff0000"},
    {"£", "387ce6c0c0fc7c6060fefe0000000000"},
    {"a", "000000003e7ee6c6e67e3e0000000000"},
    {"b", "c0c0c0c0f8fccec6cefcf80000000000"},
    {"c", "000000003e7ee0c0e07e3e0000000000"},
    {"d", "060606063e7ee6c6e67e3e0000000000"},
    {"e", "00000000387ce6fee07e3c0000000000"},
    {"f", "387ce6c0f8f8c0c0c0c0c00000000000"},
    {"g", "000000003e7ee6c6e67e3e06ce7c3800"},
    {"h", "c0c0c0c0f8fccec6c6c6c60000000000"},
    {"i", "303000003030303030fcfc0000000000"},
    {"j", "060600000606060606060606ce7c3800"},
    {"k", "c0c0c0c0c6def8e0f8dec60000000000"},
    {"l", "30303030303030303030180000000000"},
    {"m", "00000000ecfefed6d6c6c60000000000"},
    {"n", "00000000f8fccec6c6c6c60000000000"},
    {"o", "00000000387ceec6ee7c380000000000"},
    {"p", "00000000f8fccec6cefcf8c0c0c0c000"},
    {"q", "000000003e7ee6c6e67e3e0606060600"},
    {"r", "00000000387ce6c0c0c0c00000000000"},
    {"s", "000000007efec07c06fefc0000000000"},
    {"t", "c0c0c0c0fcfcc0c0e67c380000000000"},
    {"u", "00000000c6c6c6c6ee7c380000000000"},
    {"v", "00000000c6c66c6c3838100000000000"},
    {"w", "00000000c6c6c6d6d6fe6c0000000000"},
    {"x", "00000000c6ee7c387ceec60000000000"},
    {"y", "00000000c6c6c6c6e67e3e06ce7c3800"},
    {"z", "00000000fefe0c1830fefe0000000000"},
    {"{", "0c18181830e0301818180c0000000000"},
    {"|", "18181818181818181818180000000000"},
    {"}", "e0303030180e18303030e00000000000"},
    {"~", "00327e4c000000000000000000000000"},
    {"©", "387cc692aaa2aa92c67c380000000000"}
};

std::map<std::string, std::map<int, std::map<int, Texture>>> text_textures;

namespace gfx {
    void draw_text(std::string text, const RGB& fg, const RGB& bg, const int& x, const int& y) {
        for(int i = 0; i < text.size(); ++i) {
            const auto letter = text.substr(i, 1);
            const auto fg_index = fg.get_24_bit_index(); 
            const auto bg_index = bg.get_24_bit_index(); 
            if(text_textures[letter][fg_index].find(bg_index) == text_textures[letter][fg_index].end()) {
                auto texture = Texture(character_set[letter], fg, bg);
                std::swap(text_textures[letter][fg_index][bg_index], texture);
            }
            sdl::draw(text_textures[letter][fg_index][bg_index], (x + i) * 8, y * 8);
        }
    }
}

#define CORPSE_INDEX 4

std::vector<std::string> king_cobra_bytes = {
    "00000000000000000180008000800080008003000460041003e0000000000000",
    "00000000000000000000030001000080008003000460041003e0000000000000",
    "00000000000000000000000007000880008003000460041003e0000000000000",
    "00000000000000000000030001000080008003000460041003e0000000000000",
    "0000000000000000000000000000000000001e00203818c40700000000000000"
};

std::vector<std::string> dire_wolf_bytes = {
    "0000000000000000080078081c0827f00ff00a28094805500000000000000000",
    "0000000000000000080078041c0827f00ff00c28142814240000000000000000",
    "0000000000000000080078003c0c27f00ff00c1c281208080000000000000000",
    "0000000000000000080078041c0827f00ff00c28142814240000000000000000",
    "00000000000000000000000000000400058a220c1bf07ffe0000000000000000"
};

std::vector<std::string> goblin_bytes = {
    "0000000000006180718037e01ff01bd803c807e00660066006600ee000000000",
    "000030003000318031801fe01ff003d803c807e00660066006600ee000000000",
    "000006000c000d8019801fe00ff003d803c807e00660066006600ee000000000",
    "000030003000318031801fe01ff003d803c807e00660066006600ee000000000",
    "0000000000000000000000000000000000000000000011e23bfe3ffe00000000"
};

std::vector<std::string> crocodile_bytes = {
    "0000000000000000000000000000000c000201fc0ff81f243504010000000000",
    "0000000000000000000000000000000c00021dfc7ff81f243504010000000000",
    "0000000000000000000000000000000c70021dfc0ff81f243504010000000000",
    "0000000000000000000000000000000c00021dfc7ff81f243504010000000000",
    "000000000000000000000000000000000010200c19260ffe7ffc000000000000"
};

std::vector<std::string> faun_bytes = {
    "0000000001400180018023c015a0099003c003c0036001200120024000000000",
    "000000000140118011800bc00da0019003c003c0036001200120024000000000",
    "000000000540058009800fc001a0019003c003c0036001200120024000000000",
    "000000000140118011800bc00da0019003c003c0036001200120024000000000",
    "00000000000000000000000000000000000000000004013837e27ffc00000000"
};

std::vector<std::string> lion_bytes = {
    "000000000000000018007e007ff03ff81ffc0f2c122422440408000000000000",
    "00000000000018007c003e007ff03ff81ffc0f2c122422440408000000000000",
    "0000000000002c001c001e003ff01ff81ffc0f2c122422440408000000000000",
    "00000000000018007c003e007ff03ff81ffc0f2c122422440408000000000000",
    "0000000000000000000000000000042209921a147ffc7ffc3ff81c0000000000"
};

std::vector<std::string> elf_bytes = {
    "00000000020006c006c03fc006c006c002c000c0014001400240024000000000",
    "00000000020005c005c01fe005c005c002c000c0014001400240024000000000",
    "00000000020005c004c00ff004c005c002c000c0014001400240024000000000",
    "00000000020004c004c0f7f004e004c002c000c0014001400240024000000000",
    "000000000000000000000000000000000000000003e004901be41ffc00000000"
};

std::vector<std::string> orc_bytes = {
    "0000000000800180018040b02378157809b001c00240024006c0000000000000",
    "0000000000801180118010b013780d7801b001c00240024006c0000000000000",
    "0000000002800580058008b00f78017801b001c00240024006c0000000000000",
    "0000000000801180118010b013780d7801b001c00240024006c0000000000000",
    "000000000000000000000000000000000000000001c01be41ffc000000000000"
};

std::vector<std::string> bear_bytes = {
    "000000000300078003c001c00de007e003e001e003e003600360036000000000",
    "000000000300078003c001c007e01fe011e001e003e003600360036000000000",
    "000000000300078003c001c003e007e00de009e003e003600360036000000000",
    "000000000300078003c001c003e007e00de009e003e003600360036000000000",
    "00000000000000000000000000000000000000c027807fe27ffe3ffe00000000"
};

std::vector<std::string> gorilla_bytes = {
    "000000001980338033803ef81ef8072003c003c007e00e700c301c3800000000",
    "00000000018007e00db01ff80ff007e003c003c007e00e700c301c3800000000",
    "00000000019801cc01cc1f7c1f7804e003c003c007e00e700c301c3800000000",
    "00000000018007e00db01ff80ff007e003c003c007e00e700c301c3800000000",
    "0000000000000000000000000000000000000000018037e23ffe3ffe00000000"
};

std::vector<std::string> ogre_bytes = {
    "0000000000000180018007e04ff0fbd8f3c843e007f0063006380e0000000000",
    "000000000000618071803fe01ff003d803c803c007e0066006600e7000000000",
    "0000000018003d8019801fe00ff003d803c807c00fe00c601c60007000000000",
    "000000000000618071803fe01ff003d803c803c007e0066006600e7000000000",
    "000000000000000000000000000000000000000001801be41ffc1ffc00000000"
};

std::vector<std::string> hydra_bytes = {
    "000036c012401240ca984a883f3007c00e001cf8193c180c1c1c0ff807f00000",
    "00000d80648024b01290ca907f2007c00e001cf8193c180c1c1c0ff807f00000",
    "000003001960092064a032a0cf4077800e001cf8193c180c1c1c0ff807f00000",
    "00000d80648024b01290ca907f2007c00e001cf8193c180c1c1c0ff807f00000",
    "00000000000000000000050056207fe81ff8380071fc627e60067ffe3ff80000"
};

std::vector<std::string> giant_rat_bytes = {
    "000000000000000000000000000c009200611f017f82fffc1180204000000000",
    "00000000000000000000000c0012002100211f017f82fffc1080108000000000",
    "00000000000000000000000c0012001100091f017f82fffc11000a0000000000",
    "00000000000000000000000c0012002100211f017f82fffc1080108000000000",
    "00000000000000000000000000000000080010c01f00ff807ffe000101fe0200"
};

std::vector<std::string> giant_bytes = {
    "00c000c003f007f807f80fecfde4f9e603f203f0033003300330033003300770",
    "00c000c043f0e7f87ff83dec19e401e603f203f0033003300330033003300770",
    "30c030c033f037f83ff81dec01e401e603f203f0033003300330033003300770",
    "00c000c043f0e7f87ff83dec19e401e603f203f0033003300330033003300770",
    "000000000000000000000000000000000000000000000fc1dfffffff00000000"
};

std::vector<std::string> horse_bytes = {
    "000000000000000030007800dfec1ff20ff01428142812480a50000000000000",
    "000000000000000030007800dfec1ff20ff01428142824240820000000000000",
    "000000000000000030007800dfec1ff20ff0342c442208100810000000000000",
    "000000000000000030007800dfec1ff20ff01428142824240820000000000000",
    "0000000000000000000000000000020904ca150a33fc7ffc7ffb000000000000"
};

std::vector<std::string> unicorn_bytes = {
    "000000004000200018003c006ff60ff907f80a140a1409240528000000000000",
    "000000004000200018003c006ff70ff807f80a140a1412120410000000000000",
    "000000004000200018003c016ff60ff807f81a16221104080408000000000000",
    "000000004000200018003c006ff70ff807f80a140a1412120410000000000000",
    "00000000000000000000000000000109026a8a8a59fc3ffc3ffb000000000000"
};

std::vector<std::string> centaur_bytes = {
    "000000002c004c0044007c004e004ff62ff907f80a140a140924052800000000",
    "000000002c004c0044007c004e004ff62ff907f80a140a141212041000000000",
    "000000002c004c0044007c004e004ff62ff907f81a1622110408040800000000",
    "000000002c004c0044007c004e004ff62ff907f80a140a141212041000000000",
    "0000000000000000000000000000020904ca050a03fc6ffc7f97000000000000"
};

std::vector<std::string> pegasus_bytes = {
    "000000fc01f801e031807b00dfec1ff20ff01428142812480a50000000000000",
    "000000000000000030fc7bc0dfee1ff00ff01428142824240820000000000000",
    "000000000000000030007b00dfee1ff00ff035fc443e08100810000000000000",
    "000000000000000030fc7bc0dfee1ff00ff01428142824240820000000000000",
    "0000000000000000000000000000021204d4151433f87ff87ff603e000f80000"
};

std::vector<std::string> gryphon_bytes = {
    "0000008003c030c6798f0d9807f003e003f003d0025002500240000000000000",
    "0000010007800380018001800ff87fff03f003d0025002500240000000000000",
    "000002000f0003000580018007f00ff81ffc33d6425102500240000000000000",
    "0000010007800380018001800ff87fff03f003d0025002500240000000000000",
    "000000000000000000000000000011083a08621873f03ff81ffe07c000000000"
};

std::vector<std::string> manticore_bytes = {
    "078008fc08f065c565823b0d3ff03ff81ffc0f2c122422440408000000000000",
    "000000000000670568fe3bcd3ff03ff81ffc0f2c122422440408000000000000",
    "000000000000600560023b0d3ff03ff81ffc0f7c122422440408000000000000",
    "000000000000670568fe3bcd3ff03ff81ffc0f2c122422440408000000000000",
    "0000000000000000000000000000020414320c0a07fc6ffc7fff03e000000000"
};

std::vector<std::string> bat_bytes = {
    "0000000000000000000000000c60139001000000000000000000000000000000",
    "00000000000000000000000000000fe011100000000000000000000000000000",
    "0000000000000000000000000000038005401830000000000000000000000000",
    "00000000000000000000000000000fe011100000000000000000000000000000",
    "000000000000000000000000000000000000000000000000010013900c600000"
};

std::vector<std::string> green_dragon_bytes = {
    "0000f1c018e03cf04cf019f831e473007a707ff83ffe1ff9142d142524440808",
    "600018000c403e7006780cfc38fe71c07b707ff83ffe1ff9142d142524440808",
    "1800060003001f0003100e3c387e70f87bf07ff83ffe1ff9142d142524440808",
    "600018000c403e7006780cfc38fe71c07b707ff83ffe1ff9142d142524440808",
    "00000000000000000000000000003c007308e828c59ae7f97ffd3ffe00000000"
};

std::vector<std::string> red_dragon_bytes = {
    "0000000000001000f88018c031e231f23b8a1e020fe117f115f92479083a01fc",
    "0000700008007c800ce018f018fd1dc10f011fc227e229f208790079003a01fc",
    "30000c0006200e383c3e0c780ec21f8223c245e108f108f9007a007a003a01fc",
    "0000700008007c800ce018f018fd1dc10f011fc227e229f208790079003a01fc",
    "00000000000000000000000000003c007308e828c59ae7f97ffd3ffe00000000"
};

std::vector<std::string> golden_dragon_bytes = {
    "008017d030d8719c719cfb3efb3ecf6687f203f803d4025204520442000400f8",
    "1a0007001b004304630c731cfb3eff7ec7f683fa03d4025204520442000400f8",
    "68001c001c00660006008302c306f31efffefffe63dc425604520442000400f8",
    "1a0007001b004304630c731cfb3eff7ec7f683fa03d4025204520442000400f8",
    "000000000000000000000000000000000492a50aa3fc7fff7fff03c000f80000"
};

std::vector<std::string> harpy_bytes = {
    "0000000001e0027c1a781ae00fc00700078003c001f002400440008000000000",
    "000000000000000018001bc00ff807c0078003c001f002400440008000000000",
    "0000000000000000180018000f00078007c003f801f002400440008000000000",
    "000000000000000018001bc00ff807c0078003c001f002400440008000000000",
    "000000000000000000000000000000000000012000a033f03ffc078001e00000"
};

std::vector<std::string> eagle_bytes = {
    "000000000000000038006c7807ec038006800280000000000000000000000000",
    "00000000000000000000000003801fe07ef8029e000000000000000000000000",
    "00000000000000000000000003800fe01ef03ab8301c000c0000000000000000",
    "00000000000000000000000003801fe07ef8029e000000000000000000000000",
    "00000000000000000000000000000000000070003d000f400f8011e000780000"
};

std::vector<std::string> vampire_bytes = {
    "00000000000000000180018003c003c007e007e007f00ff00a48024000000000",
    "00000000000000000180018003c003e003f007f007f8065c0240024000000000",
    "00000000000000000180018003c003e003f003f807cc06440240024000000000",
    "00000000000000000180018003c003e003f007f007f8065c0240024000000000",
};

std::vector<std::string> ghost_bytes = {
    "00000004400c47ce693ef93effe3ce43ce414f6045e1056140d0028002100044",
    "00000000000407cc093c793effeefe47ce400e40476043a10421459000800210",
    "00000000000007c00924193c3ffefe7ece63ce404e40076043a044a104004090",
    "00000000000407c4492c793eff9afe668e430e41074003e00040008044010420",
};

std::vector<std::string> spectre_bytes = {
    "0000000000004060206011f00af007e000e000f001b001100110031800000000",
    "0000000000004060206011f00af007e000e000f001b001100110031800000000",
    "0000000000004060206011f00af007e000e000f001b001100110031800000000",
    "0000000000004060206011f00af007e000e000f001b001100110031800000000",
};

std::vector<std::string> wraith_bytes = {
    "00000000000000c000c011e00ad004d000c00120012002100210000000000000",
    "0000000001e001203b302e1835281b280f3802d006d805e80528073800000000",
    "000003f002107e18440c40044004600430041c0c0804080408c408c40ffc0000",
    "07f80408fc0c80c680c291e28ad284d2c0c26122312212121212100210021ffe",
};

std::vector<std::string> skeleton_bytes = {
    "000001800188008803f005c009c0f08001c00120011001100120014003200000",
    "000001800180008003fc45c029c0108001c00140022002200220014003600000",
    "000001801180108017f019c801c8008001c00240044004400240014002600000",
    "000001800180008003fc45c029c0108001c00140022002200220014003600000",
};

std::vector<std::string> zombie_bytes = {
    "000000000180018000801f8001800180018002800480044004200c4000000000",
    "000000000180018000801f800180018001800280028002700210060000000000",
    "000000000180018000801f800180018001800100010001e00120030000000000",
    "000000000180018000801f800180018001800280048002800180038000000000",
};

std::vector<std::string> blob_bytes = {
    "018003c007e007e003c033cc7fffffffffcc7fe039fc03be079e0f8c0f000600",
    "0000018003c003c003c0019819fe7ffe7fe019f001be039e078c078003000000",
    "00000000018003c003c0019819fc3ffc3fe019f003bc079c0700030000000000",
    "0000018003c003c003c0019819fe7ffe7fe019f001be039e078c078003000000",
};

std::vector<std::string> fire_bytes = {
    "2004208421000300050004800444024812482254329254924b6a279c1ff80000",
    "0000008001000000010001800158415462545252928a94924b6a24941db80000",
    "010000002004300410181830285048488a249312929294924b6a279c1ff80000",
    "0000000020043004110811800158415462545252928a94924b6a24941db80000",
};

std::vector<std::string> magic_wood_bytes = { 
    "028015502aa855542aa855542aa815d00ba0018001800180018003c007e00000",
    "028015502aa855542aa855542aa815d00ba0018001800180018003c007e00000",
    "028015502aa855542aa855542aa815d00ba0018001800180018003c007e00000",
    "028015502aa855542aa855542aa815d00ba0018001800180018003c007e00000",
};

std::vector<std::string> shadow_wood_bytes = {
    "0500022024b814400c8856a623a825f0098809840180018001a003d005480000",
    "0500022024b814400c8856a623a825f0098809840180018001a003d005480000",
    "0500022024b814400c8856a623a825f0098809840180018001a003d005480000",
    "0500022024b814400c8856a623a825f0098809840180018001a003d005480000",
};

std::vector<std::string> magic_castle_bytes = {
    "0a500e700a500a500a50ce73ce73ce73ce7fffff7ffe366c366c366c3e7c7ffe",
    "0a500e700a500a500a50ce73ce73ce73ce7fffff7ffe366c366c366c3e7c7ffe",
    "0a500e700a500a500a50ce73ce73ce73ce7fffff7ffe366c366c366c3e7c7ffe",
    "0a500e700a500a500a50ce73ce73ce73ce7fffff7ffe366c366c366c3e7c7ffe",
};

std::vector<std::string> dark_citadel_bytes = {
    "10081008381c381c7c3e7c3e381c399c3bdc7ffeffffce73cc33cc33fc3ffc3f",
    "10081008381c381c7c3e7c3e381c399c3bdc7ffeffffce73cc33cc33fc3ffc3f",
    "10081008381c381c7c3e7c3e381c399c3bdc7ffeffffce73cc33cc33fc3ffc3f",
    "10081008381c381c7c3e7c3e381c399c3bdc7ffeffffce73cc33cc33fc3ffc3f",
};

std::vector<std::string> wall_bytes = {
    "0000eeeeeeee0000bbbbbbbb0000eeeeeeee0000bbbbbbbb0000eeeeeeee0000",
    "0000eeeeeeee0000bbbbbbbb0000eeeeeeee0000bbbbbbbb0000eeeeeeee0000",
    "0000eeeeeeee0000bbbbbbbb0000eeeeeeee0000bbbbbbbb0000eeeeeeee0000",
    "0000eeeeeeee0000bbbbbbbb0000eeeeeeee0000bbbbbbbb0000eeeeeeee0000",
};

std::vector<std::pair<RGB, RGB>> king_cobra_rgb = {
    {bright_green, black},
    {bright_green, black},
    {bright_green, black},
    {bright_green, black},
    {bright_green, black}    
};

std::vector<std::pair<RGB, RGB>> dire_wolf_rgb = {
    {bright_yellow, black},
    {bright_yellow, black},
    {bright_yellow, black},
    {bright_yellow, black},
    {bright_yellow, black}
};

std::vector<std::pair<RGB, RGB>> goblin_rgb = {
    {bright_purple, black},
    {bright_purple, black},
    {bright_purple, black},
    {bright_purple, black},
    {bright_purple, black}
};

std::vector<std::pair<RGB, RGB>> crocodile_rgb = {
    {bright_green, black},
    {bright_green, black},
    {bright_green, black},
    {bright_green, black},
    {bright_green, black}
};

std::vector<std::pair<RGB, RGB>> faun_rgb = {
    {bright_white, black},
    {bright_white, black},
    {bright_white, black},
    {bright_white, black},
    {bright_white, black}
};

std::vector<std::pair<RGB, RGB>> lion_rgb = {
    {bright_yellow, black},
    {bright_yellow, black},
    {bright_yellow, black},
    {bright_yellow, black},
    {bright_yellow, black}
};

std::vector<std::pair<RGB, RGB>> elf_rgb = {
    {bright_green, black},
    {bright_green, black},
    {bright_green, black},
    {bright_green, black},
    {bright_green, black}
};

std::vector<std::pair<RGB, RGB>> orc_rgb = {
    {bright_cyan, black},
    {bright_cyan, black},
    {bright_cyan, black},
    {bright_cyan, black},
    {bright_cyan, black}
};

std::vector<std::pair<RGB, RGB>> bear_rgb = {
    {bright_red, black},
    {bright_red, black},
    {bright_red, black},
    {bright_red, black},
    {bright_red, black}
};

std::vector<std::pair<RGB, RGB>> gorilla_rgb = {
    {yellow, black},
    {yellow, black},
    {yellow, black},
    {yellow, black},
    {yellow, black}
};

std::vector<std::pair<RGB, RGB>> ogre_rgb = {
    {bright_red, black},
    {bright_red, black},
    {bright_red, black},
    {bright_red, black},
    {bright_red, black}
};

std::vector<std::pair<RGB, RGB>> hydra_rgb = {
    {bright_green, black},
    {bright_green, black},
    {bright_green, black},
    {bright_green, black},
    {bright_green, black}
};

std::vector<std::pair<RGB, RGB>> giant_rat_rgb = {
    {white, black},
    {white, black},
    {white, black},
    {white, black},
    {white, black}
};

std::vector<std::pair<RGB, RGB>> giant_rgb = {
    {cyan, black},
    {cyan, black},
    {cyan, black},
    {cyan, black},
    {cyan, black}
};

std::vector<std::pair<RGB, RGB>> horse_rgb = {
    {yellow, black},
    {yellow, black},
    {yellow, black},
    {yellow, black},
    {yellow, black}
};

std::vector<std::pair<RGB, RGB>> unicorn_rgb = {
    {bright_cyan, black},
    {bright_cyan, black},
    {bright_cyan, black},
    {bright_cyan, black},
    {bright_cyan, black}
};

std::vector<std::pair<RGB, RGB>> centaur_rgb = {
    {yellow, black},
    {yellow, black},
    {yellow, black},
    {yellow, black},
    {yellow, black}
};

std::vector<std::pair<RGB, RGB>> pegasus_rgb = {
    {bright_white, black},
    {bright_white, black},
    {bright_white, black},
    {bright_white, black},
    {bright_white, black}
};

std::vector<std::pair<RGB, RGB>> gryphon_rgb = {
    {white, black},
    {white, black},
    {white, black},
    {white, black},
    {white, black}
};

std::vector<std::pair<RGB, RGB>> manticore_rgb = {
    {bright_yellow, black},
    {bright_yellow, black},
    {bright_yellow, black},
    {bright_yellow, black},
    {bright_yellow, black}
};

std::vector<std::pair<RGB, RGB>> bat_rgb = {
    {bright_white, black},
    {bright_white, black},
    {bright_white, black},
    {bright_white, black},
    {bright_white, black}
};

std::vector<std::pair<RGB, RGB>> green_dragon_rgb = {
    {bright_green, black},
    {bright_green, black},
    {bright_green, black},
    {bright_green, black},
    {bright_green, black}
};

std::vector<std::pair<RGB, RGB>> red_dragon_rgb = {
    {bright_red, black},
    {bright_red, black},
    {bright_red, black},
    {bright_red, black},
    {bright_red, black}
};

std::vector<std::pair<RGB, RGB>> golden_dragon_rgb = {
    {bright_yellow, black},
    {bright_yellow, black},
    {bright_yellow, black},
    {bright_yellow, black},
    {bright_yellow, black}
};

std::vector<std::pair<RGB, RGB>> harpy_rgb = {
    {bright_cyan, black},
    {bright_cyan, black},
    {bright_cyan, black},
    {bright_cyan, black},
    {bright_cyan, black}
};

std::vector<std::pair<RGB, RGB>> eagle_rgb = {
    {bright_yellow, black},
    {bright_yellow, black},
    {bright_yellow, black},
    {bright_yellow, black},
    {bright_yellow, black}
};

std::vector<std::pair<RGB, RGB>> vampire_rgb = {
    {bright_red, black},
    {bright_red, black},
    {bright_red, black},
    {bright_red, black}
};

std::vector<std::pair<RGB, RGB>> ghost_rgb = {
    {cyan, black},
    {bright_cyan, black},
    {cyan, black},
    {bright_cyan, black}
};

std::vector<std::pair<RGB, RGB>> spectre_rgb = {
    {bright_white, black},
    {bright_cyan, black},
    {bright_purple, black},
    {bright_blue, black}
};

std::vector<std::pair<RGB, RGB>> wraith_rgb = {
    {bright_cyan, black},
    {bright_cyan, black},
    {bright_cyan, black},
    {bright_cyan, black}
};

std::vector<std::pair<RGB, RGB>> skeleton_rgb = {
    {bright_white, black},
    {bright_white, black},
    {bright_white, black},
    {bright_white, black}
};

std::vector<std::pair<RGB, RGB>> zombie_rgb = {
    {bright_cyan, black},
    {bright_cyan, black},
    {bright_cyan, black},
    {bright_cyan, black}
};

std::vector<std::pair<RGB, RGB>> blob_rgb = {
    {bright_green, black},
    {bright_green, black},
    {bright_green, black},
    {bright_green, black}
};

std::vector<std::pair<RGB, RGB>> fire_rgb = {
    {bright_yellow, black},
    {bright_yellow, black},
    {bright_yellow, black},
    {bright_yellow, black}
};

std::vector<std::pair<RGB, RGB>> magic_wood_rgb = {
    {green, black},
    {bright_green, black},
    {yellow, black},
    {bright_yellow, black}
};

std::vector<std::pair<RGB, RGB>> shadow_wood_rgb = {
    {cyan, black},
    {cyan, black},
    {bright_cyan, black},
    {bright_cyan, black}
};

std::vector<std::pair<RGB, RGB>> magic_castle_rgb = {
    {bright_cyan, black},
    {cyan, black},
    {bright_cyan, black},
    {cyan, black}
};

std::vector<std::pair<RGB, RGB>> dark_citadel_rgb = {
    {bright_purple, black},
    {bright_purple, black},
    {purple, black},
    {purple, black}
};

std::vector<std::pair<RGB, RGB>> wall_rgb = {
    {bright_red, bright_yellow},
    {bright_red, bright_yellow},
    {bright_red, bright_yellow},
    {bright_red, bright_yellow}
};

std::map<int, Texture> king_cobra_textures;
std::map<int, Texture> dire_wolf_textures;
std::map<int, Texture> goblin_textures;
std::map<int, Texture> crocodile_textures;
std::map<int, Texture> faun_textures;
std::map<int, Texture> lion_textures;
std::map<int, Texture> elf_textures;
std::map<int, Texture> orc_textures;
std::map<int, Texture> bear_textures;
std::map<int, Texture> gorilla_textures;
std::map<int, Texture> ogre_textures;
std::map<int, Texture> hydra_textures;
std::map<int, Texture> giant_rat_textures;
std::map<int, Texture> giant_textures;
std::map<int, Texture> horse_textures;
std::map<int, Texture> unicorn_textures;
std::map<int, Texture> centaur_textures;
std::map<int, Texture> pegasus_textures;
std::map<int, Texture> gryphon_textures;
std::map<int, Texture> manticore_textures;
std::map<int, Texture> bat_textures;
std::map<int, Texture> green_dragon_textures;
std::map<int, Texture> red_dragon_textures;
std::map<int, Texture> golden_dragon_textures;
std::map<int, Texture> harpy_textures;
std::map<int, Texture> eagle_textures;
std::map<int, Texture> vampire_textures;
std::map<int, Texture> ghost_textures;
std::map<int, Texture> spectre_textures;
std::map<int, Texture> wraith_textures;
std::map<int, Texture> skeleton_textures;
std::map<int, Texture> zombie_textures;
std::map<int, Texture> blob_textures;
std::map<int, Texture> fire_textures;
std::map<int, Texture> magic_wood_textures;
std::map<int, Texture> shadow_wood_textures;
std::map<int, Texture> magic_castle_textures;
std::map<int, Texture> dark_citadel_textures;
std::map<int, Texture> wall_textures;

void draw_creature_texture(std::vector<std::string>& creature_bytes, std::map<int, Texture>& creature_textures, const int& x, const int& y, const int& sprite_index, std::vector<std::pair<RGB, RGB>>& rgbs) {
    if(creature_textures.find(sprite_index) == creature_textures.end()) {
        auto texture = Texture(creature_bytes[sprite_index], std::get<0>(rgbs[sprite_index]), std::get<1>(rgbs[sprite_index]));
        std::swap(creature_textures[sprite_index], texture);
    }
    sdl::draw(creature_textures[sprite_index], 8 + x * 16, 8 + y * 16);
}

namespace gfx {
    void draw_king_cobra(const int& x, const int& y, const int& sprite_index) {
        draw_creature_texture(king_cobra_bytes, king_cobra_textures, x, y, sprite_index, king_cobra_rgb);
    }

    void draw_king_cobra_corpse(const int& x, const int& y) {
        draw_king_cobra(x, y, CORPSE_INDEX);
    }

    void draw_dire_wolf(const int& x, const int& y, const int& sprite_index) {
        draw_creature_texture(dire_wolf_bytes, dire_wolf_textures, x, y, sprite_index, dire_wolf_rgb);
    }

    void draw_dire_wolf_corpse(const int& x, const int& y) {
        draw_dire_wolf(x, y, CORPSE_INDEX);
    }

    void draw_goblin(const int& x, const int& y, const int& sprite_index) {
        draw_creature_texture(goblin_bytes, goblin_textures, x, y, sprite_index, goblin_rgb);
    }

    void draw_goblin_corpse(const int& x, const int& y) {
        draw_goblin(x, y, CORPSE_INDEX);
    }

    void draw_crocodile(const int& x, const int& y, const int& sprite_index) {
        draw_creature_texture(crocodile_bytes, crocodile_textures, x, y, sprite_index, crocodile_rgb);
    }

    void draw_crocodile_corpse(const int& x, const int& y) {
        draw_crocodile(x, y, CORPSE_INDEX);
    }

    void draw_faun(const int& x, const int& y, const int& sprite_index) {
        draw_creature_texture(faun_bytes, faun_textures, x, y, sprite_index, faun_rgb);
    }

    void draw_faun_corpse(const int& x, const int& y) {
        draw_faun(x, y, CORPSE_INDEX);
    }

    void draw_lion(const int& x, const int& y, const int& sprite_index) {
        draw_creature_texture(lion_bytes, lion_textures, x, y, sprite_index, lion_rgb);
    }

    void draw_lion_corpse(const int& x, const int& y) {
        draw_lion(x, y, CORPSE_INDEX);
    }

    void draw_elf(const int& x, const int& y, const int& sprite_index) {
        draw_creature_texture(elf_bytes, elf_textures, x, y, sprite_index, elf_rgb);
    }

    void draw_elf_corpse(const int& x, const int& y) {
        draw_elf(x, y, CORPSE_INDEX);
    }

    void draw_orc(const int& x, const int& y, const int& sprite_index) {
        draw_creature_texture(orc_bytes, orc_textures, x, y, sprite_index, orc_rgb);
    }

    void draw_orc_corpse(const int& x, const int& y) {
        draw_orc(x, y, CORPSE_INDEX);
    }

    void draw_bear(const int& x, const int& y, const int& sprite_index) {
        draw_creature_texture(bear_bytes, bear_textures, x, y, sprite_index, bear_rgb);
    }

    void draw_bear_corpse(const int& x, const int& y) {
        draw_bear(x, y, CORPSE_INDEX);
    }

    void draw_gorilla(const int& x, const int& y, const int& sprite_index) {
        draw_creature_texture(gorilla_bytes, gorilla_textures, x, y, sprite_index, gorilla_rgb);
    }

    void draw_gorilla_corpse(const int& x, const int& y) {
        draw_gorilla(x, y, CORPSE_INDEX);
    }

    void draw_ogre(const int& x, const int& y, const int& sprite_index) {
        draw_creature_texture(ogre_bytes, ogre_textures, x, y, sprite_index, ogre_rgb);
    }

    void draw_ogre_corpse(const int& x, const int& y) {
        draw_ogre(x, y, CORPSE_INDEX);
    }

    void draw_hydra(const int& x, const int& y, const int& sprite_index) {
        draw_creature_texture(hydra_bytes, hydra_textures, x, y, sprite_index, hydra_rgb);
    }

    void draw_hydra_corpse(const int& x, const int& y) {
        draw_hydra(x, y, CORPSE_INDEX);
    }

    void draw_giant_rat(const int& x, const int& y, const int& sprite_index) {
        draw_creature_texture(giant_rat_bytes, giant_rat_textures, x, y, sprite_index, giant_rat_rgb);
    }

    void draw_giant_rat_corpse(const int& x, const int& y) {
        draw_giant_rat(x, y, CORPSE_INDEX);
    }

    void draw_giant(const int& x, const int& y, const int& sprite_index) {
        draw_creature_texture(giant_bytes, giant_textures, x, y, sprite_index, giant_rgb);
    }

    void draw_giant_corpse(const int& x, const int& y) {
        draw_giant(x, y, CORPSE_INDEX);
    }

    void draw_horse(const int& x, const int& y, const int& sprite_index) {
        draw_creature_texture(horse_bytes, horse_textures, x, y, sprite_index, horse_rgb);
    }

    void draw_horse_corpse(const int& x, const int& y) {
        draw_horse(x, y, CORPSE_INDEX);
    }

    void draw_unicorn(const int& x, const int& y, const int& sprite_index) {
        draw_creature_texture(unicorn_bytes, unicorn_textures, x, y, sprite_index, unicorn_rgb);
    }

    void draw_unicorn_corpse(const int& x, const int& y) {
        draw_unicorn(x, y, CORPSE_INDEX);
    }

    void draw_centaur(const int& x, const int& y, const int& sprite_index) {
        draw_creature_texture(centaur_bytes, centaur_textures, x, y, sprite_index, centaur_rgb);
    }

    void draw_centaur_corpse(const int& x, const int& y) {
        draw_centaur(x, y, CORPSE_INDEX);
    }

    void draw_pegasus(const int& x, const int& y, const int& sprite_index) {
        draw_creature_texture(pegasus_bytes, pegasus_textures, x, y, sprite_index, pegasus_rgb);
    }

    void draw_pegasus_corpse(const int& x, const int& y) {
        draw_pegasus(x, y, CORPSE_INDEX);
    }

    void draw_gryphon(const int& x, const int& y, const int& sprite_index) {
        draw_creature_texture(gryphon_bytes, gryphon_textures, x, y, sprite_index, gryphon_rgb);
    }

    void draw_gryphon_corpse(const int& x, const int& y) {
        draw_gryphon(x, y, CORPSE_INDEX);
    }

    void draw_manticore(const int& x, const int& y, const int& sprite_index) {
        draw_creature_texture(manticore_bytes, manticore_textures, x, y, sprite_index, manticore_rgb);
    }

    void draw_manticore_corpse(const int& x, const int& y) {
        draw_manticore(x, y, CORPSE_INDEX);
    }

    void draw_bat(const int& x, const int& y, const int& sprite_index) {
        draw_creature_texture(bat_bytes, bat_textures, x, y, sprite_index, bat_rgb);
    }

    void draw_bat_corpse(const int& x, const int& y) {
        draw_bat(x, y, CORPSE_INDEX);
    }

    void draw_green_dragon(const int& x, const int& y, const int& sprite_index) {
        draw_creature_texture(green_dragon_bytes, green_dragon_textures, x, y, sprite_index, green_dragon_rgb);
    }

    void draw_green_dragon_corpse(const int& x, const int& y) {
        draw_green_dragon(x, y, CORPSE_INDEX);
    }

    void draw_red_dragon(const int& x, const int& y, const int& sprite_index) {
        draw_creature_texture(red_dragon_bytes, red_dragon_textures, x, y, sprite_index, red_dragon_rgb);
    }

    void draw_red_dragon_corpse(const int& x, const int& y) {
        draw_red_dragon(x, y, CORPSE_INDEX);
    }

    void draw_golden_dragon(const int& x, const int& y, const int& sprite_index) {
        draw_creature_texture(golden_dragon_bytes, golden_dragon_textures, x, y, sprite_index, golden_dragon_rgb);
    }

    void draw_golden_dragon_corpse(const int& x, const int& y) {
        draw_golden_dragon(x, y, CORPSE_INDEX);
    }

    void draw_harpy(const int& x, const int& y, const int& sprite_index) {
        draw_creature_texture(harpy_bytes, harpy_textures, x, y, sprite_index, harpy_rgb);
    }

    void draw_harpy_corpse(const int& x, const int& y) {
        draw_harpy(x, y, CORPSE_INDEX);
    }

    void draw_eagle(const int& x, const int& y, const int& sprite_index) {
        draw_creature_texture(eagle_bytes, eagle_textures, x, y, sprite_index, eagle_rgb);
    }

    void draw_eagle_corpse(const int& x, const int& y) {
        draw_eagle(x, y, CORPSE_INDEX);
    }

    void draw_vampire(const int& x, const int& y, const int& sprite_index) {
        draw_creature_texture(vampire_bytes, vampire_textures, x, y, sprite_index, vampire_rgb);
    }

    void draw_ghost(const int& x, const int& y, const int& sprite_index) {
        draw_creature_texture(ghost_bytes, ghost_textures, x, y, sprite_index, ghost_rgb);
    }

    void draw_spectre(const int& x, const int& y, const int& sprite_index) {
        draw_creature_texture(spectre_bytes, spectre_textures, x, y, sprite_index, spectre_rgb);
    }

    void draw_wraith(const int& x, const int& y, const int& sprite_index) {
        draw_creature_texture(wraith_bytes, wraith_textures, x, y, sprite_index, wraith_rgb);
    }

    void draw_skeleton(const int& x, const int& y, const int& sprite_index) {
        draw_creature_texture(skeleton_bytes, skeleton_textures, x, y, sprite_index, skeleton_rgb);
    }

    void draw_zombie(const int& x, const int& y, const int& sprite_index) {
        draw_creature_texture(zombie_bytes, zombie_textures, x, y, sprite_index, zombie_rgb);
    }

    void draw_blob(const int& x, const int& y, const int& sprite_index) {
        draw_creature_texture(blob_bytes, blob_textures, x, y, sprite_index, blob_rgb);
    }

    void draw_fire(const int& x, const int& y, const int& sprite_index) {
        draw_creature_texture(fire_bytes, fire_textures, x, y, sprite_index, fire_rgb);
    }

    void draw_magic_wood(const int& x, const int& y, const int& sprite_index) {
        draw_creature_texture(magic_wood_bytes, magic_wood_textures, x, y, sprite_index, magic_wood_rgb);
    }

    void draw_shadow_wood(const int& x, const int& y, const int& sprite_index) {
        draw_creature_texture(shadow_wood_bytes, shadow_wood_textures, x, y, sprite_index, shadow_wood_rgb);
    }

    void draw_magic_castle(const int& x, const int& y, const int& sprite_index) {
        draw_creature_texture(magic_castle_bytes, magic_castle_textures, x, y, sprite_index, magic_castle_rgb);
    }

    void draw_dark_citadel(const int& x, const int& y, const int& sprite_index) {
        draw_creature_texture(dark_citadel_bytes, dark_citadel_textures, x, y, sprite_index, dark_citadel_rgb);
    }

    void draw_wall(const int& x, const int& y, const int& sprite_index) {
        draw_creature_texture(wall_bytes, wall_textures, x, y, sprite_index, wall_rgb);
    }
}

std::vector<std::string> wizard_bytes = {
    "000000000180018007fe0f9c3b8c1b84138013c003c003c007e01fe000000000",
    "000000000180118013c017e21b941388138013c0124012401240124000000000",
    "0000000001800180038007c007f803b0039003c007c007c00f60030000000000",
    "00000000018001803fc00fe001f001d803c807c006c006c006600ee000000000",
    "0000000001800180008043e025d019c809c805c00360073004900c5800000000",
    "000000000180018000801fc00ce208d401c803d003e003e003f000fc00000000",
    "0000181819980db00e7007e003c003c003c003c007c007c007c00fe000000000",
    "00000000018001800080418023c015a009c00380028002400220064000000000"
};

std::map<int, std::map<int, Texture>> wizard_textures;

void draw_indexed_coloured_texture(std::vector<std::string>& bytes, std::map<int, std::map<int, Texture>>& textures, const int& sprite_index, const RGB& rgb, const int& x, const int& y) {
    const auto rgb_index = rgb.get_24_bit_index(); 
    if(textures[sprite_index].find(rgb_index) == textures[sprite_index].end()) {
        auto texture = Texture(bytes[sprite_index], rgb, black);
        std::swap(textures[sprite_index][rgb_index], texture);
    }
    sdl::draw(textures[sprite_index][rgb_index], 8 + x * 16, 8 + y * 16);
}

namespace gfx {
    void draw_wizard(const int& sprite_index, const RGB& rgb, const int& x, const int& y) {
        draw_indexed_coloured_texture(wizard_bytes, wizard_textures, sprite_index, rgb, x, y);
    }
}

std::vector<std::string> magic_sword_bytes = {
    "0000000000008180418021c013c00fc000c001c003c003c003e007f000000000",
    "0000000020002180118011c00fc003c000c001c003c003c003e007f000000000",
    "080008000800098009800dc007c001c000c001c003c003c003e007f000000000",
    "0000000020002180118011c00fc003c000c001c003c003c003e007f000000000"
};

std::vector<std::string> magic_knife_bytes = {
    "0000000000008180418021c013c00fc000c001c003c003c003e007f000000000",
    "0000000020002180118011c00fc003c000c001c003c003c003e007f000000000",
    "080008000800098009800dc007c001c000c001c003c003c003e007f000000000",
    "0000000020002180118011c00fc003c000c001c003c003c003e007f000000000"
};

std::vector<std::string> magic_armour_bytes = {
    "0000000000008180418021c013c00fc000c001c003c003c003e007f000000000",
    "0000000020002180118011c00fc003c000c001c003c003c003e007f000000000",
    "080008000800098009800dc007c001c000c001c003c003c003e007f000000000",
    "0000000020002180118011c00fc003c000c001c003c003c003e007f000000000"
};

std::vector<std::string> magic_shield_bytes = {
    "0000000000000180018001c003b0057809780378037803b007c007f000000000",
    "0000000000000180018001c003b0057809780378037803b007c007f000000000",
    "0000000000000180018001c003b0057809780378037803b007c007f000000000",
    "0000000000000180018001c003b0057809780378037803b007c007f000000000"
};

std::vector<std::string> magic_wings_bytes = {
    "000000000000318c799effff8ff105a0099003c0024006600420042000000000",
    "000000000000018001800ff03ffc7dbe899103c0024006600420042000000000",
    "0000000000000180018003c00ff01ff8199833cc224426640420042000000000",
    "000000000000018001800ff03ffc7dbe899103c0024006600420042000000000"
};

std::vector<std::string> magic_bow_bytes = {
    "000001000360036003701ff003700370037001f001f001f001f801fc00000000",
    "00000100016002e002f00ff802f002f0017001f001f001f001f801fc00000000",
    "0000008001600160027007fc02700170017000f001f001f001f801fc00000000",
    "00000100036003600370fbf803700370037001f001f001f001f801fc00000000"
};

std::map<int, std::map<int, Texture>> magic_sword_textures;
std::map<int, std::map<int, Texture>> magic_knife_textures;
std::map<int, std::map<int, Texture>> magic_armour_textures;
std::map<int, std::map<int, Texture>> magic_shield_textures;
std::map<int, std::map<int, Texture>> magic_wings_textures;
std::map<int, std::map<int, Texture>> magic_bow_textures;

namespace gfx {
    void draw_magic_sword(const int& sprite_index, const RGB& rgb, const int& x, const int& y) {
        draw_indexed_coloured_texture(magic_sword_bytes, magic_sword_textures, sprite_index, rgb, x, y);
    }

    void draw_magic_knife(const int& sprite_index, const RGB& rgb, const int& x, const int& y) {
        draw_indexed_coloured_texture(magic_knife_bytes, magic_knife_textures, sprite_index, rgb, x, y);
    }

    void draw_magic_armour(const int& sprite_index, const RGB& rgb, const int& x, const int& y) {
        draw_indexed_coloured_texture(magic_armour_bytes, magic_armour_textures, sprite_index, rgb, x, y);
    }

    void draw_magic_shield(const int& sprite_index, const RGB& rgb, const int& x, const int& y) {
        draw_indexed_coloured_texture(magic_shield_bytes, magic_shield_textures, sprite_index, rgb, x, y);
    }

    void draw_magic_wings(const int& sprite_index, const RGB& rgb, const int& x, const int& y) {
        draw_indexed_coloured_texture(magic_wings_bytes, magic_wings_textures, sprite_index, rgb, x, y);
    }

    void draw_magic_bow(const int& sprite_index, const RGB& rgb, const int& x, const int& y) {
        draw_indexed_coloured_texture(magic_bow_bytes, magic_bow_textures, sprite_index, rgb, x, y);
    }
}

std::vector<std::string> exploding_circle_bytes = {
    "0000000000000000000000000000018001800000000000000000000000000000",
    "00000000000000000000018003c007e007e003c0018000000000000000000000",
    "00000000000003c00ff00ff01ff81ff81ff81ff80ff00ff003c0000000000000",
    "000007e01ff83ffc3ffc7ffe7ffe7ffe7ffe7ffe7ffe3ffc3ffc1ff807e00000",
    "07e01ff83ffc7ffe7ffefffffffffe7ffe7fffffffff7ffe7ffe3ffc1ff807e0",
    "07e01ff83ffc7ffe7ffefc3ff81ff81ff81ff81ffc3f7ffe7ffe3ffc1ff807e0",
    "07e01ff83ffc7c3e700ef00fe007e007e007e007f00f700e7c3e3ffc1ff807e0",
    "07e01818200440024002800180018001800180018001400240022004181807e0",
    "0000000000000000000000000000000000000000000000000000000000000000"
};

std::vector<std::string> twirl_bytes = {
  "0180018001800180018001800000fc3ffc3f0000018001800180018001800180",
  "0010001800386030f8703e600c20000000000430067c0e1f0c061c0018000800",
  "00000000300c381c1c380e7006600000000006600e701c38381c300c00000000",
  "080018001c000c060e1f067c0430000000000c203e60f8706030003800180010",
  "0180018001800180018001800000fc3ffc3f0000018001800180018001800180",
  "0010001800386030f8703e600c20000000000430067c0e1f0c061c0018000800",
  "00000000300c381c1c380e7006600000000006600e701c38381c300c00000000",
  "080018001c000c060e1f067c0430000000000c203e60f8706030003800180010",
  "0180018001800180018001800000fc3ffc3f0000018001800180018001800180",
  "0010001800386030f8703e600c20000000000430067c0e1f0c061c0018000800",
  "00000000300c381c1c380e7006600000000006600e701c38381c300c00000000",
  "080018001c000c060e1f067c0430000000000c203e60f8706030003800180010",
  "0180018001800180018000000000f81ff81f0000000001800180018001800180",
  "0010001800386030f8303800000000000008000000000c1c0c1f1c0618001800",
  "00006006700e300c000000000000000000080018001800000c000e0006000000",
  "08001800180000060007000000000000000000000000e0006000001800180010",
  "0180000000000000000000000000800180010000000000000000000000000180",
  "0000000000000000000000000000000000000000000000000000000000000000"
};

std::vector<std::string> explosion_bytes = {
  "000000000000000002a00ac007e00fc007e00ac0054000000000000000000000",
  "000000000000029012a00ac007f03d4007e01ad0054009400000000000000000",
  "000002000248229012a00bc006b87d4006e01bd0254809401120010000000000",
  "020002444248229010a00b4006acf84006a01950644809441120212001000000",
  "022282444248228000a00a0004abd00004801150600081441122212141108100",
  "0222824400002090120000000012a00000000840412880000122000141108110",
  "0222800000482200000000000009800000000000100080240000011100008110"
};

std::vector<std::string> dragon_burn_bytes = {
  "0000000000000000000000000480038001c00200000000000000000000000000",
  "000000000000000000000440068003c005f002a0040000000000000000000000",
  "00000000040008800a600cc0068003900be00650000008000000000000000000",
  "00000240242008600a4002200bc427e813b00d40004408000000000000000000",
  "0040022004440142020a0ab24ac447f023a01940000000020020400000000000",
  "0240004409020a0a429046800408421010100000012000000000002000000000",
  "014409040a022088448004004008001000000000000001100000000000200000",
  "0904000020880400000000080000000000000000000000000000000000000000",
  "0000000000000000000000000000000000000000000000000000000000000000"
};

std::vector<std::string> attack_bytes = {
    "01004004000000000100054003808fe203800540010000000000400401000000",
    "00000000000001000920054000001c7000000540092001000000000000000000",
    "0000000001001110082000000000301800000000082011100100000000000000",
    "0000010021081010000000000000610c00000000000010102108010000000000",
    "01004004000000000100054003808fe203800540010000000000400401000000",
    "00000000000001000920054000001c7000000540092001000000000000000000",
    "0000000001001110082000000000301800000000082011100100000000000000",
    "0000010021081010000000000000610c00000000000010102108010000000000",
    "01004004000000000100054003808fe203800540010000000000400401000000",
    "00000000000001000920054000001c7000000540092001000000000000000000",
    "0000000001001110082000000000301800000000082011100100000000000000",
    "0000010021081010000000000000610c00000000000010102108010000000000",
    "01004004000000000100054003808fe203800540010000000000400401000000",
    "00000000000001000920054000001c7000000540092001000000000000000000",
    "0000000001001110082000000000301800000000082011100100000000000000",
    "0000010021081010000000000000610c00000000000010102108010000000000",
    "01004004000000000100054003808fe203800540010000000000400401000000",
    "00000000000001000920054000001c7000000540092001000000000000000000",
    "0000000001001110082000000000301800000000082011100100000000000000",
    "0000010021081010000000000000610c00000000000010102108010000000000"
};

const RGB exploding_circle_rgb = bright_white;
const RGB twirl_rgb = bright_cyan;
const RGB explosion_rgb = bright_yellow;
const RGB dragon_burn_rgb = bright_yellow;
const RGB attack_rgb = bright_yellow;

std::map<int, Texture> exploding_circle_textures;
std::map<int, Texture> twirl_textures;
std::map<int, Texture> explosion_textures;
std::map<int, Texture> dragon_burn_textures;
std::map<int, Texture> attack_textures;

void draw_effect_texture(std::vector<std::string>& bytes, std::map<int, Texture>& textures, const int& sprite_index, const RGB& rgb, const int& x, const int& y) {
    if(textures.find(sprite_index) == textures.end()) {
        auto texture = Texture(bytes[sprite_index], rgb, black);
        std::swap(textures[sprite_index], texture);
    }
    sdl::draw(textures[sprite_index], 8 + x * 16, 8 + y * 16);
}

namespace gfx {
    void draw_exploding_circle(const int& sprite_index, const int& x, const int& y) {
        draw_effect_texture(exploding_circle_bytes, exploding_circle_textures, sprite_index, exploding_circle_rgb, x, y);
    }

    void draw_twirl(const int& sprite_index, const int& x, const int& y) {
        draw_effect_texture(twirl_bytes, twirl_textures, sprite_index, twirl_rgb, x, y);
    }

    void draw_explosion(const int& sprite_index, const int& x, const int& y) {
        draw_effect_texture(explosion_bytes, explosion_textures, sprite_index, explosion_rgb, x, y);
    }

    void draw_dragon_burn(const int& sprite_index, const int& x, const int& y) {
        draw_effect_texture(dragon_burn_bytes, dragon_burn_textures, sprite_index, dragon_burn_rgb, x, y);
    }

    void draw_attack(const int& sprite_index, const int& x, const int& y) {
        draw_effect_texture(attack_bytes, attack_textures, sprite_index, attack_rgb, x, y);
    }
}

std::vector<std::vector<bool>> simple_beam_bitmask = {
    {true}
};

std::vector<std::vector<bool>> spell_beam_bitmask = {
    {false, true, false},
    {true, true, true},
    {false, true, false}
};

std::vector<std::vector<bool>> burn_beam_bitmask = {
    {false, false, false, true, false, false, false},
    {false, true, false, false, false, true, false},
    {false, false, false, true, false, false, false},
    {true, false, true, true, true, false, true},
    {false, false, false, true, false, false, false},
    {false, true, false, false, false, true, false},
    {false, false, false, true, false, false, false}
};

std::map<int, Texture> simple_beam_texures;
std::map<int, Texture> spell_beam_texures;
std::map<int, Texture> burn_beam_texures;

void draw_beam_texture(std::vector<std::vector<bool>>& bit_mask, std::map<int, Texture>& textures, const RGB& rgb, const int& x, const int& y) {
    const int rgb_index = rgb.get_24_bit_index();
    if(textures.find(rgb_index) == textures.end()) {
        auto width = bit_mask.front().size();
        auto height = bit_mask.size();
        auto texture = Texture(width, height);
        
        for(int y = 0; y < height; ++y) {
            for(int x = 0; x < width; ++x) {
                if(bit_mask[y][x])
                    texture.point(x, y, rgb);
                else
                    texture.alpha_point(x, y);
            }
        }
        texture.update();
        std::swap(textures[rgb_index], texture);
    }
    int offset = 0;
    if(textures[rgb_index].width > 0) {
        offset = (textures[rgb_index].width - 1) / 2;
    }
    sdl::draw(textures[rgb_index], 8 + x - offset, 8 + y - offset);
}

namespace gfx {
    void draw_simple_beam(const RGB& rgb, const int& x, const int& y) {
        draw_beam_texture(simple_beam_bitmask, simple_beam_texures, rgb, x, y);
    }

    void draw_spell_beam(const RGB& rgb, const int& x, const int& y) {
        draw_beam_texture(spell_beam_bitmask, spell_beam_texures, rgb, x, y);
    }

    void draw_burn_beam(const RGB& rgb, const int& x, const int& y) {
        draw_beam_texture(burn_beam_bitmask, burn_beam_texures, rgb, x, y);
    }
}
