#include <vector>
#include <iostream>

#include "bit_grid.h"

using namespace std;

BitGrid::BitGrid(vector<string> grid, char true_c) {
    this->rows = grid.size();
    this->cols = grid[0].size();
    this->grid.reserve(((rows * cols) / 64) + 1);
    
    size_t scratch_pos = 0;
    uint64_t scratch = 0;

    for (size_t r = 0; r < this->rows; r++) {
        for (size_t c = 0; c < this->cols; c++) {
            cout << grid[r][c];
            if (grid[r][c] == true_c) {
                scratch |= 1;
            }
            scratch_pos += 1;
            
            if (scratch_pos == 64) { // reached the end of scratch so push it and start a new one
                this->grid.push_back(scratch);
                cout << scratch << endl;
                scratch = 0;
                scratch_pos = 0;
            }
            else {
                scratch <<= 1;
            }
        }
        cout << endl;
    }

    if (scratch_pos != 0) {
        this->grid.push_back(scratch << (64 - scratch_pos - 1));
        cout << scratch << endl;
    }
}

BitGrid::~BitGrid() { }

bool BitGrid::get(size_t x, size_t y) {
    if (x >= this->cols || y >= this->rows) {
        return false;
    }

    size_t index = x + y * this->cols;
    size_t grid_num = index / 64;
    size_t grid_pos = (64 - (index % 64)) - 1;

    //cout << index << " " << grid_num << " " << grid_pos;

    return ((this->grid[grid_num] >> grid_pos) & 1) == 1;
}

vector<coord> BitGrid::orth_adj(size_t x, size_t y) {
    vector<coord> coords;
    if (this->get(x - 1, y)) { // left
        coords.push_back(coord { x - 1, y });
    }
    if (this->get(x + 1, y)) { // right
        coords.push_back(coord { x + 1, y });
    }
    if (this->get(x, y - 1)) { // up
        coords.push_back(coord { x, y - 1 });
    }
    if (this->get(x, y + 1)) { // down
        coords.push_back(coord { x, y + 1});
    } 

    return coords;
}

/*
...
.@.
...
*/ 

vector<coord> BitGrid::all_adj(size_t x, size_t y) {
    vector<coord> coords;
    if (this->get(x - 1, y - 1)) { // diag
        coords.push_back(coord { x - 1, y - 1 });
    }
    if (this->get(x - 1, y + 1 )) { // diag
        coords.push_back(coord { x - 1, y + 1 });
    }
    if (this->get(x + 1, y - 1)) { // diag
        coords.push_back(coord { x + 1, y - 1 });
    }
    if (this->get(x + 1, y + 1)) { // diag
        coords.push_back(coord { x + 1, y + 1 });
    }

    if (this->get(x - 1, y)) { // left
        coords.push_back(coord { x - 1, y });
    }
    if (this->get(x + 1, y)) { // right
        coords.push_back(coord { x + 1, y });
    }
    if (this->get(x, y - 1)) { // up
        coords.push_back(coord { x, y - 1 });
    }
    if (this->get(x, y + 1)) { // down
        coords.push_back(coord { x, y + 1 });
    } 

    return coords;
}

bool BitGrid::get(coord c) {
    return this->get(c.x, c.y);
}

vector<coord> BitGrid::orth_adj(coord c) {
    return this->orth_adj(c.x, c.y);
}

vector<coord> BitGrid::all_adj(coord c) {
    return this->all_adj(c.x, c.y);
}