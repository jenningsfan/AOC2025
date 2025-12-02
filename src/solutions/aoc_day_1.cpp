#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <cstdlib>

#include "aoc_day_1.h"
#include "file_utils.h"

using namespace std;

AocDay1::AocDay1():AocDay(1)
{
}

AocDay1::~AocDay1()
{
}

vector<string> AocDay1::read_input(string filename)
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

string AocDay1::part1(string filename, vector<string> extra_args)
{
    vector<string> data = read_input(filename);
    long total = 0;
    short dial = 50;

    for (vector<string>::iterator rot = data.begin(); rot != data.end(); ++rot) {
        short dir = (*rot)[0] == 'L' ? -1 : 1;
        short change = static_cast<short>(std::stoi(rot->substr(1))) * dir;
        dial += change;
        dial %= 100;
        if (dial < 0) {
            dial = 100 + dial;
        }
        if (dial == 0) {
            total += 1;
        }
    }

    ostringstream out;
    out << total;
    return out.str();
}

string AocDay1::part2(string filename, vector<string> extra_args)
{   
    vector<string> data = read_input(filename);
    long total = 0;
    long dial = 50;
    long old_dial = dial;

    for (vector<string>::iterator rot = data.begin(); rot != data.end(); ++rot) {
        long dir = (*rot)[0] == 'L' ? -1 : 1;
        long change = static_cast<long>(std::stoi(rot->substr(1)));
        
        for (long i = 0; i < change; i++) {
            dial += dir;
            if (dial >= 100) {
                dial %= 100;
            }
            else if (dial < 0) {
                dial = 100 + dial;
            }
            if (dial == 0) {
                total += 1;
            }
        }

        cout << dial << endl;
    }
    
    ostringstream out;
    out << total;
    return out.str();
}
