#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <tuple>
#include <algorithm>

#include "aoc_day_5.h"
#include "file_utils.h"

using namespace std;

AocDay5::AocDay5():AocDay(5)
{
}

AocDay5::~AocDay5()
{
}

tuple<vector<range>, vector<uint64_t>> AocDay5::read_input(string filename)
{
    FileUtils fileutils;
    vector<string> raw_lines;
    vector<range> ranges;
    vector<uint64_t> vals;

    if (!fileutils.read_as_list_of_strings(filename, raw_lines))
    {
        cerr << "Error reading in the data from " << filename << endl;
        raw_lines = vector<string>();
    }
    
    bool val_mode = false;
    for (string l: raw_lines) {
        if (l.size() == 0) {
            val_mode = true;
            continue;
        }
        if (val_mode) {
            vals.push_back(stoull(l));
        }
        else {
            ranges.push_back(range {
                stoull(l.substr(0, l.find('-'))),
                stoull(l.substr(l.find('-') + 1, l.size())),
            });
        }
    } 

    sort(ranges.begin(), ranges.end(),
        [](const range& a, const range& b) {
            return a.start < b.start;
        }
    );

    return tuple(ranges, vals);
}

string AocDay5::part1(string filename, vector<string> extra_args)
{
    auto [ranges, vals] = read_input(filename);
    long sum = 0;

    for (uint64_t v: vals) {
        for (range r: ranges) {
            if (v >= r.start && v <= r.end) {
                sum += 1;
                break;
            }
        }
    }

    ostringstream out;
    out << sum;
    return out.str();
}

string AocDay5::part2(string filename, vector<string> extra_args)
{
    vector<range> ranges = get<0>(read_input(filename));
    vector<range> merged = vector<range>();
    long sum = 0;
    uint64_t curr_low = 0;
    uint64_t curr_high = 0;

    for (range r: ranges) {
        if (r.start > curr_high) {
            sum += (curr_high - curr_low) + 1;
            curr_low = r.start;
            curr_high = r.end;
        }
        else {
            curr_high = max(r.end, curr_high);
        }
    }

    sum += (curr_high - curr_low) + 1;

    ostringstream out;
    out << sum - 1;
    return out.str();
}
