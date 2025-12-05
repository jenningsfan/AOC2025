#ifndef __AOC_DAY_4__
#define __AOC_DAY_4__

#include "aoc_day.h"
#include "bit_grid.h"

class AocDay4 : public AocDay
{
    private:
        BitGrid read_input(string filename);
    public:
        AocDay4();
        ~AocDay4();
        string part1(string filename, vector<string> extra_args);
        string part2(string filename, vector<string> extra_args);
};


#endif
