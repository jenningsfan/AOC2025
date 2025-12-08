#ifndef __AOC_DAY_7__
#define __AOC_DAY_7__

#include "aoc_day.h"
#include "bit_grid.h"

class AocDay7 : public AocDay
{
    private:
        BitGrid read_input(string filename);
    public:
        AocDay7();
        ~AocDay7();
        string part1(string filename, vector<string> extra_args);
        string part2(string filename, vector<string> extra_args);
};


#endif
