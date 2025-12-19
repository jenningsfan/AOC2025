#include <string>
#include <vector>
#include <unordered_set>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <tuple>
#include <array>
#include <cmath>
#include <climits>
#include <algorithm>
#include <numeric>

#include "aoc_day_9.h"
#include "file_utils.h"
#include "utils.h"

using namespace std;

namespace std {
    template<>
    struct hash<std::array<int,3>> {
        size_t operator()(std::array<int,3> const& a) const noexcept {
            size_t h = 0;
            for (int v : a) {
                h ^= std::hash<int>{}(v) + 0x9e3779b97f4a7c15ULL + (h << 6) + (h >> 2);
            }
            return h;
        }
    };
}

void print_progress(int lines_completed, int total_lines) {
    cout << lines_completed << endl;
    float progress = static_cast<float>(lines_completed) / static_cast<float>(total_lines);
    int barWidth = 70;

    std::cout << "[";
    int pos = barWidth * progress;
    for (int i = 0; i < barWidth; ++i) {
        if (i < pos) std::cout << "=";
        else if (i == pos) std::cout << ">";
        else std::cout << " ";
    }
    std::cout << "] " << int(progress * 100.0) << " %\r";
    std::cout.flush();
    std::cout << std::endl;
}

AocDay9::AocDay9():AocDay(9)
{
}

AocDay9::~AocDay9()
{
}

vector<array<int,2>> AocDay9::read_input(string filename)
{
    FileUtils fileutils;
    vector<vector<int>> data;
    vector<array<int,2>> converted_data;

    if (!fileutils.read_as_list_of_split_ints(filename, data, ',', '\0', '\0'))
    {
        cerr << "Error reading in the data from " << filename << endl;
        return converted_data;
    }
    
    for (const auto& obj : data) {
        converted_data.push_back({ 
            static_cast<int>(obj[0]),
            static_cast<int>(obj[1]),
        });
    }

    return converted_data;
}

bool between(int val, int start, int end) {
    return val >= start && val <= end;
}

int calc_area(const array<int,2>& a, const array<int,2>& b) {
    return (abs(a[0] - b[0]) + 1) * (abs(a[1] - b[1]) + 1);
}

bool line_horiz_in_shape(int start, int end, int y, vector<vector<array<int,2>>> *lines) {
    vector<array<int,2>> ranges = lines->at(y);

    for (array<int,2> range: ranges) {
        if (start >= range[0] && end <= range[1]) {
            return true;
        }
    }

    return false;
}

string AocDay9::part1(string filename, vector<string> extra_args)
{
    vector<array<int,2>> data = read_input(filename);
    int max_area = 0;
        
    for (size_t outer = 0; outer < data.size(); outer++) {
        for (size_t inner = outer + 1; inner < data.size(); inner++) {
            int area = calc_area(data[outer], data[inner]);

            if (area > max_area) {
                max_area = area;
            }
        }
    }
    
    ostringstream out;
    out << max_area;
    return out.str();
}


unordered_set<uint64_t> prep_coords(vector<array<int,2>> *data) {
    unordered_set<uint64_t> coords;
    coords.reserve(590072); // number of places

    for (size_t i = 0; i < data->size() - 1; i++) {
        array<int,2> before = data->at(i);
        array<int,2> after = data->at(i + 1);

        if (before[0] == after[0]) {
            for (uint64_t y = min(before[1], after[1]); y <= max(before[1], after[1]); y++) {
                coords.insert(static_cast<uint64_t>(before[0]) << 32 | y);
            }
        }
        else {
            for (uint64_t x = min(before[0], after[0]); x <= max(before[0], after[0]); x++) {
                coords.insert(x << 32 | static_cast<uint64_t>(before[1]));
            }
        }
    }

    for (uint64_t y = 0; y < 12; y++) {
        for (uint64_t x = 0; x < 15; x++) {
            if (coords.contains(x << 32 | y)) {
                cout << "#";
            }
            else {
                cout << ".";
            }
        }
        cout << endl;
    }
    
    return coords;
}

bool is_valid(const array<int,2>& a, const array<int,2>& b, vector<vector<array<int,2>>> *lines) {
    int u = min(a[1], b[1]);
    int d = max(a[1], b[1]);
    int l = min(a[0], b[0]);
    int r = max(a[0], b[0]);

    // DBG(u);
    // DBG(d);
    // DBG(l);
    // DBG(r);

    // DBG(line_horiz_in_shape(l, r, u, lines));
    // DBG(line_horiz_in_shape(l, r, d, lines));
    // DBG(line_vert_in_shape(u, d, l, lines));
    // DBG(line_vert_in_shape(u, d, r, lines));

    // return (line_horiz_in_shape(l, r, u, lines) && line_horiz_in_shape(l, r, d, lines) &&
    //     line_vert_in_shape(u, d, l, lines) && line_vert_in_shape(u, d, r, lines));

    for (int y = u; y <= d; y++) {
        if (!line_horiz_in_shape(l, r, y, lines)) {
            return false;
        }
    }

    return true;
}

