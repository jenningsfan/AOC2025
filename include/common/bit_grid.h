#ifndef __BIT_GRID_H__
#define __BIT_GRID_H__

#include <string>
#include <vector>

using namespace std;

struct coord {
    size_t x;
    size_t y;
};

class BitGrid
{
    private:
        vector<uint64_t> grid;
    public:
        size_t rows;
        size_t cols;
        
        BitGrid(vector<string> grid, char true_c);
        ~BitGrid();

        bool get(size_t x, size_t y);
        vector<coord> orth_adj(size_t x, size_t y);
        vector<coord> all_adj(size_t x, size_t y);

        bool get(coord c);
        vector<coord> orth_adj(coord c);
        vector<coord> all_adj(coord c);
};

#endif
