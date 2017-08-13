#include "event.h"
#include "gfx.h"
#include <functional>

namespace event {
    SDL_Event sdl_event;
    const Uint8 *keys;

    bool loop() {
        SDL_FlushEvent(SDL_KEYDOWN);
        while(SDL_PollEvent(&sdl_event)) {
            switch(sdl_event.type) {
            case SDL_QUIT:
                exit(0);
            case SDL_KEYDOWN:
                keys = SDL_GetKeyboardState(NULL);
                return true;
            }
        }
        sdl::refresh();
        return false;
    }

    void wait_for_keydown(const int& frame_delay) {
        int frame_count = 0;
        while(!loop()) {
            if(frame_delay) {
                if(frame_delay == frame_count) {
                    break;
                }
                frame_count += 1;
            }
        }
    }

    int get_alphabet() {
        while(true) {
            if(loop()) {
                if(keys[SDL_SCANCODE_0]) return 0;
                if(keys[SDL_SCANCODE_A]) return 1;
                if(keys[SDL_SCANCODE_B]) return 2;
                if(keys[SDL_SCANCODE_C]) return 3;
                if(keys[SDL_SCANCODE_D]) return 4;
                if(keys[SDL_SCANCODE_E]) return 5;
                if(keys[SDL_SCANCODE_F]) return 6;
                if(keys[SDL_SCANCODE_G]) return 7;
                if(keys[SDL_SCANCODE_H]) return 8;
                if(keys[SDL_SCANCODE_I]) return 9;
                if(keys[SDL_SCANCODE_J]) return 10;
                if(keys[SDL_SCANCODE_K]) return 11;
                if(keys[SDL_SCANCODE_L]) return 12;
                if(keys[SDL_SCANCODE_M]) return 13;
                if(keys[SDL_SCANCODE_N]) return 14;
                if(keys[SDL_SCANCODE_O]) return 15;
                if(keys[SDL_SCANCODE_P]) return 16;
                if(keys[SDL_SCANCODE_Q]) return 17;
                if(keys[SDL_SCANCODE_R]) return 18;
                if(keys[SDL_SCANCODE_S]) return 19;
                if(keys[SDL_SCANCODE_T]) return 20;
            }
        }
    }

    bool get_yes_or_no(const RGB& fg, const RGB& bg, const int& x, const int& y) {
        while(true) {
            if(loop()) {
                if(keys[SDL_SCANCODE_Y]) {
                    gfx::draw_text("YES", fg, bg, x, y);
                    sdl::refresh();
                    return true;
                }
                if(keys[SDL_SCANCODE_N]) {
                    gfx::draw_text("NO", fg, bg, x, y);
                    sdl::refresh();
                    return false;
                }
            }
        }
    }

    std::string text_input(std::function<bool(char)> validate, const int& max_characters, const RGB& fg, const RGB& bg, const int& x, const int& y) {
        std::string text;
        int text_x = x;
        bool text_entered = false;
        SDL_FlushEvent(SDL_KEYDOWN);
        SDL_FlushEvent(SDL_TEXTINPUT);
        do {
            while(SDL_PollEvent(&sdl_event)) {
                switch(sdl_event.type) {
                case SDL_QUIT:
                    exit(0);
                case SDL_KEYDOWN:
                    keys = SDL_GetKeyboardState(NULL);
                    if(text.size()) {
                        if(keys[SDL_SCANCODE_BACKSPACE]) {
                            text_x -= 1;
                            gfx::draw_text(text.substr(text.size() - 1, 1), bg, bg, text_x, y);
                            text.pop_back();
                        }
                        if(keys[SDL_SCANCODE_RETURN] || keys[SDL_SCANCODE_KP_ENTER])
                            text_entered = true;
                    }
                    break;
                case SDL_TEXTINPUT:
                    if(text.size() < max_characters && strlen(sdl_event.text.text) == 1 && validate(sdl_event.text.text[0])) {
                        text += sdl_event.text.text;
                        gfx::draw_text(text.substr(text.size() - 1, 1), fg, bg, text_x, y);
                        text_x += 1;
                    }
                    break;
                }
            }
            sdl::refresh();
        } while(!text_entered);
        return text;
    }

    std::string text_input_name(const RGB& fg, const int& x, const int& y) {
        return text_input([](char c) -> bool {
            return c == ' ' || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
        }, 12, fg, black, x, y);
    }

    std::string text_input_server(const RGB& fg, const int& x, const int& y) {
        return text_input([](char c) -> bool {
            return c >= '!' && c <= '~';
        }, 28, fg, black, x, y);
    }

    int number_input(const int& min, const int& max, const RGB& fg, const RGB& bg, const int& x, const int& y) {
        SDL_FlushEvent(SDL_TEXTINPUT);
        while(true) {
            while(SDL_PollEvent(&sdl_event)) {
                switch(sdl_event.type) {
                case SDL_QUIT:
                    exit(0);
                case SDL_TEXTINPUT:
                    if(strlen(sdl_event.text.text) == 1) {
                        int num = sdl_event.text.text[0] - '0';
                        if(num >= min && num <= max) {
                            gfx::draw_text(sdl_event.text.text, fg, bg, x, y);
                            loop();
                            return num;
                        }
                    }
                    break;
                }
            }
            sdl::refresh();
        }
    }

    bool move_xy(int& x, int& y, const int& diff_x, const int& diff_y) {
        x = std::max(0, std::min(14, x + diff_x));
        y = std::max(0, std::min(9, y + diff_y));
        return false;
    }

    bool cursor_movement(int& x, int& y) {
        if(loop()) {
            if(keys[SDL_SCANCODE_0]) return true;
            if(keys[SDL_SCANCODE_W]) return move_xy(x, y, 0, -1);
            if(keys[SDL_SCANCODE_E]) return move_xy(x, y, 1, -1);
            if(keys[SDL_SCANCODE_D]) return move_xy(x, y, 1, 0);
            if(keys[SDL_SCANCODE_C]) return move_xy(x, y, 1, 1);
            if(keys[SDL_SCANCODE_X]) return move_xy(x, y, 0, 1);
            if(keys[SDL_SCANCODE_Z]) return move_xy(x, y, -1, 1);
            if(keys[SDL_SCANCODE_A]) return move_xy(x, y, -1, 0);
            if(keys[SDL_SCANCODE_Q]) return move_xy(x, y, -1, -1);
        }
        return false;
    }

    int menu_option() {
        while(true) {
            if(loop()) {
                if(keys[SDL_SCANCODE_1]) return 1;
                if(keys[SDL_SCANCODE_2]) return 2;
                if(keys[SDL_SCANCODE_3]) return 3;
                if(keys[SDL_SCANCODE_4]) return 4;
            }
        }
    }
}
