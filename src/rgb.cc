#include "rgb.h"
#include <vector>

RGB::RGB(const unsigned char &red, const unsigned char &green, const unsigned char &blue, const unsigned char &alpha) {
    this->red = red;
    this->green = green;
    this->blue = blue;
    this->alpha = alpha;
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
const RGB alpha(0, 0, 0, 0);

const std::vector<const RGB *> palette = {&black, &blue, &red, &purple, &green, &cyan, &yellow, &white, &bright_black, &bright_blue, &bright_red, &bright_purple, &bright_green, &bright_cyan, &bright_yellow, &bright_white};
