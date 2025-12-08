#ifndef __AOC_DAY_6__
#define __AOC_DAY_6__

#include "aoc_day.h"

class AocDay6 : public AocDay
{
    private:
        tuple<vector<long>, vector<long>, vector<long>, vector<long>, vector<bool>> read_input_p1(string filename);
        vector<string> read_input_p2(string filename);
    public:
        AocDay6();
        ~AocDay6();
        string part1(string filename, vector<string> extra_args);
        string part2(string filename, vector<string> extra_args);
};


#endif
