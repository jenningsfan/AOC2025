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

long remove_paper(BitGrid *data) {
    long sum = 0;
    BitGrid data_clone = data->clone();

    for (size_t r = 0; r < data_clone.rows; r++)
    {
        for (size_t c = 0; c < data_clone.cols; c++) {
            if (data_clone.get(c, r) && (data_clone.all_adj(c, r).size() < 4)) {
                data->set(c, r, false);
                sum += 1;
            }
        }
    }

    return sum;
}

string AocDay4::part1(string filename, vector<string> extra_args)
{
    BitGrid data = read_input(filename);
    ostringstream out;
    out << remove_paper(&data);
    return out.str();
}

string AocDay4::part2(string filename, vector<string> extra_args)
{
    BitGrid data = read_input(filename);
    long sum = 0;
    long result = -1;
    while (result != 0) {
        result = remove_paper(&data);
        sum += result;
        cout << result << endl;
    }
    ostringstream out;
    out << sum;
    return out.str();
}
