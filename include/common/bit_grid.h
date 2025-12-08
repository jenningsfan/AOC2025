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
        BitGrid(size_t rows, size_t cols);
        ~BitGrid();
        BitGrid clone() const;

        bool get(size_t x, size_t y);
        void set(size_t x, size_t y, bool val);
        vector<coord> orth_adj(size_t x, size_t y);
        vector<coord> all_adj(size_t x, size_t y);

        bool get(coord c);
        void set(coord c, bool val);
        vector<coord> orth_adj(coord c);
        vector<coord> all_adj(coord c);

        long bits_set();
        void print();
};

#endif
