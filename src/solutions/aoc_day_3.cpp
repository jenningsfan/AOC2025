#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <tuple>

#include "aoc_day_3.h"
#include "file_utils.h"

using namespace std;

AocDay3::AocDay3():AocDay(3)
{
}

AocDay3::~AocDay3()
{
}

vector<string> AocDay3::read_input(string filename)
{
    FileUtils fileutils;
    vector<string> raw_lines;
    if (!fileutils.read_as_list_of_strings(filename, raw_lines))
    {
        cerr << "Error reading in the data from " << filename << endl;
        return vector<string>();
    }
    
    return raw_lines;
}

tuple<long, long> max_val(string line, size_t start, size_t end) {
    if (end >= line.size()) {
        end = line.size();
    }

    long max = -1;
    size_t max_i = 0;
    for(int i = start; i < end; i++) {
        long val = line[i] - '0';
        if (val > max) {
            max = val;
            max_i = i;
        }
    }

    return tuple<long, long>(max, max_i);
}

long max_val_p1(string line) {
    long max_start = -1;
    size_t max_i = 0;
    for(int i = 0; i < line.size() - 2; i++) {
        long val = line[i] - '0';
        //cout << val;
        //cout << val << endl;
        if (val > max_start) {
            max_start = val;
            max_i = i;
        }
    }

    //cout << endl;
   // cout << max_i << endl;

    long max_end = 0;
    for(int i = max_i + 1; i < line.size(); i++) {
        long val = line[i] - '0';
        if (val > max_end) {
            max_end = val;
            max_i = i;
        }
    }

    return max_start * 10 + max_end;
}

long max_val_p2(string line, bool debug) {
    string got = "";
    size_t check_pos = -1;

    while (got.size() < 12) {
        size_t check = (line.size() - check_pos) - (12 - got.size()) + 1;
        
        tuple<long, long> found = max_val(line, check_pos + 1, check_pos + check);
        got += std::to_string(std::get<0>(found));
        check_pos = std::get<1>(found);
        //cout << "got " << std::to_string(std::get<0>(found)) << "check pos " << check_pos << endl;
        if (debug) {
            cout << (line.size()) << " " << check_pos << " " << 12 - got.size() << endl;
            cout << "check " << check << "got " << std::get<0>(found) << "check pos" << check_pos << endl;
        }
    }

    return stol(got);
}

string AocDay3::part1(string filename, vector<string> extra_args)
{
    vector<string> data = read_input(filename);
    long sum = 0;
    for (vector<string>::iterator iter = data.begin(); iter != data.end(); ++iter)
    {
        sum += max_val_p1(*iter);
    }
    ostringstream out;
    out << sum;
    return out.str();
}

string AocDay3::part2(string filename, vector<string> extra_args)
{
    vector<string> data = read_input(filename);
    long sum = 0;
    bool debug = true;
    for (vector<string>::iterator iter = data.begin(); iter != data.end(); ++iter)
    {
        for (unsigned char c : *iter) {
            cout << "[" << int(c) << "]";
        }

        cout << *iter << endl;
        cout << "811111111111119" << endl;
        cout << iter->compare("811111111111119") << endl;
        cout << max_val_p2(*iter, iter->compare("811111111111119") == 0) << endl;
        debug = false;
        sum += max_val_p2(*iter, debug);
    }
    ostringstream out;
    out << sum;
    return out.str();
}
