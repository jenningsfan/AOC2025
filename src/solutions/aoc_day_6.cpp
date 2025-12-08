#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <tuple>
#include <algorithm>
#include <ranges>

#include "aoc_day_6.h"
#include "file_utils.h"

using namespace std;

AocDay6::AocDay6():AocDay(6)
{
}

AocDay6::~AocDay6()
{
}

vector<string> split_ws(const string& s) {
    istringstream iss(s);
    vector<string> tokens;
    string w;
    while (iss >> w) tokens.push_back(move(w));
    return tokens;
}

tuple<vector<long>, vector<long>, vector<long>, vector<long>, vector<bool>> AocDay6::read_input_p1(string filename)
{
    FileUtils fileutils;
    vector<string> raw_lines;

    if (!fileutils.read_as_list_of_strings(filename, raw_lines))
    {
        cerr << "Error reading in the data from " << filename << endl;
        raw_lines = vector<string>();
    }
    
    vector<long> nums1;
    vector<long> nums2;
    vector<long> nums3;
    vector<long> nums4;
    vector<bool> ops; // + = false, * = true

    vector<string> split0 = split_ws(raw_lines[0]);
    vector<string> split1 = split_ws(raw_lines[1]);
    vector<string> split2 = split_ws(raw_lines[2]);
    vector<string> split3 = split_ws(raw_lines[3]);
    vector<string> split4 = split_ws(raw_lines[4]);

    for (long i = 0; i < split0.size(); i++) {
        cout << split0[i] << " " << split1[i] << " " << split2[i] << " " << split3[i] << " " << split4[i] << endl;
        nums1.push_back(stoi(split0[i]));
        nums2.push_back(stoi(split1[i]));
        nums3.push_back(stoi(split2[i]));
        nums4.push_back(stoi(split3[i]));
        ops.push_back(split4[i] == "*");
    }

    return tuple(nums1, nums2, nums3, nums4, ops);
}

vector<string> AocDay6::read_input_p2(string filename)
{
    FileUtils fileutils;
    vector<string> raw_lines;

    if (!fileutils.read_as_list_of_strings(filename, raw_lines))
    {
        cerr << "Error reading in the data from " << filename << endl;
        raw_lines = vector<string>();
    }
    
    return raw_lines;
}

string AocDay6::part1(string filename, vector<string> extra_args)
{
    auto [nums1, nums2, nums3, nums4, ops] = read_input_p1(filename);
    long sum = 0;

    for (long i = 0; i < nums1.size(); i++) {
        if (ops[i]) { // + = false, * = true
            sum += nums1[i] * nums2[i] * nums3[i] * nums4[i];
        }
        else {
            sum += nums1[i] + nums2[i] + nums3[i] + nums4[i];
        }
    }

    ostringstream out;
    out << sum;
    return out.str();
}

string AocDay6::part2(string filename, vector<string> extra_args)
{
    vector<string> input = read_input_p2(filename);
    string nums1 = input[0];
    string nums2 = input[1];
    string nums3 = input[2];
    string nums4 = input[3];
    string ops = input[4];

    long sum = 0;
    bool op;
    long curr = 0;

    for (size_t i = 0; i < nums1.size(); i++) {
        string big_line = string {nums1[i], nums2[i], nums3[i], nums4[i]};

        if (big_line == "    ") {
            sum += curr;
            curr = 0;
            continue;
        }

        if (ops[i] == '*') {
            op = true; 
        }
        else if (ops[i] == '+') {
            op = false;
        }

        if (op) { // + = false, * = true
            if (curr == 0) {
                curr = 1;
            }
            curr *= stol(big_line);
        }
        else {
            curr += stol(big_line);
        }
    }

    sum += curr;

    ostringstream out;
    out << sum;
    return out.str();
}
