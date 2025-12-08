#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <tuple>

#include "aoc_day_7.h"
#include "file_utils.h"
#include "bit_grid.h"

using namespace std;

AocDay7::AocDay7():AocDay(7)
{
}

AocDay7::~AocDay7()
{
}

BitGrid AocDay7::read_input(string filename)
{
    FileUtils fileutils;
    vector<string> raw_lines;
    if (!fileutils.read_as_list_of_strings(filename, raw_lines))
    {
        cerr << "Error reading in the data from " << filename << endl;
        raw_lines = vector<string>();
    }
    
    return BitGrid(raw_lines, '^');
}

void update_manifold(BitGrid* splitters_map, BitGrid* splitters_hit, coord location) {
    if (location.x > splitters_map->cols || location.y > splitters_map->rows) {
        return;
    }

    for (size_t r = location.y; r < splitters_map->rows; r++) {
        if (splitters_map->get(location.x, r)) {
            if (splitters_hit->get(location.x, r)) {
                // already been so no point going again
                return;
            }
            splitters_hit->set(location.x, r, true);
            update_manifold(splitters_map, splitters_hit, coord { location.x - 1, r + 1 });
            update_manifold(splitters_map, splitters_hit, coord { location.x + 1, r + 1 });
            return;
        }
    }
}

string AocDay7::part1(string filename, vector<string> extra_args)
{
    BitGrid data = read_input(filename);
    BitGrid map = BitGrid(data.rows, data.cols);
    update_manifold(&data, &map, coord { data.cols / 2, 0 });

    ostringstream out;
    out << map.bits_set();
    return out.str();
}

string AocDay7::part2(string filename, vector<string> extra_args)
{
    BitGrid data = read_input(filename);
    BitGrid map = BitGrid(data.rows, data.cols);
    update_manifold(&data, &map, coord { data.cols / 2, 0 });

    ostringstream out;
    out << map.bits_set();
    return out.str();
}
