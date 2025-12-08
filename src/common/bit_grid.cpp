#include <vector>
#include <iostream>
#include <bit>
#include <bitset>

#include "bit_grid.h"
#include "utils.h"

using namespace std;

BitGrid::BitGrid(vector<string> grid, char true_c) {
    this->rows = grid.size();
    this->cols = grid[0].size();
    this->grid.reserve(((rows * cols) / 64) + 1);
    
    size_t scratch_pos = 0;
    uint64_t scratch = 0;

    for (size_t r = 0; r < this->rows; r++) {
        for (size_t c = 0; c < this->cols; c++) {
            if (grid[r][c] == true_c) {
                scratch |= 1;
            }
            scratch_pos += 1;
            
            if (scratch_pos == 64) { // reached the end of scratch so push it and start a new one
                this->grid.push_back(scratch);
                scratch = 0;
                scratch_pos = 0;
            }
            else {
                scratch <<= 1;
            }
        }
    }

    if (scratch_pos != 0) {
        this->grid.push_back(scratch << (64 - scratch_pos - 1));
    }
}

BitGrid::BitGrid(size_t rows, size_t cols) {
    this->grid = vector<uint64_t>((rows * cols) / 64 + 1, 0);
    this->rows = rows;
    this->cols = cols;
}

BitGrid::~BitGrid() { }

BitGrid BitGrid::clone() const {
    return *this;  // uses compiler-generated copy constructor
}

bool BitGrid::get(size_t x, size_t y) {
    if (x >= this->cols || y >= this->rows) {
        return false;
    }

    size_t index = x + y * this->cols;
    size_t grid_num = index / 64;
    size_t grid_pos = (64 - (index % 64)) - 1;

    return ((this->grid[grid_num] >> grid_pos) & 1) == 1;
}

void BitGrid::set(size_t x, size_t y, bool val) {
    if (x >= this->cols || y >= this->rows) {
        return;
    }

    size_t index = x + y * this->cols;
    size_t grid_num = index / 64;
    size_t grid_pos = (64 - (index % 64)) - 1;

    //cout << index << " " << grid_num << " " << grid_pos;

    if (val) {
        this->grid[grid_num] |= (1ULL << grid_pos);
    }
    else {
        this->grid[grid_num] &= ~(1ULL << grid_pos);
    }
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

void BitGrid::set(coord c, bool val) {
    return this->set(c.x, c.y, val);
}

vector<coord> BitGrid::orth_adj(coord c) {
    return this->orth_adj(c.x, c.y);
}

vector<coord> BitGrid::all_adj(coord c) {
    return this->all_adj(c.x, c.y);
}

long BitGrid::bits_set() {
    long sum = 0;
    for (uint64_t val: this->grid) {
        sum += popcount(val);
    }

    return sum;
}

void BitGrid::print() {
    for (size_t y = 0; y < this->cols; y++) {
        for (size_t x = 0; x < this->cols; x++) {
            if (this->get(x, y)) {
                cout << '#';
            }
            else {
                cout << '.';
            }
        }
        cout << endl;
    }
}