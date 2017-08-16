#include "coords.h"
#include <cstdlib>

bool Coords::operator==(const Coords& other) const {
    return (x == other.x) && (y == other.y);
}

Coords Coords::get_middle_of_tile() const {
    return {x * 16 + 8, y * 16 + 8};
}

Coords Coords::compute_diff_with(const Coords& other) const {
    return {std::abs(x - other.x), std::abs(y - other.y)};
}

std::vector<Coords> Coords::line_to(const Coords& dxy) const {
    Coords xy = {x, y};
    int dx = std::abs(dxy.x - x);
    int sx = (x < dxy.x) ? 1 : -1;
    int dy = std::abs(dxy.y - y);
    int sy = (y < dxy.y) ? 1 : -1;
    int err = ((dx > dy) ? dx : -dy) / 2;
    int e2;
    std::vector<Coords> coords;
    while(true) {
        e2 = err;
        if(e2 > -dx) {
            err -= dy;
            xy.x += sx;
        }
        if(e2 < dy) {
            err += dx;
            xy.y += sy;
        }
        if(xy == dxy)
            break;
        coords.push_back(xy);
    }
    return coords;
}
