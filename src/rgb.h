#pragma once
#include <vector>
struct RGB {
    unsigned char red, green, blue, alpha;
    RGB(const unsigned char &red = 0, const unsigned char &green = 0, const unsigned char &blue = 0, const unsigned char &alpha = 255);
    int get_24_bit_index() const;
};

extern const RGB black;
extern const RGB blue;
extern const RGB red;
extern const RGB purple;
extern const RGB green;
extern const RGB cyan;
extern const RGB yellow;
extern const RGB white;
extern const RGB bright_black;
extern const RGB bright_blue;
extern const RGB bright_red;
extern const RGB bright_purple;
extern const RGB bright_green;
extern const RGB bright_cyan;
extern const RGB bright_yellow;
extern const RGB bright_white;
extern const RGB alpha;

extern const std::vector<const RGB *> palette;
