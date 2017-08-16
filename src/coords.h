#pragma once
#include <vector>

struct Coords {
    int x = 0;
    int y = 0;
    bool operator==(const Coords& other) const;
    Coords get_middle_of_tile() const;
    Coords compute_diff_with(const Coords& other) const;
    std::vector<Coords> line_to(const Coords& dxy) const;
};