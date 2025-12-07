#ifndef __AOC_DAY_5__
#define __AOC_DAY_5__

#include "aoc_day.h"

struct range {
    uint64_t start;
    uint64_t end;
};

class AocDay5 : public AocDay
{
    private:
        tuple<vector<range>, vector<uint64_t>> read_input(string filename);
    public:
        AocDay5();
        ~AocDay5();
        string part1(string filename, vector<string> extra_args);
        string part2(string filename, vector<string> extra_args);
};


#endif
