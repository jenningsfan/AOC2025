#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <unordered_set>
#include <numeric>

#include "aoc_day_2.h"
#include "file_utils.h"

using namespace std;

AocDay2::AocDay2():AocDay(2)
{
}

AocDay2::~AocDay2()
{
}

vector<string> AocDay2::read_input(string filename)
{
    FileUtils fileutils;
    string raw;
    ifstream infile(filename);
    if (!infile)
    {
        cerr << "*****Error opening file " << filename << endl;
        return vector<string>();
    }
    getline(infile, raw);
    vector<string> ranges = fileutils.split_line_to_strings(raw, ',', '\0', '\0');
    return ranges;
}

void find_for_range(string start, string end, long modulus, unordered_set<long> *vals) {
    // do we need to split them ??? (so comparisions alsways have the same length)
    //cout << "start " << start << "end " << end << endl;
    size_t size_dif = end.size() - start.size();
    if (size_dif != 0) {
        // we need to split
        if (size_dif != 1) { cerr << "OH WE DO HAVE TO HANDLE THIS NOO SADNESS ah well not too bad" << endl; }
        find_for_range(start, std::string(start.size(), '9'), modulus, vals);
        find_for_range('1' + std::string(start.size(), '0'), end, modulus, vals);
    }

    size_t size = start.size();

    // is it an even length bc it needs to be even
    if (size % modulus != 0) {
        return;
    }

    long start_l = stol(start);
    long end_l = stol(end);
    string start_half = start.substr(0, size / modulus);
    string end_half = end.substr(0, size / modulus);

    for (long i = stol(start_half); i <= stol(end_half); i++) {
        string full_s;
        for (long j = 0; j < modulus; j++) { full_s += std::to_string(i); }
        long full = stol(full_s);

        if (start_l <= full && end_l >= full) {
            cout << full << endl;
            vals->insert(full);
        }
    }
}

long calc_for_range(string start, string end, long modulus) {
    unordered_set<long> vals;
    find_for_range(start, end, modulus, &vals);
    return std::accumulate(vals.begin(), vals.end(), 0L);
}

string AocDay2::part1(string filename, vector<string> extra_args)
{
    vector<string> data = read_input(filename);
    long total = 0;
    for (vector<string>::iterator rot = data.begin(); rot != data.end(); ++rot) {
        string range = (*rot);
        string start = range.substr(0, range.find("-"));
        string end = range.substr(range.find("-") + 1, range.length());
        total += calc_for_range(start, end, 2);
    }

    ostringstream out;
    out << total;
    return out.str();
}

string AocDay2::part2(string filename, vector<string> extra_args)
{   
    vector<string> data = read_input(filename);
    unordered_set<long> vals;

    for (vector<string>::iterator rot = data.begin(); rot != data.end(); ++rot) {
        string range = (*rot);
        string start = range.substr(0, range.find("-"));
        string end = range.substr(range.find("-") + 1, range.length());
        
        for (long i = 2; i <= end.size(); i++) {
            find_for_range(start, end, i, &vals);
        }
    }

    for (const long& x : vals) {
        std::cout << x << "\n";
    }

    ostringstream out;
    out << std::accumulate(vals.begin(), vals.end(), 0L);
    return out.str();
}
