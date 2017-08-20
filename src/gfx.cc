#include "gfx.h"
#include "sdl.h"
#include "event.h"
#include "audio.h"
#include <cstdlib>
#include <map>
#include <cmath>

struct TextTexture : public Texture {
    TextTexture(const std::string &bytes, const RGB &fg, const RGB &bg) : Texture(8, 16) {
        bytes_to_texture_text(bytes, fg, bg);
    }
};

struct SpriteTexture : public Texture {
    SpriteTexture(const std::string &bytes, const RGB &fg, const RGB &bg) : Texture(16, 16) {
        bytes_to_texture_gfx(bytes, fg, bg);
    }

    SpriteTexture(const std::string &bytes, const RGB &fg) : Texture(16, 16) {
        SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
        bytes_to_texture_gfx(bytes, fg, alpha);
    }

    SpriteTexture(const std::vector<std::vector<bool>> &bit_mask, const RGB &rgb) : Texture(bit_mask.front().size(), bit_mask.size()) {
        SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
        bitmask_to_texture(bit_mask, rgb);
    }
};

struct ScreenTexture : public Texture {
    ScreenTexture(const std::vector<std::string> &screen_bytes, const std::vector<std::string> &attribute_bytes) : Texture(screen_bytes.size() * 4, screen_bytes.front().size() * 8) {
        std::map<int, std::map<int, std::pair<const RGB *, const RGB *>>> pen_and_paper;
        for(int y = 0; y < attribute_bytes.size(); ++y) {
            for(int x = 0; x < attribute_bytes[y].size() / 2; ++x) {
                auto first_hex = attribute_bytes[y].substr(x * 2 + 1, 1);
                auto second_hex = attribute_bytes[y].substr(x * 2, 1);
                auto attribute = std::stoi(first_hex) + (std::stoi(second_hex) << 4);
                auto pen = attribute & 0b00000111;
                auto paper = (attribute & 0b01111000) >> 3;
                if(paper & 0b00001000)
                    pen += 0b00001000;
                pen_and_paper[x][y] = {&palette[pen], &palette[paper]};
            }
        }
        for(int y = 0; y < screen_bytes.size(); ++y) {
            auto bits = get_bits_8(screen_bytes[y]);
            for(int i = 0, x = 0; i < bits.size(); ++i) {
                for(int j = 0; j < 8; ++j, ++x) {
                    if(bits[i][7 - j] == 1)
                        point(x, y, *std::get<0>(pen_and_paper[i][floor(y / 8)]));
                    else
                        point(x, y, *std::get<1>(pen_and_paper[i][floor(y / 8)]));
                }
            }
        }
        update();
    }
};

