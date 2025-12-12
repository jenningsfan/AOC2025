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
    struct hash<std::array<long,3>> {
        size_t operator()(std::array<long,3> const& a) const noexcept {
            size_t h = 0;
            for (long v : a) {
                h ^= std::hash<long>{}(v) + 0x9e3779b97f4a7c15ULL + (h << 6) + (h >> 2);
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

vector<array<long,2>> AocDay9::read_input(string filename)
{
    FileUtils fileutils;
    vector<vector<long>> data;
    vector<array<long,2>> converted_data;

    if (!fileutils.read_as_list_of_split_longs(filename, data, ',', '\0', '\0'))
    {
        cerr << "Error reading in the data from " << filename << endl;
        return converted_data;
    }
    
    for (const auto& obj : data) {
        converted_data.push_back({ 
            static_cast<long>(obj[0]),
            static_cast<long>(obj[1]),
        });
    }

    return converted_data;
}

bool between(long val, long start, long end) {
    return val >= min(start, end) && val <= max(start, end);
}

long calc_area(const array<long,2>& a, const array<long,2>& b) {
    return abs((a[0] - b[0] + 1) * (a[1] - b[1] + 1));
}

bool point_in_shape(const array<long,2>& a, const vector<array<long,2>> *lines) {
    long inters = 0;

    for (long i = 0; i < lines->size() - 1; i++) {
        array<long,2> before = lines->at(i);
        array<long,2> after = lines->at(i+1);
        
        if ((before[0] == a[0] && before[1] == a[1]) || (after[0] == a[0] && after[1] == a[1])) {
            return true; // on border
        }

        if (before[1] == after[1]) { // is line horiz
            if (before[1] < a[1]) { // is line y < pos y aka is line above pos
                if (between(a[0], before[0], after[0])) { // is x inside line
                    inters += 1;
                }
            }
            else if (before[1] == a[1]) { // same y
                // cout << before[1] << "=" << a[1] << endl;
                // cout << "ABC" << a[0] << " " << before[0] << " " << after[0] << endl;
                if (between(a[0], before[0], after[0])) {
                    return true;
                }
            }
        }
    }

    //cout << a[0] << "," << a[1] << " " << inters << endl;
    
    return (inters % 2) == 1;
}

bool line_horiz_in_shape(long start, long end, long y, const vector<array<long,2>> *lines) {
    for (long i = start; i <= end; i++) {
        if (!(point_in_shape({ i, y }, lines))) {
            //cout << i << "," << y << "not in shape" << endl;
            return false;
        }
        //cout << i << endl;
    }

    return true;
}

bool line_vert_in_shape(long start, long end, long x, const vector<array<long,2>> *lines) {
    for (long i = start; i <= end; i++) {
        if (!(point_in_shape({ x, i }, lines))) {
            //cout << x << "," << i << "not in shape" << endl;
            return false;
        }
        //cout << i << endl;
    }

    return true;
}

bool is_valid(const array<long,2>& a, const array<long,2>& b, const vector<array<long,2>> *lines) {
    long u = min(a[1], b[1]);
    long d = max(a[1], b[1]);
    long l = min(a[0], b[0]);
    long r = max(a[0], b[0]);

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
    vector<array<long,2>> data = read_input(filename);
    long max_area = 0;
        
    for (size_t outer = 0; outer < data.size(); outer++) {
        for (size_t inner = outer + 1; inner < data.size(); inner++) {
            long area = calc_area(data[outer], data[inner]);

            if (area > max_area) {
                max_area = area;
            }
        }
    }
    
    ostringstream out;
    out << max_area;
    return out.str();
}

void print_progress(long lines_completed) {
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

string AocDay9::part2(string filename, vector<string> extra_args)
{
    vector<array<long,2>> data = read_input(filename);
    data.push_back(data[0]); // add last to end so that it all nicely wraps yipee happiness lala tadah
    long max_area = 0;
    
    // erase_if(data, [](array<long,2> x) {
    //     return x % 2 == 0;
    // });

    //cout << "9,5 2,3" << is_valid({ 9, 5 }, { 2, 3 }, &data) << endl;
    
   #pragma omp parallel for schedule(dynamic)
    for (size_t outer = 0; outer < data.size() - 1; outer++) {
        long local_max = 0;

        for (size_t inner = outer + 1; inner < data.size() - 1; inner++) {
            long area = calc_area(data[outer], data[inner]);

            if (area > local_max && is_valid(data[outer], data[inner], &data)) {
                cout << data[outer][0] << "," << data[outer][1] << " " << data[inner][0] << "," << data[inner][1] << " " << area << endl;
                local_max = area;
            }
        }

        #pragma omp critical
        {
            if (local_max > max_area) {
                max_area = local_max;
            }
        }

        print_progress(outer);
    }

    ostringstream out;
    out << max_area;
    return out.str();
}
