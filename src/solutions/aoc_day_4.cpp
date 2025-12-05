#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <tuple>

#include "aoc_day_4.h"
#include "file_utils.h"
#include "bit_grid.h"

using namespace std;

AocDay4::AocDay4():AocDay(4)
{
}

AocDay4::~AocDay4()
{
}

BitGrid AocDay4::read_input(string filename)
{
    FileUtils fileutils;
    vector<string> raw_lines;
    if (!fileutils.read_as_list_of_strings(filename, raw_lines))
    {
        cerr << "Error reading in the data from " << filename << endl;
        raw_lines = vector<string>();
    }
    
    return BitGrid(raw_lines, '@');
}

string AocDay4::part1(string filename, vector<string> extra_args)
{
    BitGrid data = read_input(filename);
    long sum = 0;
    for (size_t r = 0; r < data.rows; r++)
    {
        for (size_t c = 0; c < data.cols; c++) {
            if (data.get(c, r) && (data.all_adj(c, r).size() < 4)) {
                cout << "x";
                sum += 1;
            }
            else if (data.get(c, r)) {
                cout << "@";
            }
            else {
                cout << ".";
            }
        }
        cout << endl;
    }
    ostringstream out;
    out << sum;
    return out.str();
}

string AocDay4::part2(string filename, vector<string> extra_args)
{
    BitGrid data = read_input(filename);
    long sum = 0;
    for (size_t r = 0; r < data.rows; r++)
    {
        for (size_t c = 0; c < data.cols; c++) {
            if (data.all_adj(c, r).size() < 4) {
                sum += 1;
            }
        }
    }
    ostringstream out;
    out << sum;
    return out.str();
}