namespace gfx {
    std::map<const std::string, const std::string> character_set = {
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

    std::map<std::string, std::map<int, std::map<int, std::unique_ptr<TextTexture>>>> text_textures;

    void draw_text(std::string text, const Coords& xy, const RGB &fg, const RGB &bg) {
        for(int i = 0; i < text.size(); ++i) {
            const auto letter = text.substr(i, 1);
            const auto fg_index = fg.get_24_bit_index(); 
            const auto bg_index = bg.get_24_bit_index(); 
            if(!text_textures[letter][fg_index][bg_index])
                text_textures[letter][fg_index][bg_index] = std::unique_ptr<TextTexture>(new TextTexture(character_set[letter], fg, bg));
            sdl::draw(*text_textures[letter][fg_index][bg_index], (xy.x + i) * 8, xy.y * 8);
        }
    }

    void draw_press_any_key(const RGB &fg, const RGB &bg) {
        draw_text("         PRESS ANY KEY          ", {0, 22}, fg, bg);
    }

    void draw_press_keys_1_to_4(const RGB &fg, const RGB &bg) {
        draw_text("       PRESS KEYS 1 TO 4        ", {0, 22}, fg, bg);
    }

    void clear_status_line() {
        const int text_height = 16 * 3;
        sdl::clear(0, WINDOW_HEIGHT - text_height, WINDOW_WIDTH, text_height);
    }

    void draw_illusion_question() {
        clear_status_line();
        draw_text("ILLUSION? (PRESS Y OR N)", {0, 22}, bright_white);
    }

    void draw_spell_succeeds() {
        clear_status_line();
        draw_text("SPELL SUCCEEDS", {0, 22}, bright_white);
    }

    void draw_spell_fails() {
        clear_status_line();
        draw_text("SPELL FAILS", {0, 22}, bright_purple);
    }

    void draw_out_of_range(const RGB &rgb) {
        clear_status_line();
        draw_text("OUT OF RANGE", {0, 22}, rgb);
    }

    void no_line_of_sight() {
        clear_status_line();
        draw_text("NO LINE OF SIGHT", {0, 22}, bright_cyan);
    }

    void draw_turn_text(const std::string& name) {
        clear_status_line();
        draw_text(name + "'S TURN", {0, 22}, bright_yellow);
    }

    void draw_movement_range(const int& movement, const bool& flying) {
        const std::string movement_text = std::to_string(movement);
        clear_status_line();
        draw_text("MOVEMENT RANGE=", {0, 22}, bright_green);
        draw_text(movement_text , {15, 22}, bright_yellow);
        if(flying)
            draw_text(" (FLYING)", {static_cast<int>(movement_text.size()) + 15, 22}, bright_cyan);
    }

    void draw_movement_points_left(const int& movement) {
        const std::string movement_text = std::to_string(movement);
        clear_status_line();
        draw_text("MOVEMENT POINTS LEFT=", {0, 22}, bright_green);
        draw_text(movement_text , {21, 22}, bright_yellow);
    }

    void draw_ranged_combat(const int& range) {
        const std::string range_text = std::to_string(range);
        clear_status_line();
        draw_text("RANGED COMBAT,RANGE=", {0, 22}, bright_green);
        draw_text(range_text , {20, 22}, bright_yellow);
    }

    void draw_engaged_to_enemy() {
        clear_status_line();
        draw_text("ENGAGED TO ENEMY", {0, 22}, bright_yellow);
    }

    void alignment_text(const int &alignment, const Coords& xy) {
        if (alignment < 0)
            draw_text("(CHAOS " + std::to_string(std::abs(alignment)) + ")", xy, bright_purple);
        else if (alignment > 0)
            draw_text("(LAW " + std::to_string(alignment) + ")", xy, bright_cyan);
    }

    const std::string corner_bytes = "ffffffffeaaff55bd087e42bd087ea0bd057e10bd427e10bdaaff557ffffffff";
    const std::string edge_bytes = "d057e40bd027ea0bd057e40bd027ea0bff00ff11aa44551188aa225588ff00ff";

    std::map<int, std::map<int, std::unique_ptr<SpriteTexture>>> corner_textures;
    std::map<int, std::map<int, std::unique_ptr<SpriteTexture>>> edge_textures;

    void draw_border(const RGB &fg, const RGB &bg) {
        sdl::clear();
        const auto fg_index = fg.get_24_bit_index(); 
        const auto bg_index = bg.get_24_bit_index(); 
        if(!corner_textures[fg_index][bg_index])
            corner_textures[fg_index][bg_index] = std::unique_ptr<SpriteTexture>(new SpriteTexture(corner_bytes, bg, fg));
        if(!edge_textures[fg_index][bg_index])
            edge_textures[fg_index][bg_index] = std::unique_ptr<SpriteTexture>(new SpriteTexture(edge_bytes, bg, fg));
        sdl::draw(*corner_textures[fg_index][bg_index], 0, 0, 0, 0, 8, 8);
        sdl::draw(*corner_textures[fg_index][bg_index], 31 * 8, 0, 8, 0, 8, 8);
        sdl::draw(*corner_textures[fg_index][bg_index], 0, 21 * 8, 0, 8, 8, 8);
        sdl::draw(*corner_textures[fg_index][bg_index], 31 * 8, 21 * 8, 8, 8, 8, 8);
        for(int x = 1; x <= 30; ++x) {
            sdl::draw(*edge_textures[fg_index][bg_index], x * 8, 0, 0, 8, 8, 8);
            sdl::draw(*edge_textures[fg_index][bg_index], x * 8, 21 * 8, 8, 8, 8, 8);
        }
        for(int y = 1; y <= 20; ++y) {
            sdl::draw(*edge_textures[fg_index][bg_index], 0, y * 8, 0, 0, 8, 8);
            sdl::draw(*edge_textures[fg_index][bg_index], 31 * 8, y * 8, 8, 0, 8, 8);
        }
    }

    void draw_title_border() {
        draw_border(bright_purple, bright_red);
    }

    void draw_menu_border() {
        draw_border(bright_yellow, bright_red);
        draw_press_keys_1_to_4(bright_yellow, bright_red);
    }

    void draw_player_entry_border() {
        draw_border(bright_cyan, bright_blue);
    }

    void draw_spell_border() {
        draw_border(bright_cyan, bright_blue);
        draw_press_any_key(bright_cyan, bright_blue);
    }

    void draw_info_border() {
        draw_border(bright_black, bright_green);
        draw_press_any_key(black, bright_green);
    }

    void draw_arena_border(const RGB& rgb) {
        draw_border(black, rgb);
    }

    const std::string spell_bytes = "ffff8001800187e18c3188118c0187e18031801188118c3187e180018001ffff";
    const std::string box_bytes = "ffffffffc003c003c003c003c003c003c003c003c003c003c003c003ffffffff";
    const std::string wings_bytes = "c003e007700e300c000000001c383e7c63c6018001800240300c700ee007c003";
    const std::string ranged_bytes = "f81fe007f00fb81d98190000000000000000000000009819b81df00fe007f83f";

    std::map <int, std::unique_ptr<SpriteTexture>> spell_textures;
    std::map <int, std::unique_ptr<SpriteTexture>> box_textures;
    std::map <int, std::unique_ptr<SpriteTexture>> wings_textures;
    std::map <int, std::unique_ptr<SpriteTexture>> ranged_textures;

    void draw_cursor_texture(const std::string &bytes, std::map<int, std::unique_ptr<SpriteTexture>> &textures, const RGB &rgb, const Coords& xy) {
        const auto rgb_index = rgb.get_24_bit_index(); 
        if(!textures[rgb_index])
            textures[rgb_index] = std::unique_ptr<SpriteTexture>(new SpriteTexture(bytes, rgb));
        sdl::draw(*textures[rgb_index], 8 + xy.x * 16, 8 + xy.y * 16);
    }

    void draw_spell_cursor(const RGB &rgb, const Coords& xy) {
        draw_cursor_texture(spell_bytes, spell_textures, rgb, xy);
    }

    void draw_box_cursor(const RGB &rgb, const Coords& xy) {
        draw_cursor_texture(box_bytes, box_textures, rgb, xy);
    }

    void draw_wings_cursor(const RGB &rgb, const Coords& xy) {
        draw_cursor_texture(wings_bytes, wings_textures, rgb, xy);
    }

    void draw_ranged_cursor(const RGB &rgb, const Coords& xy) {
        draw_cursor_texture(ranged_bytes, ranged_textures, rgb, xy);
    }

    #define CORPSE_INDEX 4

    const std::vector<std::string> king_cobra_bytes = {
        "00000000000000000180008000800080008003000460041003e0000000000000",
        "00000000000000000000030001000080008003000460041003e0000000000000",
        "00000000000000000000000007000880008003000460041003e0000000000000",
        "00000000000000000000030001000080008003000460041003e0000000000000",
        "0000000000000000000000000000000000001e00203818c40700000000000000"
    };

    const std::vector<std::string> dire_wolf_bytes = {
        "0000000000000000080078081c0827f00ff00a28094805500000000000000000",
        "0000000000000000080078041c0827f00ff00c28142814240000000000000000",
        "0000000000000000080078003c0c27f00ff00c1c281208080000000000000000",
        "0000000000000000080078041c0827f00ff00c28142814240000000000000000",
        "00000000000000000000000000000400058a220c1bf07ffe0000000000000000"
    };

    const std::vector<std::string> goblin_bytes = {
        "0000000000006180718037e01ff01bd803c807e00660066006600ee000000000",
        "000030003000318031801fe01ff003d803c807e00660066006600ee000000000",
        "000006000c000d8019801fe00ff003d803c807e00660066006600ee000000000",
        "000030003000318031801fe01ff003d803c807e00660066006600ee000000000",
        "0000000000000000000000000000000000000000000011e23bfe3ffe00000000"
    };

    const std::vector<std::string> crocodile_bytes = {
        "0000000000000000000000000000000c000201fc0ff81f243504010000000000",
        "0000000000000000000000000000000c00021dfc7ff81f243504010000000000",
        "0000000000000000000000000000000c70021dfc0ff81f243504010000000000",
        "0000000000000000000000000000000c00021dfc7ff81f243504010000000000",
        "000000000000000000000000000000000010200c19260ffe7ffc000000000000"
    };

    const std::vector<std::string> faun_bytes = {
        "0000000001400180018023c015a0099003c003c0036001200120024000000000",
        "000000000140118011800bc00da0019003c003c0036001200120024000000000",
        "000000000540058009800fc001a0019003c003c0036001200120024000000000",
        "000000000140118011800bc00da0019003c003c0036001200120024000000000",
        "00000000000000000000000000000000000000000004013837e27ffc00000000"
    };

    const std::vector<std::string> lion_bytes = {
        "000000000000000018007e007ff03ff81ffc0f2c122422440408000000000000",
        "00000000000018007c003e007ff03ff81ffc0f2c122422440408000000000000",
        "0000000000002c001c001e003ff01ff81ffc0f2c122422440408000000000000",
        "00000000000018007c003e007ff03ff81ffc0f2c122422440408000000000000",
        "0000000000000000000000000000042209921a147ffc7ffc3ff81c0000000000"
    };

    const std::vector<std::string> elf_bytes = {
        "00000000020006c006c03fc006c006c002c000c0014001400240024000000000",
        "00000000020005c005c01fe005c005c002c000c0014001400240024000000000",
        "00000000020005c004c00ff004c005c002c000c0014001400240024000000000",
        "00000000020004c004c0f7f004e004c002c000c0014001400240024000000000",
        "000000000000000000000000000000000000000003e004901be41ffc00000000"
    };

    const std::vector<std::string> orc_bytes = {
        "0000000000800180018040b02378157809b001c00240024006c0000000000000",
        "0000000000801180118010b013780d7801b001c00240024006c0000000000000",
        "0000000002800580058008b00f78017801b001c00240024006c0000000000000",
        "0000000000801180118010b013780d7801b001c00240024006c0000000000000",
        "000000000000000000000000000000000000000001c01be41ffc000000000000"
    };

    const std::vector<std::string> bear_bytes = {
        "000000000300078003c001c00de007e003e001e003e003600360036000000000",
        "000000000300078003c001c007e01fe011e001e003e003600360036000000000",
        "000000000300078003c001c003e007e00de009e003e003600360036000000000",
        "000000000300078003c001c003e007e00de009e003e003600360036000000000",
        "00000000000000000000000000000000000000c027807fe27ffe3ffe00000000"
    };

    const std::vector<std::string> gorilla_bytes = {
        "000000001980338033803ef81ef8072003c003c007e00e700c301c3800000000",
        "00000000018007e00db01ff80ff007e003c003c007e00e700c301c3800000000",
        "00000000019801cc01cc1f7c1f7804e003c003c007e00e700c301c3800000000",
        "00000000018007e00db01ff80ff007e003c003c007e00e700c301c3800000000",
        "0000000000000000000000000000000000000000018037e23ffe3ffe00000000"
    };

    const std::vector<std::string> ogre_bytes = {
        "0000000000000180018007e04ff0fbd8f3c843e007f0063006380e0000000000",
        "000000000000618071803fe01ff003d803c803c007e0066006600e7000000000",
        "0000000018003d8019801fe00ff003d803c807c00fe00c601c60007000000000",
        "000000000000618071803fe01ff003d803c803c007e0066006600e7000000000",
        "000000000000000000000000000000000000000001801be41ffc1ffc00000000"
    };

    const std::vector<std::string> hydra_bytes = {
        "000036c012401240ca984a883f3007c00e001cf8193c180c1c1c0ff807f00000",
        "00000d80648024b01290ca907f2007c00e001cf8193c180c1c1c0ff807f00000",
        "000003001960092064a032a0cf4077800e001cf8193c180c1c1c0ff807f00000",
        "00000d80648024b01290ca907f2007c00e001cf8193c180c1c1c0ff807f00000",
        "00000000000000000000050056207fe81ff8380071fc627e60067ffe3ff80000"
    };

    const std::vector<std::string> giant_rat_bytes = {
        "000000000000000000000000000c009200611f017f82fffc1180204000000000",
        "00000000000000000000000c0012002100211f017f82fffc1080108000000000",
        "00000000000000000000000c0012001100091f017f82fffc11000a0000000000",
        "00000000000000000000000c0012002100211f017f82fffc1080108000000000",
        "00000000000000000000000000000000080010c01f00ff807ffe000101fe0200"
    };

    const std::vector<std::string> giant_bytes = {
        "00c000c003f007f807f80fecfde4f9e603f203f0033003300330033003300770",
        "00c000c043f0e7f87ff83dec19e401e603f203f0033003300330033003300770",
        "30c030c033f037f83ff81dec01e401e603f203f0033003300330033003300770",
        "00c000c043f0e7f87ff83dec19e401e603f203f0033003300330033003300770",
        "000000000000000000000000000000000000000000000fc1dfffffff00000000"
    };

    const std::vector<std::string> horse_bytes = {
        "000000000000000030007800dfec1ff20ff01428142812480a50000000000000",
        "000000000000000030007800dfec1ff20ff01428142824240820000000000000",
        "000000000000000030007800dfec1ff20ff0342c442208100810000000000000",
        "000000000000000030007800dfec1ff20ff01428142824240820000000000000",
        "0000000000000000000000000000020904ca150a33fc7ffc7ffb000000000000"
    };

    const std::vector<std::string> unicorn_bytes = {
        "000000004000200018003c006ff60ff907f80a140a1409240528000000000000",
        "000000004000200018003c006ff70ff807f80a140a1412120410000000000000",
        "000000004000200018003c016ff60ff807f81a16221104080408000000000000",
        "000000004000200018003c006ff70ff807f80a140a1412120410000000000000",
        "00000000000000000000000000000109026a8a8a59fc3ffc3ffb000000000000"
    };

    const std::vector<std::string> centaur_bytes = {
        "000000002c004c0044007c004e004ff62ff907f80a140a140924052800000000",
        "000000002c004c0044007c004e004ff62ff907f80a140a141212041000000000",
        "000000002c004c0044007c004e004ff62ff907f81a1622110408040800000000",
        "000000002c004c0044007c004e004ff62ff907f80a140a141212041000000000",
        "0000000000000000000000000000020904ca050a03fc6ffc7f97000000000000"
    };

    const std::vector<std::string> pegasus_bytes = {
        "000000fc01f801e031807b00dfec1ff20ff01428142812480a50000000000000",
        "000000000000000030fc7bc0dfee1ff00ff01428142824240820000000000000",
        "000000000000000030007b00dfee1ff00ff035fc443e08100810000000000000",
        "000000000000000030fc7bc0dfee1ff00ff01428142824240820000000000000",
        "0000000000000000000000000000021204d4151433f87ff87ff603e000f80000"
    };

    const std::vector<std::string> gryphon_bytes = {
        "0000008003c030c6798f0d9807f003e003f003d0025002500240000000000000",
        "0000010007800380018001800ff87fff03f003d0025002500240000000000000",
        "000002000f0003000580018007f00ff81ffc33d6425102500240000000000000",
        "0000010007800380018001800ff87fff03f003d0025002500240000000000000",
        "000000000000000000000000000011083a08621873f03ff81ffe07c000000000"
    };

    const std::vector<std::string> manticore_bytes = {
        "078008fc08f065c565823b0d3ff03ff81ffc0f2c122422440408000000000000",
        "000000000000670568fe3bcd3ff03ff81ffc0f2c122422440408000000000000",
        "000000000000600560023b0d3ff03ff81ffc0f7c122422440408000000000000",
        "000000000000670568fe3bcd3ff03ff81ffc0f2c122422440408000000000000",
        "0000000000000000000000000000020414320c0a07fc6ffc7fff03e000000000"
    };

    const std::vector<std::string> bat_bytes = {
        "0000000000000000000000000c60139001000000000000000000000000000000",
        "00000000000000000000000000000fe011100000000000000000000000000000",
        "0000000000000000000000000000038005401830000000000000000000000000",
        "00000000000000000000000000000fe011100000000000000000000000000000",
        "000000000000000000000000000000000000000000000000010013900c600000"
    };

    const std::vector<std::string> green_dragon_bytes = {
        "0000f1c018e03cf04cf019f831e473007a707ff83ffe1ff9142d142524440808",
        "600018000c403e7006780cfc38fe71c07b707ff83ffe1ff9142d142524440808",
        "1800060003001f0003100e3c387e70f87bf07ff83ffe1ff9142d142524440808",
        "600018000c403e7006780cfc38fe71c07b707ff83ffe1ff9142d142524440808",
        "00000000000000000000000000003c007308e828c59ae7f97ffd3ffe00000000"
    };

    const std::vector<std::string> red_dragon_bytes = {
        "0000000000001000f88018c031e231f23b8a1e020fe117f115f92479083a01fc",
        "0000700008007c800ce018f018fd1dc10f011fc227e229f208790079003a01fc",
        "30000c0006200e383c3e0c780ec21f8223c245e108f108f9007a007a003a01fc",
        "0000700008007c800ce018f018fd1dc10f011fc227e229f208790079003a01fc",
        "00000000000000000000000000003c007308e828c59ae7f97ffd3ffe00000000"
    };

    const std::vector<std::string> golden_dragon_bytes = {
        "008017d030d8719c719cfb3efb3ecf6687f203f803d4025204520442000400f8",
        "1a0007001b004304630c731cfb3eff7ec7f683fa03d4025204520442000400f8",
        "68001c001c00660006008302c306f31efffefffe63dc425604520442000400f8",
        "1a0007001b004304630c731cfb3eff7ec7f683fa03d4025204520442000400f8",
        "000000000000000000000000000000000492a50aa3fc7fff7fff03c000f80000"
    };

    const std::vector<std::string> harpy_bytes = {
        "0000000001e0027c1a781ae00fc00700078003c001f002400440008000000000",
        "000000000000000018001bc00ff807c0078003c001f002400440008000000000",
        "0000000000000000180018000f00078007c003f801f002400440008000000000",
        "000000000000000018001bc00ff807c0078003c001f002400440008000000000",
        "000000000000000000000000000000000000012000a033f03ffc078001e00000"
    };

    const std::vector<std::string> eagle_bytes = {
        "000000000000000038006c7807ec038006800280000000000000000000000000",
        "00000000000000000000000003801fe07ef8029e000000000000000000000000",
        "00000000000000000000000003800fe01ef03ab8301c000c0000000000000000",
        "00000000000000000000000003801fe07ef8029e000000000000000000000000",
        "00000000000000000000000000000000000070003d000f400f8011e000780000"
    };

    const std::vector<std::string> vampire_bytes = {
        "00000000000000000180018003c003c007e007e007f00ff00a48024000000000",
        "00000000000000000180018003c003e003f007f007f8065c0240024000000000",
        "00000000000000000180018003c003e003f003f807cc06440240024000000000",
        "00000000000000000180018003c003e003f007f007f8065c0240024000000000",
    };

    const std::vector<std::string> ghost_bytes = {
        "00000004400c47ce693ef93effe3ce43ce414f6045e1056140d0028002100044",
        "00000000000407cc093c793effeefe47ce400e40476043a10421459000800210",
        "00000000000007c00924193c3ffefe7ece63ce404e40076043a044a104004090",
        "00000000000407c4492c793eff9afe668e430e41074003e00040008044010420",
    };

    const std::vector<std::string> spectre_bytes = {
        "0000000000004060206011f00af007e000e000f001b001100110031800000000",
        "0000000000004060206011f00af007e000e000f001b001100110031800000000",
        "0000000000004060206011f00af007e000e000f001b001100110031800000000",
        "0000000000004060206011f00af007e000e000f001b001100110031800000000",
    };

    const std::vector<std::string> wraith_bytes = {
        "00000000000000c000c011e00ad004d000c00120012002100210000000000000",
        "0000000001e001203b302e1835281b280f3802d006d805e80528073800000000",
        "000003f002107e18440c40044004600430041c0c0804080408c408c40ffc0000",
        "07f80408fc0c80c680c291e28ad284d2c0c26122312212121212100210021ffe",
    };

    const std::vector<std::string> skeleton_bytes = {
        "000001800188008803f005c009c0f08001c00120011001100120014003200000",
        "000001800180008003fc45c029c0108001c00140022002200220014003600000",
        "000001801180108017f019c801c8008001c00240044004400240014002600000",
        "000001800180008003fc45c029c0108001c00140022002200220014003600000",
    };

    const std::vector<std::string> zombie_bytes = {
        "000000000180018000801f8001800180018002800480044004200c4000000000",
        "000000000180018000801f800180018001800280028002700210060000000000",
        "000000000180018000801f800180018001800100010001e00120030000000000",
        "000000000180018000801f800180018001800280048002800180038000000000",
    };

    const std::vector<std::string> blob_bytes = {
        "018003c007e007e003c033cc7fffffffffcc7fe039fc03be079e0f8c0f000600",
        "0000018003c003c003c0019819fe7ffe7fe019f001be039e078c078003000000",
        "00000000018003c003c0019819fc3ffc3fe019f003bc079c0700030000000000",
        "0000018003c003c003c0019819fe7ffe7fe019f001be039e078c078003000000",
    };

    const std::vector<std::string> fire_bytes = {
        "2004208421000300050004800444024812482254329254924b6a279c1ff80000",
        "0000008001000000010001800158415462545252928a94924b6a24941db80000",
        "010000002004300410181830285048488a249312929294924b6a279c1ff80000",
        "0000000020043004110811800158415462545252928a94924b6a24941db80000",
    };

    const std::vector<std::string> magic_wood_bytes = { 
        "028015502aa855542aa855542aa815d00ba0018001800180018003c007e00000",
        "028015502aa855542aa855542aa815d00ba0018001800180018003c007e00000",
        "028015502aa855542aa855542aa815d00ba0018001800180018003c007e00000",
        "028015502aa855542aa855542aa815d00ba0018001800180018003c007e00000",
    };

    const std::vector<std::string> shadow_wood_bytes = {
        "0500022024b814400c8856a623a825f0098809840180018001a003d005480000",
        "0500022024b814400c8856a623a825f0098809840180018001a003d005480000",
        "0500022024b814400c8856a623a825f0098809840180018001a003d005480000",
        "0500022024b814400c8856a623a825f0098809840180018001a003d005480000",
    };

    const std::vector<std::string> magic_castle_bytes = {
        "0a500e700a500a500a50ce73ce73ce73ce7fffff7ffe366c366c366c3e7c7ffe",
        "0a500e700a500a500a50ce73ce73ce73ce7fffff7ffe366c366c366c3e7c7ffe",
        "0a500e700a500a500a50ce73ce73ce73ce7fffff7ffe366c366c366c3e7c7ffe",
        "0a500e700a500a500a50ce73ce73ce73ce7fffff7ffe366c366c366c3e7c7ffe",
    };

    const std::vector<std::string> dark_citadel_bytes = {
        "10081008381c381c7c3e7c3e381c399c3bdc7ffeffffce73cc33cc33fc3ffc3f",
        "10081008381c381c7c3e7c3e381c399c3bdc7ffeffffce73cc33cc33fc3ffc3f",
        "10081008381c381c7c3e7c3e381c399c3bdc7ffeffffce73cc33cc33fc3ffc3f",
        "10081008381c381c7c3e7c3e381c399c3bdc7ffeffffce73cc33cc33fc3ffc3f",
    };

    const std::vector<std::string> wall_bytes = {
        "0000eeeeeeee0000bbbbbbbb0000eeeeeeee0000bbbbbbbb0000eeeeeeee0000",
        "0000eeeeeeee0000bbbbbbbb0000eeeeeeee0000bbbbbbbb0000eeeeeeee0000",
        "0000eeeeeeee0000bbbbbbbb0000eeeeeeee0000bbbbbbbb0000eeeeeeee0000",
        "0000eeeeeeee0000bbbbbbbb0000eeeeeeee0000bbbbbbbb0000eeeeeeee0000",
    };

    const std::vector<std::pair<const RGB *, const RGB *>> king_cobra_rgb = {
        {&bright_green, &black},
        {&bright_green, &black},
        {&bright_green, &black},
        {&bright_green, &black},
        {&bright_green, &black}    
    };

    const std::vector<std::pair<const RGB *, const RGB *>> dire_wolf_rgb = {
        {&bright_yellow, &black},
        {&bright_yellow, &black},
        {&bright_yellow, &black},
        {&bright_yellow, &black},
        {&bright_yellow, &black}
    };

    const std::vector<std::pair<const RGB *, const RGB *>> goblin_rgb = {
        {&bright_purple, &black},
        {&bright_purple, &black},
        {&bright_purple, &black},
        {&bright_purple, &black},
        {&bright_purple, &black}
    };

    const std::vector<std::pair<const RGB *, const RGB *>> crocodile_rgb = {
        {&bright_green, &black},
        {&bright_green, &black},
        {&bright_green, &black},
        {&bright_green, &black},
        {&bright_green, &black}
    };

    const std::vector<std::pair<const RGB *, const RGB *>> faun_rgb = {
        {&bright_white, &black},
        {&bright_white, &black},
        {&bright_white, &black},
        {&bright_white, &black},
        {&bright_white, &black}
    };

    const std::vector<std::pair<const RGB *, const RGB *>> lion_rgb = {
        {&bright_yellow, &black},
        {&bright_yellow, &black},
        {&bright_yellow, &black},
        {&bright_yellow, &black},
        {&bright_yellow, &black}
    };

    const std::vector<std::pair<const RGB *, const RGB *>> elf_rgb = {
        {&bright_green, &black},
        {&bright_green, &black},
        {&bright_green, &black},
        {&bright_green, &black},
        {&bright_green, &black}
    };

    const std::vector<std::pair<const RGB *, const RGB *>> orc_rgb = {
        {&bright_cyan, &black},
        {&bright_cyan, &black},
        {&bright_cyan, &black},
        {&bright_cyan, &black},
        {&bright_cyan, &black}
    };

    const std::vector<std::pair<const RGB *, const RGB *>> bear_rgb = {
        {&bright_red, &black},
        {&bright_red, &black},
        {&bright_red, &black},
        {&bright_red, &black},
        {&bright_red, &black}
    };

    const std::vector<std::pair<const RGB *, const RGB *>> gorilla_rgb = {
        {&yellow, &black},
        {&yellow, &black},
        {&yellow, &black},
        {&yellow, &black},
        {&yellow, &black}
    };

    const std::vector<std::pair<const RGB *, const RGB *>> ogre_rgb = {
        {&bright_red, &black},
        {&bright_red, &black},
        {&bright_red, &black},
        {&bright_red, &black},
        {&bright_red, &black}
    };

    const std::vector<std::pair<const RGB *, const RGB *>> hydra_rgb = {
        {&bright_green, &black},
        {&bright_green, &black},
        {&bright_green, &black},
        {&bright_green, &black},
        {&bright_green, &black}
    };

    const std::vector<std::pair<const RGB *, const RGB *>> giant_rat_rgb = {
        {&white, &black},
        {&white, &black},
        {&white, &black},
        {&white, &black},
        {&white, &black}
    };

    const std::vector<std::pair<const RGB *, const RGB *>> giant_rgb = {
        {&cyan, &black},
        {&cyan, &black},
        {&cyan, &black},
        {&cyan, &black},
        {&cyan, &black}
    };

    const std::vector<std::pair<const RGB *, const RGB *>> horse_rgb = {
        {&yellow, &black},
        {&yellow, &black},
        {&yellow, &black},
        {&yellow, &black},
        {&yellow, &black}
    };

    const std::vector<std::pair<const RGB *, const RGB *>> unicorn_rgb = {
        {&bright_cyan, &black},
        {&bright_cyan, &black},
        {&bright_cyan, &black},
        {&bright_cyan, &black},
        {&bright_cyan, &black}
    };

    const std::vector<std::pair<const RGB *, const RGB *>> centaur_rgb = {
        {&yellow, &black},
        {&yellow, &black},
        {&yellow, &black},
        {&yellow, &black},
        {&yellow, &black}
    };

    const std::vector<std::pair<const RGB *, const RGB *>> pegasus_rgb = {
        {&bright_white, &black},
        {&bright_white, &black},
        {&bright_white, &black},
        {&bright_white, &black},
        {&bright_white, &black}
    };

    const std::vector<std::pair<const RGB *, const RGB *>> gryphon_rgb = {
        {&white, &black},
        {&white, &black},
        {&white, &black},
        {&white, &black},
        {&white, &black}
    };

    const std::vector<std::pair<const RGB *, const RGB *>> manticore_rgb = {
        {&bright_yellow, &black},
        {&bright_yellow, &black},
        {&bright_yellow, &black},
        {&bright_yellow, &black},
        {&bright_yellow, &black}
    };

    const std::vector<std::pair<const RGB *, const RGB *>> bat_rgb = {
        {&bright_white, &black},
        {&bright_white, &black},
        {&bright_white, &black},
        {&bright_white, &black},
        {&bright_white, &black}
    };

    const std::vector<std::pair<const RGB *, const RGB *>> green_dragon_rgb = {
        {&bright_green, &black},
        {&bright_green, &black},
        {&bright_green, &black},
        {&bright_green, &black},
        {&bright_green, &black}
    };

    const std::vector<std::pair<const RGB *, const RGB *>> red_dragon_rgb = {
        {&bright_red, &black},
        {&bright_red, &black},
        {&bright_red, &black},
        {&bright_red, &black},
        {&bright_red, &black}
    };

    const std::vector<std::pair<const RGB *, const RGB *>> golden_dragon_rgb = {
        {&bright_yellow, &black},
        {&bright_yellow, &black},
        {&bright_yellow, &black},
        {&bright_yellow, &black},
        {&bright_yellow, &black}
    };

    const std::vector<std::pair<const RGB *, const RGB *>> harpy_rgb = {
        {&bright_cyan, &black},
        {&bright_cyan, &black},
        {&bright_cyan, &black},
        {&bright_cyan, &black},
        {&bright_cyan, &black}
    };

    const std::vector<std::pair<const RGB *, const RGB *>> eagle_rgb = {
        {&bright_yellow, &black},
        {&bright_yellow, &black},
        {&bright_yellow, &black},
        {&bright_yellow, &black},
        {&bright_yellow, &black}
    };

    const std::vector<std::pair<const RGB *, const RGB *>> vampire_rgb = {
        {&bright_red, &black},
        {&bright_red, &black},
        {&bright_red, &black},
        {&bright_red, &black}
    };

    const std::vector<std::pair<const RGB *, const RGB *>> ghost_rgb = {
        {&cyan, &black},
        {&bright_cyan, &black},
        {&cyan, &black},
        {&bright_cyan, &black}
    };

    const std::vector<std::pair<const RGB *, const RGB *>> spectre_rgb = {
        {&bright_white, &black},
        {&bright_cyan, &black},
        {&bright_purple, &black},
        {&bright_blue, &black}
    };

    const std::vector<std::pair<const RGB *, const RGB *>> wraith_rgb = {
        {&bright_cyan, &black},
        {&bright_cyan, &black},
        {&bright_cyan, &black},
        {&bright_cyan, &black}
    };

    const std::vector<std::pair<const RGB *, const RGB *>> skeleton_rgb = {
        {&bright_white, &black},
        {&bright_white, &black},
        {&bright_white, &black},
        {&bright_white, &black}
    };

    const std::vector<std::pair<const RGB *, const RGB *>> zombie_rgb = {
        {&bright_cyan, &black},
        {&bright_cyan, &black},
        {&bright_cyan, &black},
        {&bright_cyan, &black}
    };

    const std::vector<std::pair<const RGB *, const RGB *>> blob_rgb = {
        {&bright_green, &black},
        {&bright_green, &black},
        {&bright_green, &black},
        {&bright_green, &black}
    };

    const std::vector<std::pair<const RGB *, const RGB *>> fire_rgb = {
        {&bright_yellow, &black},
        {&bright_yellow, &black},
        {&bright_yellow, &black},
        {&bright_yellow, &black}
    };

    const std::vector<std::pair<const RGB *, const RGB *>> magic_wood_rgb = {
        {&green, &black},
        {&bright_green, &black},
        {&yellow, &black},
        {&bright_yellow, &black}
    };

    const std::vector<std::pair<const RGB *, const RGB *>> shadow_wood_rgb = {
        {&cyan, &black},
        {&cyan, &black},
        {&bright_cyan, &black},
        {&bright_cyan, &black}
    };

    const std::vector<std::pair<const RGB *, const RGB *>> magic_castle_rgb = {
        {&bright_cyan, &black},
        {&cyan, &black},
        {&bright_cyan, &black},
        {&cyan, &black}
    };

    const std::vector<std::pair<const RGB *, const RGB *>> dark_citadel_rgb = {
        {&bright_purple, &black},
        {&bright_purple, &black},
        {&purple, &black},
        {&purple, &black}
    };

    const std::vector<std::pair<const RGB *, const RGB *>> wall_rgb = {
        {&bright_red, &bright_yellow},
        {&bright_red, &bright_yellow},
        {&bright_red, &bright_yellow},
        {&bright_red, &bright_yellow}
    };

    std::map<int, std::unique_ptr<SpriteTexture>> king_cobra_textures;
    std::map<int, std::unique_ptr<SpriteTexture>> dire_wolf_textures;
    std::map<int, std::unique_ptr<SpriteTexture>> goblin_textures;
    std::map<int, std::unique_ptr<SpriteTexture>> crocodile_textures;
    std::map<int, std::unique_ptr<SpriteTexture>> faun_textures;
    std::map<int, std::unique_ptr<SpriteTexture>> lion_textures;
    std::map<int, std::unique_ptr<SpriteTexture>> elf_textures;
    std::map<int, std::unique_ptr<SpriteTexture>> orc_textures;
    std::map<int, std::unique_ptr<SpriteTexture>> bear_textures;
    std::map<int, std::unique_ptr<SpriteTexture>> gorilla_textures;
    std::map<int, std::unique_ptr<SpriteTexture>> ogre_textures;
    std::map<int, std::unique_ptr<SpriteTexture>> hydra_textures;
    std::map<int, std::unique_ptr<SpriteTexture>> giant_rat_textures;
    std::map<int, std::unique_ptr<SpriteTexture>> giant_textures;
    std::map<int, std::unique_ptr<SpriteTexture>> horse_textures;
    std::map<int, std::unique_ptr<SpriteTexture>> unicorn_textures;
    std::map<int, std::unique_ptr<SpriteTexture>> centaur_textures;
    std::map<int, std::unique_ptr<SpriteTexture>> pegasus_textures;
    std::map<int, std::unique_ptr<SpriteTexture>> gryphon_textures;
    std::map<int, std::unique_ptr<SpriteTexture>> manticore_textures;
    std::map<int, std::unique_ptr<SpriteTexture>> bat_textures;
    std::map<int, std::unique_ptr<SpriteTexture>> green_dragon_textures;
    std::map<int, std::unique_ptr<SpriteTexture>> red_dragon_textures;
    std::map<int, std::unique_ptr<SpriteTexture>> golden_dragon_textures;
    std::map<int, std::unique_ptr<SpriteTexture>> harpy_textures;
    std::map<int, std::unique_ptr<SpriteTexture>> eagle_textures;
    std::map<int, std::unique_ptr<SpriteTexture>> vampire_textures;
    std::map<int, std::unique_ptr<SpriteTexture>> ghost_textures;
    std::map<int, std::unique_ptr<SpriteTexture>> spectre_textures;
    std::map<int, std::unique_ptr<SpriteTexture>> wraith_textures;
    std::map<int, std::unique_ptr<SpriteTexture>> skeleton_textures;
    std::map<int, std::unique_ptr<SpriteTexture>> zombie_textures;
    std::map<int, std::unique_ptr<SpriteTexture>> blob_textures;
    std::map<int, std::unique_ptr<SpriteTexture>> fire_textures;
    std::map<int, std::unique_ptr<SpriteTexture>> magic_wood_textures;
    std::map<int, std::unique_ptr<SpriteTexture>> shadow_wood_textures;
    std::map<int, std::unique_ptr<SpriteTexture>> magic_castle_textures;
    std::map<int, std::unique_ptr<SpriteTexture>> dark_citadel_textures;
    std::map<int, std::unique_ptr<SpriteTexture>> wall_textures;

    void draw_creation_texture(const std::vector<std::string> &creation_bytes, std::map<int, std::unique_ptr<SpriteTexture>> &creation_textures, const Coords& xy, const int &sprite_index, const std::vector<std::pair<const RGB *, const RGB *>> &rgbs) {
        if(!creation_textures[sprite_index])
            creation_textures[sprite_index] = std::unique_ptr<SpriteTexture>(new SpriteTexture(creation_bytes[sprite_index], *std::get<0>(rgbs[sprite_index]), *std::get<1>(rgbs[sprite_index])));
        sdl::draw(*creation_textures[sprite_index], 8 + xy.x * 16, 8 + xy.y * 16);
    }

    void draw_king_cobra(const Coords& xy, const int &sprite_index) {
        draw_creation_texture(king_cobra_bytes, king_cobra_textures, xy, sprite_index, king_cobra_rgb);
    }

    void draw_king_cobra_corpse(const Coords& xy) {
        draw_king_cobra(xy, CORPSE_INDEX);
    }

    void draw_dire_wolf(const Coords& xy, const int &sprite_index) {
        draw_creation_texture(dire_wolf_bytes, dire_wolf_textures, xy, sprite_index, dire_wolf_rgb);
    }

    void draw_dire_wolf_corpse(const Coords& xy) {
        draw_dire_wolf(xy, CORPSE_INDEX);
    }

    void draw_goblin(const Coords& xy, const int &sprite_index) {
        draw_creation_texture(goblin_bytes, goblin_textures, xy, sprite_index, goblin_rgb);
    }

    void draw_goblin_corpse(const Coords& xy) {
        draw_goblin(xy, CORPSE_INDEX);
    }

    void draw_crocodile(const Coords& xy, const int &sprite_index) {
        draw_creation_texture(crocodile_bytes, crocodile_textures, xy, sprite_index, crocodile_rgb);
    }

    void draw_crocodile_corpse(const Coords& xy) {
        draw_crocodile(xy, CORPSE_INDEX);
    }

    void draw_faun(const Coords& xy, const int &sprite_index) {
        draw_creation_texture(faun_bytes, faun_textures, xy, sprite_index, faun_rgb);
    }

    void draw_faun_corpse(const Coords& xy) {
        draw_faun(xy, CORPSE_INDEX);
    }

    void draw_lion(const Coords& xy, const int &sprite_index) {
        draw_creation_texture(lion_bytes, lion_textures, xy, sprite_index, lion_rgb);
    }

    void draw_lion_corpse(const Coords& xy) {
        draw_lion(xy, CORPSE_INDEX);
    }

    void draw_elf(const Coords& xy, const int &sprite_index) {
        draw_creation_texture(elf_bytes, elf_textures, xy, sprite_index, elf_rgb);
    }

    void draw_elf_corpse(const Coords& xy) {
        draw_elf(xy, CORPSE_INDEX);
    }

    void draw_orc(const Coords& xy, const int &sprite_index) {
        draw_creation_texture(orc_bytes, orc_textures, xy, sprite_index, orc_rgb);
    }

    void draw_orc_corpse(const Coords& xy) {
        draw_orc(xy, CORPSE_INDEX);
    }

    void draw_bear(const Coords& xy, const int &sprite_index) {
        draw_creation_texture(bear_bytes, bear_textures, xy, sprite_index, bear_rgb);
    }

    void draw_bear_corpse(const Coords& xy) {
        draw_bear(xy, CORPSE_INDEX);
    }

    void draw_gorilla(const Coords& xy, const int &sprite_index) {
        draw_creation_texture(gorilla_bytes, gorilla_textures, xy, sprite_index, gorilla_rgb);
    }

    void draw_gorilla_corpse(const Coords& xy) {
        draw_gorilla(xy, CORPSE_INDEX);
    }

    void draw_ogre(const Coords& xy, const int &sprite_index) {
        draw_creation_texture(ogre_bytes, ogre_textures, xy, sprite_index, ogre_rgb);
    }

    void draw_ogre_corpse(const Coords& xy) {
        draw_ogre(xy, CORPSE_INDEX);
    }

    void draw_hydra(const Coords& xy, const int &sprite_index) {
        draw_creation_texture(hydra_bytes, hydra_textures, xy, sprite_index, hydra_rgb);
    }

    void draw_hydra_corpse(const Coords& xy) {
        draw_hydra(xy, CORPSE_INDEX);
    }

    void draw_giant_rat(const Coords& xy, const int &sprite_index) {
        draw_creation_texture(giant_rat_bytes, giant_rat_textures, xy, sprite_index, giant_rat_rgb);
    }

    void draw_giant_rat_corpse(const Coords& xy) {
        draw_giant_rat(xy, CORPSE_INDEX);
    }

    void draw_giant(const Coords& xy, const int &sprite_index) {
        draw_creation_texture(giant_bytes, giant_textures, xy, sprite_index, giant_rgb);
    }

    void draw_giant_corpse(const Coords& xy) {
        draw_giant(xy, CORPSE_INDEX);
    }

    void draw_horse(const Coords& xy, const int &sprite_index) {
        draw_creation_texture(horse_bytes, horse_textures, xy, sprite_index, horse_rgb);
    }

    void draw_horse_corpse(const Coords& xy) {
        draw_horse(xy, CORPSE_INDEX);
    }

    void draw_unicorn(const Coords& xy, const int &sprite_index) {
        draw_creation_texture(unicorn_bytes, unicorn_textures, xy, sprite_index, unicorn_rgb);
    }

    void draw_unicorn_corpse(const Coords& xy) {
        draw_unicorn(xy, CORPSE_INDEX);
    }

    void draw_centaur(const Coords& xy, const int &sprite_index) {
        draw_creation_texture(centaur_bytes, centaur_textures, xy, sprite_index, centaur_rgb);
    }

    void draw_centaur_corpse(const Coords& xy) {
        draw_centaur(xy, CORPSE_INDEX);
    }

    void draw_pegasus(const Coords& xy, const int &sprite_index) {
        draw_creation_texture(pegasus_bytes, pegasus_textures, xy, sprite_index, pegasus_rgb);
    }

    void draw_pegasus_corpse(const Coords& xy) {
        draw_pegasus(xy, CORPSE_INDEX);
    }

    void draw_gryphon(const Coords& xy, const int &sprite_index) {
        draw_creation_texture(gryphon_bytes, gryphon_textures, xy, sprite_index, gryphon_rgb);
    }

    void draw_gryphon_corpse(const Coords& xy) {
        draw_gryphon(xy, CORPSE_INDEX);
    }

    void draw_manticore(const Coords& xy, const int &sprite_index) {
        draw_creation_texture(manticore_bytes, manticore_textures, xy, sprite_index, manticore_rgb);
    }

    void draw_manticore_corpse(const Coords& xy) {
        draw_manticore(xy, CORPSE_INDEX);
    }

    void draw_bat(const Coords& xy, const int &sprite_index) {
        draw_creation_texture(bat_bytes, bat_textures, xy, sprite_index, bat_rgb);
    }

    void draw_bat_corpse(const Coords& xy) {
        draw_bat(xy, CORPSE_INDEX);
    }

    void draw_green_dragon(const Coords& xy, const int &sprite_index) {
        draw_creation_texture(green_dragon_bytes, green_dragon_textures, xy, sprite_index, green_dragon_rgb);
    }

    void draw_green_dragon_corpse(const Coords& xy) {
        draw_green_dragon(xy, CORPSE_INDEX);
    }

    void draw_red_dragon(const Coords& xy, const int &sprite_index) {
        draw_creation_texture(red_dragon_bytes, red_dragon_textures, xy, sprite_index, red_dragon_rgb);
    }

    void draw_red_dragon_corpse(const Coords& xy) {
        draw_red_dragon(xy, CORPSE_INDEX);
    }

    void draw_golden_dragon(const Coords& xy, const int &sprite_index) {
        draw_creation_texture(golden_dragon_bytes, golden_dragon_textures, xy, sprite_index, golden_dragon_rgb);
    }

    void draw_golden_dragon_corpse(const Coords& xy) {
        draw_golden_dragon(xy, CORPSE_INDEX);
    }

    void draw_harpy(const Coords& xy, const int &sprite_index) {
        draw_creation_texture(harpy_bytes, harpy_textures, xy, sprite_index, harpy_rgb);
    }

    void draw_harpy_corpse(const Coords& xy) {
        draw_harpy(xy, CORPSE_INDEX);
    }

    void draw_eagle(const Coords& xy, const int &sprite_index) {
        draw_creation_texture(eagle_bytes, eagle_textures, xy, sprite_index, eagle_rgb);
    }

    void draw_eagle_corpse(const Coords& xy) {
        draw_eagle(xy, CORPSE_INDEX);
    }

    void draw_vampire(const Coords& xy, const int &sprite_index) {
        draw_creation_texture(vampire_bytes, vampire_textures, xy, sprite_index, vampire_rgb);
    }

    void draw_ghost(const Coords& xy, const int &sprite_index) {
        draw_creation_texture(ghost_bytes, ghost_textures, xy, sprite_index, ghost_rgb);
    }

    void draw_spectre(const Coords& xy, const int &sprite_index) {
        draw_creation_texture(spectre_bytes, spectre_textures, xy, sprite_index, spectre_rgb);
    }

    void draw_wraith(const Coords& xy, const int &sprite_index) {
        draw_creation_texture(wraith_bytes, wraith_textures, xy, sprite_index, wraith_rgb);
    }

    void draw_skeleton(const Coords& xy, const int &sprite_index) {
        draw_creation_texture(skeleton_bytes, skeleton_textures, xy, sprite_index, skeleton_rgb);
    }

    void draw_zombie(const Coords& xy, const int &sprite_index) {
        draw_creation_texture(zombie_bytes, zombie_textures, xy, sprite_index, zombie_rgb);
    }

    void draw_blob(const Coords& xy, const int &sprite_index) {
        draw_creation_texture(blob_bytes, blob_textures, xy, sprite_index, blob_rgb);
    }

    void draw_fire(const Coords& xy, const int &sprite_index) {
        draw_creation_texture(fire_bytes, fire_textures, xy, sprite_index, fire_rgb);
    }

    void draw_magic_wood(const Coords& xy, const int &sprite_index) {
        draw_creation_texture(magic_wood_bytes, magic_wood_textures, xy, sprite_index, magic_wood_rgb);
    }

    void draw_shadow_wood(const Coords& xy, const int &sprite_index) {
        draw_creation_texture(shadow_wood_bytes, shadow_wood_textures, xy, sprite_index, shadow_wood_rgb);
    }

    void draw_magic_castle(const Coords& xy, const int &sprite_index) {
        draw_creation_texture(magic_castle_bytes, magic_castle_textures, xy, sprite_index, magic_castle_rgb);
    }

    void draw_dark_citadel(const Coords& xy, const int &sprite_index) {
        draw_creation_texture(dark_citadel_bytes, dark_citadel_textures, xy, sprite_index, dark_citadel_rgb);
    }

    void draw_wall(const Coords& xy, const int &sprite_index) {
        draw_creation_texture(wall_bytes, wall_textures, xy, sprite_index, wall_rgb);
    }

    void draw_creation(const Creation& creation, const Coords& xy, const int &sprite_index) {
        switch(creation.id) {
        case 2:
            draw_king_cobra(xy, sprite_index);
            break;
        case 3:
            draw_dire_wolf(xy, sprite_index);
            break;
        case 4:
            draw_goblin(xy, sprite_index);
            break;
        case 5:
            draw_crocodile(xy, sprite_index);
            break;
        case 6:
            draw_faun(xy, sprite_index);
            break;
        case 7:
            draw_lion(xy, sprite_index);
            break;
        case 8:
            draw_elf(xy, sprite_index);
            break;
        case 9:
            draw_orc(xy, sprite_index);
            break;
        case 10:
            draw_bear(xy, sprite_index);
            break;
        case 11:
            draw_gorilla(xy, sprite_index);
            break;
        case 12:
            draw_ogre(xy, sprite_index);
            break;
        case 13:
            draw_hydra(xy, sprite_index);
            break;
        case 14:
            draw_giant_rat(xy, sprite_index);
            break;
        case 15:
            draw_giant(xy, sprite_index);
            break;
        case 16:
            draw_horse(xy, sprite_index);
            break;
        case 17:
            draw_unicorn(xy, sprite_index);
            break;
        case 18:
            draw_centaur(xy, sprite_index);
            break;
        case 19:
            draw_pegasus(xy, sprite_index);
            break;
        case 20:
            draw_gryphon(xy, sprite_index);
            break;
        case 21:
            draw_manticore(xy, sprite_index);
            break;
        case 22:
            draw_bat(xy, sprite_index);
            break;
        case 23:
            draw_green_dragon(xy, sprite_index);
            break;
        case 24:
            draw_red_dragon(xy, sprite_index);
            break;
        case 25:
            draw_golden_dragon(xy, sprite_index);
            break;
        case 26:
            draw_harpy(xy, sprite_index);
            break;
        case 27:
            draw_eagle(xy, sprite_index);
            break;
        case 28:
            draw_vampire(xy, sprite_index);
            break;
        case 29:
            draw_ghost(xy, sprite_index);
            break;
        case 30:
            draw_spectre(xy, sprite_index);
            break;
        case 31:
            draw_wraith(xy, sprite_index);
            break;
        case 32:
            draw_skeleton(xy, sprite_index);
            break;
        case 33:
            draw_zombie(xy, sprite_index);
            break;
        default:
            throw 0;
        }
    }

    void draw_creation_corpse(const Creation& creation, const Coords& xy) {
        switch(creation.id) {
        case 2:
            draw_king_cobra_corpse(xy);
            break;
        case 3:
            draw_dire_wolf_corpse(xy);
            break;
        case 4:
            draw_goblin_corpse(xy);
            break;
        case 5:
            draw_crocodile_corpse(xy);
            break;
        case 6:
            draw_faun_corpse(xy);
            break;
        case 7:
            draw_lion_corpse(xy);
            break;
        case 8:
            draw_elf_corpse(xy);
            break;
        case 9:
            draw_orc_corpse(xy);
            break;
        case 10:
            draw_bear_corpse(xy);
            break;
        case 11:
            draw_gorilla_corpse(xy);
            break;
        case 12:
            draw_ogre_corpse(xy);
            break;
        case 13:
            draw_hydra_corpse(xy);
            break;
        case 14:
            draw_giant_rat_corpse(xy);
            break;
        case 15:
            draw_giant_corpse(xy);
            break;
        case 16:
            draw_horse_corpse(xy);
            break;
        case 17:
            draw_unicorn_corpse(xy);
            break;
        case 18:
            draw_centaur_corpse(xy);
            break;
        case 19:
            draw_pegasus_corpse(xy);
            break;
        case 20:
            draw_gryphon_corpse(xy);
            break;
        case 21:
            draw_manticore_corpse(xy);
            break;
        case 22:
            draw_bat_corpse(xy);
            break;
        case 23:
            draw_green_dragon_corpse(xy);
            break;
        case 24:
            draw_red_dragon_corpse(xy);
            break;
        case 25:
            draw_golden_dragon_corpse(xy);
            break;
        case 26:
            draw_harpy_corpse(xy);
            break;
        case 27:
            draw_eagle_corpse(xy);
            break;
        default:
            throw 0;
        }
    }

    const std::vector<std::string> wizard_bytes = {
        "000000000180018007fe0f9c3b8c1b84138013c003c003c007e01fe000000000",
        "000000000180118013c017e21b941388138013c0124012401240124000000000",
        "0000000001800180038007c007f803b0039003c007c007c00f60030000000000",
        "00000000018001803fc00fe001f001d803c807c006c006c006600ee000000000",
        "0000000001800180008043e025d019c809c805c00360073004900c5800000000",
        "000000000180018000801fc00ce208d401c803d003e003e003f000fc00000000",
        "0000181819980db00e7007e003c003c003c003c007c007c007c00fe000000000",
        "00000000018001800080418023c015a009c00380028002400220064000000000"
    };

    std::map<int, std::map<int, std::unique_ptr<SpriteTexture>>> wizard_textures;

    void draw_indexed_coloured_texture_xy(const std::vector<std::string> &bytes, std::map<int, std::map<int, std::unique_ptr<SpriteTexture>>> &textures, const int &sprite_index, const RGB &rgb, const Coords& xy) {
        const auto rgb_index = rgb.get_24_bit_index(); 
        if(!textures[sprite_index][rgb_index])
            textures[sprite_index][rgb_index] = std::unique_ptr<SpriteTexture>(new SpriteTexture(bytes[sprite_index], rgb, black));
        sdl::draw(*textures[sprite_index][rgb_index], xy.x, xy.y);
    }

    void draw_indexed_coloured_texture(const std::vector<std::string> &bytes, std::map<int, std::map<int, std::unique_ptr<SpriteTexture>>> &textures, const int &sprite_index, const RGB &rgb, const Coords& xy) {
        draw_indexed_coloured_texture_xy(bytes, textures, sprite_index, rgb, {8 + xy.x * 16, 8 + xy.y * 16});
    }

    void draw_wizard(const int &sprite_index, const RGB &rgb, const Coords& xy) {
        draw_indexed_coloured_texture(wizard_bytes, wizard_textures, sprite_index, rgb, xy);
    }

    void draw_wizard_xy(const int &sprite_index, const RGB &rgb, const Coords& xy) {
        draw_indexed_coloured_texture_xy(wizard_bytes, wizard_textures, sprite_index, rgb, xy);
    }

    const std::vector<std::string> magic_sword_bytes = {
        "0000000000008180418021c013c00fc000c001c003c003c003e007f000000000",
        "0000000020002180118011c00fc003c000c001c003c003c003e007f000000000",
        "080008000800098009800dc007c001c000c001c003c003c003e007f000000000",
        "0000000020002180118011c00fc003c000c001c003c003c003e007f000000000"
    };

    const std::vector<std::string> magic_knife_bytes = {
        "0000000000008180418021c013c00fc000c001c003c003c003e007f000000000",
        "0000000020002180118011c00fc003c000c001c003c003c003e007f000000000",
        "080008000800098009800dc007c001c000c001c003c003c003e007f000000000",
        "0000000020002180118011c00fc003c000c001c003c003c003e007f000000000"
    };

    const std::vector<std::string> magic_armour_bytes = {
        "0000000000008180418021c013c00fc000c001c003c003c003e007f000000000",
        "0000000020002180118011c00fc003c000c001c003c003c003e007f000000000",
        "080008000800098009800dc007c001c000c001c003c003c003e007f000000000",
        "0000000020002180118011c00fc003c000c001c003c003c003e007f000000000"
    };

    const std::vector<std::string> magic_shield_bytes = {
        "0000000000000180018001c003b0057809780378037803b007c007f000000000",
        "0000000000000180018001c003b0057809780378037803b007c007f000000000",
        "0000000000000180018001c003b0057809780378037803b007c007f000000000",
        "0000000000000180018001c003b0057809780378037803b007c007f000000000"
    };

    const std::vector<std::string> magic_wings_bytes = {
        "000000000000318c799effff8ff105a0099003c0024006600420042000000000",
        "000000000000018001800ff03ffc7dbe899103c0024006600420042000000000",
        "0000000000000180018003c00ff01ff8199833cc224426640420042000000000",
        "000000000000018001800ff03ffc7dbe899103c0024006600420042000000000"
    };

    const std::vector<std::string> magic_bow_bytes = {
        "000001000360036003701ff003700370037001f001f001f001f801fc00000000",
        "00000100016002e002f00ff802f002f0017001f001f001f001f801fc00000000",
        "0000008001600160027007fc02700170017000f001f001f001f801fc00000000",
        "00000100036003600370fbf803700370037001f001f001f001f801fc00000000"
    };

    std::map<int, std::map<int, std::unique_ptr<SpriteTexture>>> magic_sword_textures;
    std::map<int, std::map<int, std::unique_ptr<SpriteTexture>>> magic_knife_textures;
    std::map<int, std::map<int, std::unique_ptr<SpriteTexture>>> magic_armour_textures;
    std::map<int, std::map<int, std::unique_ptr<SpriteTexture>>> magic_shield_textures;
    std::map<int, std::map<int, std::unique_ptr<SpriteTexture>>> magic_wings_textures;
    std::map<int, std::map<int, std::unique_ptr<SpriteTexture>>> magic_bow_textures;

    void draw_magic_sword(const int &sprite_index, const RGB &rgb, const Coords& xy) {
        draw_indexed_coloured_texture(magic_sword_bytes, magic_sword_textures, sprite_index, rgb, xy);
    }

    void draw_magic_knife(const int &sprite_index, const RGB &rgb, const Coords& xy) {
        draw_indexed_coloured_texture(magic_knife_bytes, magic_knife_textures, sprite_index, rgb, xy);
    }

    void draw_magic_armour(const int &sprite_index, const RGB &rgb, const Coords& xy) {
        draw_indexed_coloured_texture(magic_armour_bytes, magic_armour_textures, sprite_index, rgb, xy);
    }

    void draw_magic_shield(const int &sprite_index, const RGB &rgb, const Coords& xy) {
        draw_indexed_coloured_texture(magic_shield_bytes, magic_shield_textures, sprite_index, rgb, xy);
    }

    void draw_magic_wings(const int &sprite_index, const RGB &rgb, const Coords& xy) {
        draw_indexed_coloured_texture(magic_wings_bytes, magic_wings_textures, sprite_index, rgb, xy);
    }

    void draw_magic_bow(const int &sprite_index, const RGB &rgb, const Coords& xy) {
        draw_indexed_coloured_texture(magic_bow_bytes, magic_bow_textures, sprite_index, rgb, xy);
    }

    void draw_wizard(const Wizard wizard, const Coords& xy, const int &frame_index) {
        switch(wizard.last_change) {
        case Wizard::LastChange::none:
            draw_wizard(wizard.sprite_index, wizard.rgb, xy);
            break;
        case Wizard::LastChange::magic_sword:
            draw_magic_sword(frame_index, wizard.rgb, xy);
            break;
        case Wizard::LastChange::magic_knife:
            draw_magic_knife(frame_index, wizard.rgb, xy);
            break;
        case Wizard::LastChange::magic_armour:
            draw_magic_armour(frame_index, wizard.rgb, xy);
            break;
        case Wizard::LastChange::magic_shield:
            draw_magic_shield(frame_index, wizard.rgb, xy);
            break;
        case Wizard::LastChange::magic_wings:
            draw_magic_wings(frame_index, wizard.rgb, xy);
            break;
        case Wizard::LastChange::magic_bow:
            draw_magic_bow(frame_index, wizard.rgb, xy);
            break;
        }
    }

    const std::vector<std::string> exploding_circle_bytes = {
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

    const std::vector<std::string> twirl_bytes = {
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

    const std::vector<std::string> explosion_bytes = {
      "000000000000000002a00ac007e00fc007e00ac0054000000000000000000000",
      "000000000000029012a00ac007f03d4007e01ad0054009400000000000000000",
      "000002000248229012a00bc006b87d4006e01bd0254809401120010000000000",
      "020002444248229010a00b4006acf84006a01950644809441120212001000000",
      "022282444248228000a00a0004abd00004801150600081441122212141108100",
      "0222824400002090120000000012a00000000840412880000122000141108110",
      "0222800000482200000000000009800000000000100080240000011100008110"
    };

    const std::vector<std::string> dragon_burn_bytes = {
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

    const std::vector<std::string> attack_bytes = {
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

    const RGB *exploding_circle_rgb = &bright_white;
    const RGB *twirl_rgb = &bright_cyan;
    const RGB *explosion_rgb = &bright_yellow;
    const RGB *dragon_burn_rgb = &bright_yellow;
    const RGB *attack_rgb = &bright_yellow;

    std::map<int, std::unique_ptr<SpriteTexture>> exploding_circle_textures;
    std::map<int, std::unique_ptr<SpriteTexture>> twirl_textures;
    std::map<int, std::unique_ptr<SpriteTexture>> explosion_textures;
    std::map<int, std::unique_ptr<SpriteTexture>> dragon_burn_textures;
    std::map<int, std::unique_ptr<SpriteTexture>> attack_textures;

    void draw_effect_texture(const std::vector<std::string> &bytes, std::map<int, std::unique_ptr<SpriteTexture>> &textures, const RGB &rgb, const Coords& xy, const int& delay) {
        for(int sprite_index = 0; sprite_index < bytes.size(); ++sprite_index) {
            if(!textures[sprite_index])
                textures[sprite_index] = std::unique_ptr<SpriteTexture>(new SpriteTexture(bytes[sprite_index], rgb, black));
            sdl::draw(*textures[sprite_index], 8 + xy.x * 16, 8 + xy.y * 16);
            event::delay(delay);
        }
    }

    void draw_effect_textures(const std::vector<std::string> &bytes, std::map<int, std::unique_ptr<SpriteTexture>> &textures, const RGB &rgb, const std::vector<Coords> coords, const int& delay) {
        for(int sprite_index = 0; sprite_index < bytes.size(); ++sprite_index) {
            if(!textures[sprite_index])
                textures[sprite_index] = std::unique_ptr<SpriteTexture>(new SpriteTexture(bytes[sprite_index], rgb, black));
            for(const auto& xy:coords) {
                sdl::draw(*textures[sprite_index], 8 + xy.x * 16, 8 + xy.y * 16);
                event::delay(delay);
            }
        }
    }

    void draw_exploding_circle(const Coords& xy) {
        draw_effect_texture(exploding_circle_bytes, exploding_circle_textures, *exploding_circle_rgb, xy, 2);
    }

    void draw_exploding_circles(const std::vector<Coords> coords) {
        draw_effect_textures(exploding_circle_bytes, exploding_circle_textures, *exploding_circle_rgb, coords, 2);
    }

    void draw_twirl(const Coords& xy) {
        draw_effect_texture(twirl_bytes, twirl_textures, *twirl_rgb, xy, 2);
    }

    void draw_explosion(const Coords& xy) {
        draw_effect_texture(explosion_bytes, explosion_textures, *explosion_rgb, xy, 6);
    }

    void draw_dragon_burn(const Coords& xy) {
        draw_effect_texture(dragon_burn_bytes, dragon_burn_textures, *dragon_burn_rgb, xy, 6);
    }

    void draw_attack(const Coords& xy) {
        draw_effect_texture(attack_bytes, attack_textures, *attack_rgb, xy, 2);
    }

    const std::vector<std::vector<bool>> simple_beam_bitmask = {
        {true}
    };

    const std::vector<std::vector<bool>> spell_beam_bitmask = {
        {false, true, false},
        {true, true, true},
        {false, true, false}
    };

    const std::vector<std::vector<bool>> burn_beam_bitmask = {
        {false, false, false, true, false, false, false},
        {false, true, false, false, false, true, false},
        {false, false, false, true, false, false, false},
        {true, false, true, true, true, false, true},
        {false, false, false, true, false, false, false},
        {false, true, false, false, false, true, false},
        {false, false, false, true, false, false, false}
    };

    std::map<int, std::unique_ptr<Texture>> simple_beam_texures;
    std::map<int, std::unique_ptr<Texture>> spell_beam_texures;
    std::map<int, std::unique_ptr<Texture>> burn_beam_texures;

    void draw_beam_texture(const std::vector<std::vector<bool>> &bit_mask, std::map<int, std::unique_ptr<Texture>> &textures, const RGB &rgb, const Coords& xy) {
        const int rgb_index = rgb.get_24_bit_index();
        if(!textures[rgb_index])
            textures[rgb_index] = std::unique_ptr<SpriteTexture>(new SpriteTexture(bit_mask, rgb));
        int offset = 0;
        if(textures[rgb_index]->width > 1)
            offset = (textures[rgb_index]->width - 1) / 2;
        sdl::draw(*textures[rgb_index], 8 + xy.x - offset, 8 + xy.y - offset);
    }

    void draw_simple_beam(const RGB &rgb, const Coords& xy) {
        draw_beam_texture(simple_beam_bitmask, simple_beam_texures, rgb, xy);
    }

    void draw_spell_beam(const RGB &rgb, const Coords& xy) {
        draw_beam_texture(spell_beam_bitmask, spell_beam_texures, rgb, xy);
    }

    void draw_burn_beam(const RGB &rgb, const Coords& xy) {
        draw_beam_texture(burn_beam_bitmask, burn_beam_texures, rgb, xy);
    }

    void draw_burn_beam_animation(void (* draw)(const RGB &rgb, const Coords& xy), const RGB &rgb, const Coords& sxy, const Coords& dxy) {
        const int length = 12;
        auto coords = sxy.get_middle_of_tile().line_to(dxy.get_middle_of_tile());
        for(int i = 0; i < coords.size() + length; i += 4) {
            if(i < coords.size())
                (draw)(rgb, coords[i]);
            if(i >= length)
                (draw)(black, coords[i - length]);
            event::delay(4);
        }
    }

    void draw_beam_animation(void (* draw)(const RGB &rgb, const Coords& xy), const RGB &rgb, const Coords& sxy, const Coords& dxy, const int& length) {
        auto coords = sxy.get_middle_of_tile().line_to(dxy.get_middle_of_tile());
        for(int i = 0; i < coords.size() + length; ++i) {
            if(i < coords.size())
                (draw)(rgb, coords[i]);
            if(i >= length)
                (draw)(black, coords[i - length]);
            if(i % 5 == 0)
                event::delay(1);
        }
    }

    void simple_beam_animation(const RGB &rgb, const Coords& sxy, const Coords& dxy, const int& length) {
        audio::play_shot();
        draw_beam_animation(draw_simple_beam, rgb, sxy, dxy, length);
        audio::play_hit();
        draw_exploding_circle(dxy);
    }

    void spell_beam_animation(const Coords& sxy, const Coords& dxy) {
        audio::play_spell_beam();
        draw_beam_animation(draw_spell_beam, bright_cyan, sxy, dxy, 20);
        audio::play_spell_end();
        draw_twirl(dxy);
    }

    void burn_beam_animation(const Coords& sxy, const Coords& dxy) {
        audio::play_burn_beam();
        draw_burn_beam_animation(draw_burn_beam, bright_yellow, sxy, dxy);
        audio::play_burn();
        draw_dragon_burn(dxy);
    }

    const std::vector<std::string> loading_screen_bytes = {
      "0000000000000000000000000000000000000000000000000000000000000000",
      "0000000000000000000000000000000000000000000000000000000000000000",
      "0000000000000000000000000000000000000000000000000000000000000000",
      "0000000000000000000000000000000000000000000000000000000000000000",
      "0000000000000000000000000000000000000000000000000000000000000000",
      "0000000000000000000000000000000000000000000000000000000000000000",
      "0000000000000000000000000000000000000000000000000000000000000000",
      "0000000000000000000000000000000000000000000000000000000000000000",
      "00003c0000000000000000000000000000000000000000000000000000000000",
      "000042003c0000000000420000200040000000183c3c7e000000000000000000",
      "0000990042386838380042381c28384038780028424240000000000000000000",
      "0000a10040045444400042442030407844440008423c7c000000000000000000",
      "0000a1004e3c54783800424420303844444400083e4202000000000000000000",
      "000099004244544004005a442028044444780008024242000000000000000000",
      "000042003c3c543c78002438202478443840003e3c3c3c000000000000000000",
      "00003c0000000000000000000000000000400000000000000000000000000000",
      "0000000000000000000000000000000000000000000000000000000000000000",
      "0000000000000000000000000000000000000000000000000000000000000000",
      "0000000000000000000000000000000000000000000000000000000000000000",
      "0000000000000000000000000000000000000000000000000000000000000000",
      "0000000000000000000000000000000000000000000000000000000000000000",
      "0000000000000000000000000000000000000000000000000000000000000000",
      "0000000000000000000000000000000000000000000000000000000000000000",
      "0000000000000000000000000000000000000000000000000000000000000000",
      "0000000000000000000000000000000000000000000000000000000000000000",
      "0000000000000000000000000000000000000000000000000000000000000000",
      "0000000000000000000000000000000000000000000000000000000000000000",
      "0000000000000000000000000000000000000000000000000000000000000000",
      "0000000000000000000000000000000000000000000000000000000000000000",
      "0000000000000000000000000000000000000000000000000000000000000000",
      "0000000000000000000000000000000000000000000000000000000000000000",
      "0000000000000000000000000000000000000000000000000000000000000000",
      "0001fffff03fe3fe0fffff83ffffe1fffff00000000000000000000000000000",
      "0000ffffe01fc1fc07ffff01ffffc0ffffe00000000000000000000000000000",
      "0000ffffe01fc1fc07ffff01ffffc0ffffe00000000000000000000000000000",
      "0000ffffe01fc1fc07ffff01ffffc0ffffe00000000000000000000000000000",
      "0000ffffe01fc1fc07ffff01ffffc0ffffe00000000000000000000000000000",
      "0000ff1fe01fc1fc07f8ff01fe3fc0ff1fe00000000000000000000000000000",
      "0000fe0fe01fc1fc07f07f01fc1fc0fe0fe00000000000000000000000000000",
      "0000fe0fe01fc1fc07f07f01fc1fc0fe0fe00000000000000000000000000000",
      "0000fe0f801fc1fc07f07f01fc1fc0fe0fe00000000000000000000000000000",
      "0000fe0e001fc1fc07f07f01fc1fc0fe0fe00000000000000000000000000000",
      "0000fe08001fc3fc07f0ff01fc1fc0fe07e00000000000000000000000000000",
      "0000fe00001fcffc07f3ff01fc1fc0fe01e00000000000000000000000000000",
      "0000fe00001ffffc07ffff01fc1fc0ff80600000000000000000000000000000",
      "0000fe00001ffffc07ffff01fc1fc0ffe0000000000000000000000000000000",
      "0000fe00001ffffc07ffff01fc1fc0fff8000000000000000000000000000000",
      "0000fe00001ffdfc07ff7f01fc1fc0fffe000000000000000000000000000000",
      "0000fe00001ff1fc07fc7f01fc1fc03fff800000000000000000000000000000",
      "0000fe00601fc1fc07f07f01fc1fc00fffe00000000000000000000000000000",
      "0000fe01e01fc1fc07f07f01fc1fc003ffe00000000000000000000000000000",
      "0000fe07e01fc1fc07f07f01fc1fc0c0ffe00000000000000000000000000000",
      "0000fe0fe01fc1fc07f07f01fc1fc0f03fe00000000000000000000000000000",
      "0000fe0fe01fc1fc07f07f01fc1fc0fc0fe00000000000000000000000000000",
      "0000fe0fe01fc1fc07f07f01fc1fc0fe0fe00000000000000000000000000000",
      "0000fe0fe01fc1fc07f07f01fc1fc0fe0fe00000000000000000000000000000",
      "0000fe0fe01fc1fc07f07f01fc1fc0fe0fe00000000000000000000000000000",
      "0000fe0fe01fc1fc07f07f01fc1fc0fe0fe00000000000000000000000000000",
      "0000ff1fe01fc1fc07f07f01fe3fc0ff1fe00000000000000000000000000000",
      "0000ffffe01fc1fc07f07f01ffffc0ffffe00000000000000000000000000000",
      "0000ffffe01fc1fc07f07f01ffffc0ffffe00000000000000000000000000000",
      "0000ffffe01fc1fc07f07f01ffffc0ffffe00000000000000000000000000000",
      "0000ffffe01fc1fc07f07f01ffffc0ffffe00000000000000000000000000000",
      "0001fffff03fe3fe0ff8ff83ffffe1fffff00000000000000000000000000000",
      "0000000000000000000000000000000000000000000000000000000000000000",
      "0000000000000000000000000000000000000000000000000000000000000000",
      "0000000000000000000000000000000000000000000000000000000000000000",
      "0000000000000000000000000000000000000000000000000000000000000000",
      "0000000000000000000000000000000000000000000000000000000000000000",
      "0000000000000000000000000000000000000000000000000000000000000000",
      "0000000000000000000000000000000000000000000000000000000000000000",
      "0000000000000000000000000000000000000000000000000000000000000000",
      "0000000000000000000000000000000000000000000000000000000000000000",
      "0000000000000000000000000000000000000000000000000000000000000000",
      "0000000000000000000000000000000000000000000000000000000000000000",
      "0000000000000000000000000000000000000000000000000000000000000000",
      "000000000000000000000000000000000000000003ffe0000000000000000000",
      "00000000000000000000000000000000000038000dfff8000000000000000000",
      "0000000000000000000000000000000000007bfdfe1ffe000000000000000000",
      "0000000000000000000000000000000000007801c1e3f1fff000000000000000",
      "00000000000000000000000000000000000076fe3ffdefffff00000000000000",
      "0000000000000000000000000000000000008f79fffe2fffff80000000000000",
      "00000000000000000000000000000e000000f367ffffdfffffe0000000000000",
      "00000000000000000000000000001e0000015c6fffffc01ffff0000000000000",
      "0000000000000000000000000000180000017b8fffffdfe03ff0000000000000",
      "000000000000000000000000000018000001bb70ffffdfffc7f0000000000000",
      "000000000000000000000000000018000001c77b7fff3ffffbf6000000000000",
      "00000000000000000000000000001c000001f7858000fffffdcfb3f000000000",
      "00000000000000000000000000001e01e000f7fc8cdefffffd2fb87000000000",
      "000000000000000000000000000007fff800f7fcecdefffffef7bbafc0000000",
      "000000000000000000000000000003fffc007bfe73beffffff775b9fe0000000",
      "000000000000000000000000000000007e003dff1f7f7fffff776a6fe6000000",
      "000000000000000000000000000000001f001efe60ff7ffffef8f1f7cf000000",
      "000000000000000000000000000000000f000f7d7d7fbffffef3fbf737c00000",
      "000000000000000000000000000000000700078d7ebf4ffffdede5f6fbe00000",
      "00000000000000000000000000000000070000f37ec0f0ff83ddddf9fbd80000",
      "000000000000000000000000303c00000700076f82feff007b3d3ef5fa370000",
      "00000000000000000000000039fe00000700075ffdfefeff7cfefecef9fbc000",
      "0000000000000000000000001fdf00000700073ffdfefeff3dfeff3f65fdf000",
      "0000000000000000000000000703807f0700060fe3fdfeffd2fd7f7f5dfdf800",
      "0000000000000000000000000001c0ffc7000603dffdfeffcef37cbfbefdfe00",
      "0000000000000000000000000001e3ffe7000300effffeffdf6f73bf3efe0000",
      "0000000000000000000000000000ffc3ff00000007fbfd7fdf1f0fbcbdf8fb80",
      "00000000000000000000000000007f00ff00000003f7fd002c7fffb3ddf77dc0",
      "000000000000000000000000000000007f0000000037fdfff3797fafdf4f7de0",
      "000000000000000000000000000000003f0000000383fdffef67bf9fdfbf7ee0",
      "000000000000000000000000000000003f0000001f54fdf3ef1fbe5fde3f7ef0",
      "000000000000000000000000000000001f0000003eaa1c0faf7fb9dfddbf7ef0",
      "000000000000000000000000000000001f800000fd5500f75f7fa7dfc3bf7ef8",
      "000000000000000000000000000000000f800003eaaa1fcfdcff1fde1fbf60f8",
      "000000000000000000000000000000000f80000fd5553c3fe37cefd1dfb81e1c",
      "0000000000000000000000000000000007c0003fa0a0f3ffef23e7cfd8077eee",
      "0000000000000000000000000000000007f0007f001f0fefdfdfd91fc77f7ef1",
      "0000000000000000000000000000000003f803fa00f87fef0f1f0fefdf7f7f7f",
      "0000000000000000000000000000000001fffff403c7fe000000006fbf7f7f7f",
      "0000000000000000000000000000000000ffffe81c3ffc000000001c3eff7eff",
      "00000000000000000000000000000000003fffd033fff00000000003defc00ff",
      "00000000000000000000000000000000000fff804ff9c00000000001dec37e0f",
      "00000000000000000000000000000000000000003c00000000000000e13f7ef3",
      "000000000000000000000000000000000000000070000000000000001f7f7efb",
      "000000000000000000000000000000000000000000000000000000001f7f7dfd",
      "000000000000000000000000000000000000000000000000000000001f7efdff",
      "000000000000000000000000000000000000000000000000000000000f7efbff",
      "0000000000000000000000000000000000000000000000000000000006cefbff",
      "00000000000000000000000000000000000000000000000000000000003117ff",
      "0000000000000000000000000000000000000000000000000000000003efe7fe",
      "0000000000000000000000000000000000000000000000000000000003efe8fd",
      "0000000000000000000000000000000000000000000000000000000001dfef3b",
      "000000000000000000000000000000000000003ffe3fc0000000000001bfdfd7",
      "000000000000000000000000000000000003ff81f9dff1e000000000007fbfd7",
      "0000000000000000000000000000000001dfff7e27e7cef800000000019f7fe7",
      "0000000000000000000000000000000007bffeffdffb3f7d0000000001e6ffeb",
      "000000000000000000000000000000007f7ffcffa7fcff9e0000000001f9ffdd",
      "00000000000000000000000000000000ff7ffb7fb9fb7fe17e00000001fa7fbf",
      "00000000000000000000000000000003fffffbff7ee7bf1fbf00000001e7bf7f",
      "00000000000000000000000000000007ff0003ff7f5fdcefdf800000019fdeff",
      "0000000000000000000000000000000fc0fff87f7fbfd3f7efe00000007fe5ff",
      "0000000000000000000000000000001e3f7ffb877fbfeff7c0400000018ffbff",
      "0000000000000000000000000000003dff7ffbfb7f5fd7f42fee000003f7e5ff",
      "00000000000000000000000000000073ff7ffbfcff5fb7f3f7ef000003fbdeff",
      "0000000000000000000000000000006fff7ffdff7f6f7bcbf7ef000005fd3eff",
      "0000000000000000000000000000009fffbffdff9ef6fbbbf7cfc0000df8ff7e",
      "0000000000000000000000000000007fffbffeff6efafd7bfe2fe0003de6ffbd",
      "0000000000000000000000000000007fffdfff7f75fdfefdc5f7f0003c1f7fbb",
      "000000000000000000000000000000bfffdf807efbf9f9fd3bfbf800737f7fd7",
      "000000000000000000000000000001bfffe07f86f5f6e6f2ffff6c004f7fbfcf",
      "000000000000000000000000000001dffc17ffb80eef1e0efaaaf201bfbfdfaf",
      "000000000000000000000000000001eff3f9ffdc00000f56fdfde807bfbfee77",
      "00000000000000000000000000000db7effeffe80000002af3fb9a0fbfbffff7",
      "00000000000000000000000000001d7bdfff3ff0000000154575450fbfdfe3fb",
      "00000000000000000000000000007afd3fffcfe80000000a22e3801f9fdf17fb",
      "0000000000000000000000000001f9fefffff01800000000595114506000f7fb",
      "0000000000000000000000000007fabc1fffff580000000028ac09cf7fdffbfd",
      "000000000000000000000000000ff97beffffce800000000144053bf7fdffbfd",
      "000000000000000000000000001ffafbf3ffe3e0000000000a02007f7fdffbfd",
      "000000000000000000000000001ff977fc7fdfd80000000005151f807fdffbf3",
      "000000000000000000000000001ffaf7ff87b8000000000000aa7f7f8feffdef",
      "00000000000000000000000000dff96ffff847bb000000000010feff73effd9f",
      "00000000000000000000000001dffcefffe7ffbd8000000007f9feff7c6ffc7f",
      "00000000000000000000000003dc026fffdfff7d3f0000001ffa3eff7f8001ff",
      "00000000000000000000000003a3f95fffbffef8dfdf8701bff7c6ff7feffdff",
      "00000000000000000000000007affa9fff7ffee5df9ff0fe3feff97f7feffdff",
      "00000000000000000000000007aff95fff7ffd9eef6ff6fec7dffd9fbfeffeff",
      "0000000000000000000000000faffa8ffefffb7ef6f7cf3df8bffbe3bfeffeff",
      "0000000000000000000000000f77fd53fefff4fef6fbbfdbff1ffbfc3feffeff",
      "0000000000000000000000000f79fc9c7dff07fefdfcbfe7ff67fbffbfeffeff",
      "0000000000000000000000000f7e7d5f8000f7fef5ff7ff3fefbfbff8fefff7f",
      "0000000000000000000000000eff808ffdffeffec2fcbfedfdfcfbffb3efff7e",
      "0000000000000000000000000efffa57fbffeffd3ed3dfdefdff37ffbcefff7c",
      "0000000000000000000000006efffb3bfbffeff8ff6fefbf3dffd7ffbf6fff78",
      "00000000000000000000000ff6fff79ffbffeff5ff5ff77fdbffefffbf8fff70",
      "00000000000000000000003feafff7aff7ffdfedffbff6ffebffe7ffbff1ff60",
      "00000000000000000000007fdafff7d5f7ffdfedff5ffafff3ffebffbff61f00",
      "0000000000000000000002ffdaffefcaf7efdf9dfedff9fff5ffedffbff7e000",
      "0000000000000000000006ffdcffef2577dfdf7dfdeff6fff6ffee7fbff7ff00",
      "000000000000000000000effdeffecf2b2bfdcfdfbefef7ff77fefbfdff7ff00",
      "000000000000000000001effde3fe3f8557fd3fdf7f7efbff79fefdfdff7ff00",
      "000000000000000000001eff9ec01ffa02efeffdeff7dfdfefefdfefdff7fe00",
      "000000000000000000003efc6effdffd505c1ffdeffbdfefdff7dff3dff7fc00",
      "000000000000000000003f73eeffbffaaa83dffddffdbff7dffbdffddff7f800",
      "000000000000000000007f6feabfbff7517fdffdbffebff9dffddffedff7f000",
      "000000000000000000007f5fec7dbff7aaefdfed7fff7ffddffedfff1ff7c000",
      "000000000000000000007f3ff6ebbff7d55f5fdeff7f3ffebfff1fffeffb8000",
      "000000000000000000007ecff957b7f7e8badbbcfeff3fff3fffdfffe0fb0000",
      "000000000000000000007ef3feabaff4f555c77afdfedfffbfffdfffee7a0000",
      "001000007800000000007dfc7d559ffbf8aaaef6fbfeefff5fffcfffef980000",
      "001038004438444400003dfb8aabbfc5fe55454f77fef7ff6fffb3ffefe00000",
      "00104400424444280000ddf7f747373dffaaabbf6fecf7ff77ffbc3ff7f80000",
      "00107800424444100000eeefb02880edff00145555dbfbff7bffbfdff7e00000",
      "00104000444444280000f65545d5555cffca82aa2bbbfdff7dffbfeff7c00000",
      "000c3c00783838440000fb2aabca8aaa7fe55155175bfefefeffbff3f7000000",
      "00000000000000000000fd5547e555573ff2a8aaaabbff3eff7f7ffdf0000000"
    };

    const std::vector<std::string> loading_screen_attributes_bytes = {
      "0707070707070707070707070707070707070707070707070707070707070707",
      "0606060606060606060606060606060606060606060606060606060707070707",
      "0602420202020202020202020202420202020202020207070707070707070707",
      "0642024202020202020202020242024202020202020207070707070707070707",
      "0642424242424242424242424242424242424242070707070707070707070707",
      "0642424242424242424242424242424242424242070707070707070707070707",
      "0602420242024202420242024202420242024202020207070707070707070707",
      "0602024202420242024202420242024202420204040404040404070707070704",
      "0607070742074207420742024202420242060604040404040444040407070704",
      "0607070707420742074202420202020202060444444444444444440404040404",
      "0607070707074207420742070202420202060644444444444444444444444444",
      "0602020202020242024202424242424242020606040444444404044444444444",
      "0607070707070707424242424242424242024706060606040404040444444444",
      "0607070707070707074207070242424242420202020206060604040404444444",
      "0607070707070707070742070707020202020202060606060606060604040444",
      "0607070707070707070707040404040404040404060606060606060606040444",
      "0607070707070707070707444444040404444444444444444444040406040404",
      "0607070707070707070707070404040404444404040404040404040404040404",
      "0607070707070707070707074444440404040404060606060404040404040404",
      "0607070707070707070704044444440404040404040404044444440404040404",
      "0707070707070707070744444404040404040404444444444404040404040404",
      "0707070707070707070744040404040404040404040404040404040404040404",
      "0707070707040404040444040404040404040404040404040404040404040444",
      "0303030303030303030444040404040404040404040404040404040404040444"
    };

    std::unique_ptr<ScreenTexture> loading_screen;

    void draw_loading_screen() {
        if(!loading_screen)
            loading_screen = std::unique_ptr<ScreenTexture>(new ScreenTexture(loading_screen_bytes, loading_screen_attributes_bytes));
        sdl::draw(*loading_screen, 0, 0);
    }

    void draw_attributes(const std::vector<std::string> attributes) {
        int x = 4;
        for(auto& text:attributes) {
            if(text != attributes.front()) {
                draw_text(",", {x, 4}, bright_green);
                x += 1;
            }
            draw_text(text, {x, 4}, bright_green);
            x += text.size();
        }
    }

    void draw_stats(const Unit& unit) {
        draw_info_border();
        draw_text(unit.name, {4, 2}, bright_yellow);
        draw_text("COMBAT=", {4, 6}, bright_cyan);
        draw_text(std::to_string(unit.combat), {11, 6}, bright_white);
        draw_text("RANGED COMBAT=", {4, 8}, bright_cyan);
        draw_text(std::to_string(unit.ranged_combat), {18, 8}, bright_white);
        draw_text("RANGE=", {21, 8}, bright_cyan);
        draw_text(std::to_string(unit.range), {27, 8}, bright_white);
        draw_text("DEFENCE=", {4, 10}, bright_cyan);
        draw_text(std::to_string(unit.defence), {12, 10}, bright_white);
        draw_text("MOVEMENT ALLOWANCE=", {4, 12}, bright_cyan);
        draw_text(std::to_string(unit.movement), {23, 12}, bright_white);
        draw_text("MANOEUVRE RATING=", {4, 14}, bright_cyan);
        draw_text(std::to_string(unit.manoeuvre), {21, 14}, bright_white);
        draw_text("MAGIC RESISTANCE=", {4, 16}, bright_cyan);
        draw_text(std::to_string(unit.magical_resistance), {21, 16}, bright_white);
    }

    void draw_stats(const Creation& creation, const std::string& wizard_name) {
        draw_stats(static_cast<Unit>(creation));
        alignment_text(creation.alignment, {int(creation.name.size() + 5), 2});
        std::vector<std::string> attributes;
        if(creation.mount) {
            if(wizard_name.size())
                attributes.push_back("MOUNT(" + wizard_name + ")");
            else
                attributes.push_back("MOUNT");
        }
        if(creation.flying)
            attributes.push_back("FLYING");
        if(creation.undead)
            attributes.push_back("UNDEAD");
        draw_attributes(attributes);
    }

    void draw_stats(const Wizard& wizard) {
        draw_stats(static_cast<Unit>(wizard));
        std::vector<std::string> attributes;
        if (wizard.magic_sword)
            attributes.push_back("SWORD");
        else if (wizard.magic_knife)
            attributes.push_back("KNIFE");
        if (wizard.magic_armour)
            attributes.push_back("ARMOUR");
        else if (wizard.magic_shield)
            attributes.push_back("SHIELD");
        if (wizard.magic_wings)
            attributes.push_back("FLYING");
        if (wizard.shadow_form)
            attributes.push_back("SHADOW");
        draw_attributes(attributes);
        draw_text("SPELLS=" + std::to_string(wizard.number_of_spells), {4, 18}, bright_yellow);
        draw_text("ABILITY=" + std::to_string(wizard.ability), {14, 18}, bright_yellow);
    }

    void draw_spell(const Spell& spell) {
        int cast_chance_percentage = (spell.world_cast_chance() + 1) * 10;
        if (spell.type == Spell::creation) {
            auto creation = wizard::generate_creation_from_id(spell.id);
            draw_stats(creation);
            draw_text("CASTING CHANCE=" + std::to_string(cast_chance_percentage) + "%", {4, 18}, bright_cyan);
        } else {
            int cast_range = int(std::floor(spell.doubled_cast_range / 2));
            if(cast_range > 9)
                cast_range = 20;
            draw_spell_border();
            draw_text(spell.name, {8, 5}, bright_yellow);
            alignment_text(spell.alignment, {8, 7});
            draw_text("CASTING CHANCE=", {8, 11}, bright_green);
            draw_text(std::to_string(cast_chance_percentage) + "%", {23, 11}, bright_yellow);
            draw_text("RANGE=", {8, 15}, bright_green);
            draw_text(std::to_string(cast_range), {14, 15}, bright_yellow);
        }
        
    }

    const std::vector<const RGB *> spell_rgbs = {&bright_red, &bright_purple, &bright_purple, &bright_green, &bright_green, &bright_cyan, &bright_cyan, &bright_yellow, &bright_yellow, &bright_white};

    void draw_spellbook(const Wizard& wizard) {
        sdl::clear();
        draw_text(wizard.name + "'S SPELLS", {0, 0}, bright_yellow);
        for(int i = 0; i < wizard.number_of_spells; ++i) {
            int x = (i % 2) ? 17 : 0;
            int y = (std::floor(i / 2) + 1) * 2;
            std::string text;
            text = 'A' + i;
            if(wizard.spellbook[i].alignment > 0) {
                text += "^";
            } else if(wizard.spellbook[i].alignment < 0) {
                text += "*";
            } else {
                text += "-";
            }
            text += wizard.spellbook[i].name;
            int cast_chance = wizard.spellbook[i].world_cast_chance();
            draw_text(text, {x, y}, *spell_rgbs[cast_chance]);
        }
        draw_text("PRESS '0' TO RETURN TO MAIN MENU", {0, 22}, bright_yellow);
    }

    void draw_title_screen_network() {
        draw_title_border();
        draw_text("CHAOS -THE BATTLE OF WIZARDS", {2, 2}, bright_purple);
        draw_text("By Julian Gollop", {8, 4}, bright_red);
        draw_text("Host game?", {2, 9}, bright_yellow);
        draw_text("(2 to 8 players)", {2, 11}, bright_green);
    }

    void draw_title_screen() {
        draw_title_border();
        draw_text("CHAOS -THE BATTLE OF WIZARDS", {2, 2}, bright_purple);
        draw_text("By Julian Gollop", {8, 4}, bright_red);
        draw_text("How many wizards?", {2, 9}, bright_yellow);
        draw_text("(Press 2 to 8)", {2, 11}, bright_green);
    }

    void draw_enter_server() {
        draw_text("Server?", {2, 14}, bright_yellow);
        draw_text("(28 letters max.)", {10, 14}, bright_purple);
    }

    void draw_main_menu(const std::string& player_name, const int& world_alignment) {
        draw_menu_border();
        draw_text(player_name, {7, 5}, bright_yellow);
        if(world_alignment > 0) {
            draw_text("(LAW ", {7, 7}, bright_yellow);
            int num = std::floor(world_alignment / 4);
            for(int x = 0; x < num; ++x)
                draw_text("^", {12 + x, 7}, bright_yellow);
            draw_text(")", {12 + num, 7}, bright_yellow);
        }
        if(world_alignment < 0) {
            draw_text("(CHAOS ", {7, 7}, bright_cyan);
            int num = std::floor(std::abs(world_alignment) / 4);
            for(int x = 0; x < num; ++x)
                draw_text("*", {14 + x, 7}, bright_cyan);
            draw_text(")", {14 + num, 7}, bright_cyan);
        }
        draw_text("1.EXAMINE SPELLS", {7, 9}, bright_cyan);
        draw_text("2.SELECT SPELL", {7, 11}, bright_cyan);
        draw_text("3.EXAMINE BOARD", {7, 13}, bright_cyan);
        draw_text("4.CONTINUE WITH GAME", {7, 15}, bright_cyan);
    }

    void draw_player_entry_screen(const int& player_number) {
        draw_player_entry_border();
        draw_text("PLAYER", {2, 2}, bright_yellow);
        if(player_number)
            draw_text(std::to_string(player_number), {9, 2}, bright_yellow);
        draw_text("Enter name (12 letters max.)", {2, 4}, bright_purple);
    }

    void draw_computer_controlled_question() {
        draw_text("Computer controlled?", {2, 9}, bright_purple);
        draw_text("NO", {23, 9}, bright_yellow);
    }

    void draw_which_character_question() {
        draw_text("Which character?", {2, 11}, bright_purple);
        for(int i = 0; i < 8; ++i) {
            draw_text(std::to_string(i + 1), {2 + i * 3, 13}, bright_cyan);
            draw_wizard_xy(i, bright_cyan, {24 + i * 24, 13 * 8});
        }
    }

    void draw_which_colour_question(const int& sprite_index) {
        draw_text("Which colour?", {2, 16}, bright_purple);
        for(int i = 0; i < 8; ++i) {
            draw_text(std::to_string(i + 1), {2 + i * 3, 18}, bright_yellow);
            draw_wizard_xy(sprite_index, wizard_rgbs[i], {24 + i * 24, 18 * 8});
        }
    }

    void draw_info_text(const Tile& tile) {
        clear_status_line();
        int x = 0;
        if(tile.creation) {
            draw_text(tile.creation->name, {0, 22}, bright_cyan);
            x += tile.creation->name.size();
            if(tile.wizard) {
                draw_text("#", {x, 22}, bright_white);
                x += 1;
            } else if(tile.corpse) {
                draw_text("#", {x, 22}, bright_purple);
                x += 1;
            }
            draw_text("(" + tile.creation->owner->name + ")", {x, 22}, bright_yellow);
        } else if(tile.wizard) {
            draw_text(tile.wizard->name, {0, 22}, bright_cyan);
            x += tile.wizard->name.size();
            if(tile.corpse) {
                draw_text("#", {x, 22}, bright_purple);
                x += 1;
            }
        } else if(tile.corpse) {
            draw_text(tile.corpse->name, {0, 22}, bright_cyan);
            x += tile.corpse->name.size();
            draw_text("(DEAD)", {x + 1, 22}, bright_green);
        }
    }

    void cast_spell_text(const Wizard& wizard, const Spell& spell) {
        int x = 0;
        audio::play_spell_text();
        draw_text(wizard.name, {x, 22}, bright_yellow);
        event::delay(10);
        x += wizard.name.size() + 1;
        draw_text(spell.name, {x, 22}, bright_green);
        event::delay(10);
        x += spell.name.size() + 1;
        int cast_range = int(std::floor(spell.doubled_cast_range / 2));
        if(cast_range > 9)
            cast_range = 20;
        draw_text(std::to_string(cast_range), {x, 22}, bright_white);
        event::delay(30);
    }

    void draw_wizard_death_xy(const int& sprite_index, const RGB& rgb, const Coords& xy) {
        if(xy.x >= 8 && xy.x < 240 && xy.y >= 8 && xy.y < 160) {
            draw_wizard_xy(sprite_index, rgb, xy);
        }
    }

    void draw_wizard_death(const int& sprite_index, const Coords& wizard_xy) {
        const std::vector<RGB> wizard_colour_death_cycle = {bright_white, bright_yellow, bright_cyan, bright_green, bright_purple, bright_red, bright_blue, bright_black};
        audio::play_wizard_death();
        event::delay(10);
        for(auto& rgb:wizard_colour_death_cycle) {
            Coords xy = {wizard_xy.x * 16 + 8, wizard_xy.y * 16 + 8};
            for(int i = 0; i < 256; i += 8) {
                draw_wizard_death_xy(sprite_index, rgb, {xy.x - i, xy.y});
                draw_wizard_death_xy(sprite_index, rgb, {xy.x + i, xy.y});
                draw_wizard_death_xy(sprite_index, rgb, {xy.x, xy.y - i});
                draw_wizard_death_xy(sprite_index, rgb, {xy.x, xy.y + i});
                draw_wizard_death_xy(sprite_index, rgb, {xy.x - i, xy.y - i});
                draw_wizard_death_xy(sprite_index, rgb, {xy.x + i, xy.y - i});
                draw_wizard_death_xy(sprite_index, rgb, {xy.x + i, xy.y + i});
                draw_wizard_death_xy(sprite_index, rgb, {xy.x - i, xy.y + i});
                if(i % 48 == 0)
                    event::delay(2);
            }
        }
    }
}
