#include "event.h"
#include "gfx.h"
#include "audio.h"
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

    void wait_for_key(const int& frame_delay) {
        int frame_count = 0;
        while(!loop()) {
            if(frame_delay) {
                if(frame_delay == frame_count)
                    return;
                frame_count += 1;
            }
        }
    }

    int get_alphabet() {
        while(true) {
            if(loop()) {
                if(keys[SDL_SCANCODE_0]) return -1;
                if(keys[SDL_SCANCODE_A]) return 0;
                if(keys[SDL_SCANCODE_B]) return 1;
                if(keys[SDL_SCANCODE_C]) return 2;
                if(keys[SDL_SCANCODE_D]) return 3;
                if(keys[SDL_SCANCODE_E]) return 4;
                if(keys[SDL_SCANCODE_F]) return 5;
                if(keys[SDL_SCANCODE_G]) return 6;
                if(keys[SDL_SCANCODE_H]) return 7;
                if(keys[SDL_SCANCODE_I]) return 8;
                if(keys[SDL_SCANCODE_J]) return 9;
                if(keys[SDL_SCANCODE_K]) return 10;
                if(keys[SDL_SCANCODE_L]) return 11;
                if(keys[SDL_SCANCODE_M]) return 12;
                if(keys[SDL_SCANCODE_N]) return 13;
                if(keys[SDL_SCANCODE_O]) return 14;
                if(keys[SDL_SCANCODE_P]) return 15;
                if(keys[SDL_SCANCODE_Q]) return 16;
                if(keys[SDL_SCANCODE_R]) return 17;
                if(keys[SDL_SCANCODE_S]) return 18;
                if(keys[SDL_SCANCODE_T]) return 19;
            }
        }
    }

    bool get_yes_or_no() {
        while(true) {
            if(loop()) {
                if(keys[SDL_SCANCODE_Y]) return true;
                if(keys[SDL_SCANCODE_N]) return false;
            }
        }
    }

    bool get_yes_or_no(const RGB& fg, const RGB& bg, const Coords& xy) {
        bool choice = get_yes_or_no();
        audio::play_key();
        gfx::draw_text(choice ? "YES" : "NO", fg, bg, xy);
        sdl::refresh();
        return choice;
    }

    std::string text_input(std::function<bool(char)> validate, const int& max_characters, const RGB& fg, const RGB& bg, const Coords& xy) {
        std::string text;
        int text_x = xy.x;
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
                            audio::play_key();
                            gfx::draw_text(text.substr(text.size() - 1, 1), bg, bg, {text_x, xy.y});
                            text.pop_back();
                        }
                        if(keys[SDL_SCANCODE_RETURN] || keys[SDL_SCANCODE_KP_ENTER])
                            text_entered = true;
                    }
                    break;
                case SDL_TEXTINPUT:
                    if(text.size() < max_characters && strlen(sdl_event.text.text) == 1 && validate(sdl_event.text.text[0])) {
                        text += sdl_event.text.text;
                        audio::play_key();
                        gfx::draw_text(text.substr(text.size() - 1, 1), fg, bg, {text_x, xy.y});
                        text_x += 1;
                    }
                    break;
                }
            }
            sdl::refresh();
        } while(!text_entered);
        return text;
    }

    std::string text_input_name(const RGB& fg, const Coords& xy) {
        return text_input([](char c) -> bool {
            return c == ' ' || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
        }, 12, fg, black, xy);
    }

    std::string text_input_server(const RGB& fg, const Coords& xy) {
        return text_input([](char c) -> bool {
            return c >= '!' && c <= '~';
        }, 28, fg, black, xy);
    }

    int number_input(const int& min, const int& max, const RGB& fg, const RGB& bg, const Coords& xy) {
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
                            audio::play_key();
                            gfx::draw_text(sdl_event.text.text, fg, bg, xy);
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

    CursorOption move_xy(Coords& xy, const Coords& diff) {
        xy.x = std::max(0, std::min(14, xy.x + diff.x));
        xy.y = std::max(0, std::min(9, xy.y + diff.y));
        return CursorOption::none;
    }

    CursorOption cursor_movement(Coords& xy) {
        if(loop()) {
            if(keys[SDL_SCANCODE_0]) return CursorOption::ok;
            if(keys[SDL_SCANCODE_W]) return move_xy(xy, {0, -1});
            if(keys[SDL_SCANCODE_E]) return move_xy(xy, {1, -1});
            if(keys[SDL_SCANCODE_D]) return move_xy(xy, {1, 0});
            if(keys[SDL_SCANCODE_C]) return move_xy(xy, {1, 1});
            if(keys[SDL_SCANCODE_X]) return move_xy(xy, {0, 1});
            if(keys[SDL_SCANCODE_Z]) return move_xy(xy, {-1, 1});
            if(keys[SDL_SCANCODE_A]) return move_xy(xy, {-1, 0});
            if(keys[SDL_SCANCODE_Q]) return move_xy(xy, {-1, -1});
            if(keys[SDL_SCANCODE_I]) return CursorOption::info;
            if(keys[SDL_SCANCODE_K]) return CursorOption::cancel;
            if(keys[SDL_SCANCODE_S]) return CursorOption::select;
        }
        return CursorOption::none;
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

    void delay(const int& frames) {
        for(int i = 0; i < frames; ++i)
            if(!loop())
                sdl::refresh();
    }
}