vector<array<int,2>> merge_ranges(vector<array<int,2>> *ranges) {
    vector<array<int,2>> merged;
    merged.reserve(ranges->size());

    cout << "merge lala" << ranges->size() << endl;

    bool merge_at_end = false;
    for (int i = 0; i < ranges->size() - 1; i++) {
        array<int, 2> first = ranges->at(i);
        array<int, 2> second = ranges->at(i + 1);

        if (first[1] <= second[0]) {
            merged.push_back({ first[0], second[0] });
            i += 1;
            if (i >= ranges->size() - 1) {
                merge_at_end = true;
            }
        }
        else {
            merged.push_back(first);
        }
    }

    if (merge_at_end) {
        merged.push_back(ranges->at(ranges->size() - 1));
    }

    if (ranges->size() == merged.size() || merged.size() == 1) {
        return merged;
    }
    else {
        return merge_ranges(&merged);
    }
}

vector<vector<array<int,2>>> prep_lines(vector<array<int,2>> *data) {
    vector<vector<array<int,2>>> lines(100000);

    for (size_t i = 0; i < data->size() - 1; i++) {
        array<int,2> before = data->at(i);
        array<int,2> after = data->at(i + 1);

        if (before[1] == after[1]) {
            // horiz
            lines[before[1]].push_back({ min(before[0], after[0]), max(before[0], after[0]) });
        }
        else {
            for (int y = min(before[1], after[1]); y <= max(before[1], after[1]); y++) {
                lines[y].push_back({ before[0], before[0] });
            }
        }
    }

    // merge here if needed??
    // yes it is indeeedlely needed
    
    vector<vector<array<int,2>>> lines_merged(100000);

    int y = 0;
    for (vector<array<int,2>>& line: lines) {
        if (line.size() > 1) {
            cout << "merging " << y << endl;
            sort(line.begin(), line.end());
            lines_merged[y] = merge_ranges(&line);
            cout << "merged " << y << endl;
        }
        y += 1;
    }

    vector<vector<array<int,2>>> lines_final(100000);
    y = 0;
    for (vector<array<int,2>>& line: lines_merged) {
        if (line.size() % 2 == 1 && line.size() != 1) {
            cout << line.size() << endl;
            cout << 1 / 0 << endl;
        }
        if (line.size() >= 2) {
            for (size_t i = 0; i < line.size(); i+=2) {
                array<int,2> before = line.at(i);
                array<int,2> after = line.at(i + 1);
                lines_final[y].push_back({ before[0], after[1] });
            }
        }
        else if (line.size() == 1) {
            lines_final[y].push_back(line[0]);
        }

        //cout << y << endl;
        y += 1;
    }

    return lines_final;
}

string AocDay9::part2(string filename, vector<string> extra_args)
{
    vector<array<int,2>> data = read_input(filename);
    data.push_back(data[0]); // add last to end so that it all nicely wraps yipee happiness lala tadah
    int max_area = 0;
    
    unordered_set<uint64_t> coords = prep_coords(&data);
    int bottom_y = 0;

    for (array<int, 2> c: data) {
        if (c[1] > bottom_y) {
            bottom_y = c[1];
        }
    }
    
    vector<vector<array<int,2>>> lines = prep_lines(&data);

    // erase_if(data, [](array<int,2> x) {
    //     return x % 2 == 0;
    // });

    //cout << "9,5 2,3" << is_valid({ 9, 5 }, { 2, 3 }, &data) << endl;
    
    for (size_t outer = 0; outer < data.size() - 1; outer++) {
        for (size_t inner = outer + 1; inner < data.size() - 1; inner++) {
            int area = calc_area(data[outer], data[inner]);
            if (area > max_area && is_valid(data[outer], data[inner], &lines)) {
                cout << "max " << area << " " << data[outer][0] << "," << data[outer][1] << " " << data[inner][0] << "," << data[inner][1] << endl;
                max_area = area;
            }
        }
            
        print_progress(outer, data.size() - 1);
    }
        
    ostringstream out;
    out << max_area;
    return out.str();
}
