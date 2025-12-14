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

bool point_in_shape(const array<int,2>& a, const vector<array<int,3>> *lines) {
    int inters = 0;

    for (int i = 0; i < lines->size(); i++) {
        array<int,3> line = lines->at(i);
        
        int l_y = line[0];
        int bef_x = line[1];
        int aft_x = line[2];

        if (l_y > a[1]) {
            break;
        }

        if ((bef_x == a[0] && l_y == a[1]) || (aft_x == a[0] && l_y == a[1])) {
            return true; // on border
        }

        if (l_y < a[1]) { // is line y < pos y aka is line above pos
            if (between(a[0], bef_x, aft_x)) { //&& (!point_in_shape({a[0], l_y - 1}, lines) || !point_in_shape({a[0], l_y + 1}, lines))) { // is x inside line
                inters += 1;
            }
        }
        else if (l_y == a[1]) { // same y
            // cout << before[1] << "=" << a[1] << endl;
            // cout << "ABC" << a[0] << " " << before[0] << " " << after[0] << endl;
            if (between(a[0], bef_x, aft_x)) {
                return true;
            }
        }
    }

    //cout << a[0] << "," << a[1] << " " << inters << endl;
    
    return (inters % 2) == 1;
}

bool line_horiz_in_shape(int start, int end, int y, const vector<array<int,3>> *lines) {
    for (int i = start; i <= end; i++) {
        if (!(point_in_shape({ i, y }, lines))) {
            //cout << i << "," << y << "not in shape" << endl;
            return false;
        }
        //cout << i << endl;
    }

    return true;
}

bool line_vert_in_shape(int start, int end, int x, const vector<array<int,3>> *lines) {
    for (int i = start; i <= end; i++) {
        if (!(point_in_shape({ x, i }, lines))) {
            //cout << x << "," << i << "not in shape" << endl;
            return false;
        }
        //cout << i << endl;
    }

    return true;
}

bool is_valid(const array<int,2>& a, const array<int,2>& b, const vector<array<int,3>> *lines) {
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

    return (line_horiz_in_shape(l, r, u, lines) && line_horiz_in_shape(l, r, d, lines) &&
        line_vert_in_shape(u, d, l, lines) && line_vert_in_shape(u, d, r, lines));
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

void print_progress(int lines_completed) {
    cout << lines_completed << endl;
    float progress = static_cast<float>(lines_completed) / 496.0;
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

vector<array<int, 3>> prep_lines(const vector<array<int,2>> *data) {
    vector<array<int, 3>> lines;
    lines.reserve(data->size() / 2);
    int count = 0;
    int min_x = 0;
    int max_x = 0;
    int min_y = 0;
    int max_y = 0;

    for (int i = 0; i < data->size() - 1; i++) {
        array<int,2> before = data->at(i);
        array<int,2> after = data->at(i+1);
        
        min_x = min(min_x, min(before[0], after[0]));
        max_x = max(max_x, max(before[0], after[0]));
        min_y = min(min_y, min(before[1], after[1]));
        max_y = max(max_y, max(before[1], after[1]));

        count += calc_area(before, after) - 1;

        if (before[1] == after[1]) { // is line horiz
            array<int,3> l = {before[1], min(before[0], after[0]), max(before[0], after[0])};
            lines.push_back(l);
        }
    }

    std::sort(lines.begin(), lines.end(),
        [](const array<int,3>& a, const array<int,3>& b) {
            return a[0] < b[0];
        }
    );

    cout << calc_area({min_x, min_y}, {max_x, max_y}) / 8 << endl;
    cout << count << endl;
    cout << lines.size() << endl;

    for (auto l: lines) {
        cout << l[1] << "," << l[0] << " " << l[2] << "," << l[0] << endl;
    }

    cout << endl;

    return lines;
}

string AocDay9::part2(string filename, vector<string> extra_args)
{
    vector<array<int,2>> data = read_input(filename);
    data.push_back(data[0]); // add last to end so that it all nicely wraps yipee happiness lala tadah
    int max_area = 0;
    
    vector<array<int,3>> lines = prep_lines(&data);

    // erase_if(data, [](array<int,2> x) {
    //     return x % 2 == 0;
    // });

    //cout << "9,5 2,3" << is_valid({ 9, 5 }, { 2, 3 }, &data) << endl;
    
    for (size_t outer = 0; outer < data.size() - 1; outer++) {
        for (size_t inner = outer + 1; inner < data.size() - 1; inner++) {
            int area = calc_area(data[outer], data[inner]);

            if (area > max_area && is_valid(data[outer], data[inner], &lines)) {
                cout << data[outer][0] << "," << data[outer][1] << " " << data[inner][0] << "," << data[inner][1] << " " << area << endl;
                max_area = area;
            }
        }

        print_progress(outer);
    }

    ostringstream out;
    out << max_area;
    return out.str();
}
