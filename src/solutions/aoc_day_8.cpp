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

#include "aoc_day_8.h"
#include "file_utils.h"

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

AocDay8::AocDay8():AocDay(8)
{
}

AocDay8::~AocDay8()
{
}

vector<array<int,3>> AocDay8::read_input(string filename)
{
    FileUtils fileutils;
    vector<vector<long>> data;
    vector<array<int,3>> converted_data;

    if (!fileutils.read_as_list_of_split_longs(filename, data, ',', '\0', '\0'))
    {
        cerr << "Error reading in the data from " << filename << endl;
        return converted_data;
    }
    
    for (const auto& obj : data) {
        converted_data.push_back({ 
            static_cast<int>(obj[0]),
            static_cast<int>(obj[1]),
            static_cast<int>(obj[2])
        });
    }

    return converted_data;
}

int calc_distance(const array<int,3>& a, const array<int,3>& b) {
    return static_cast<int>(
        sqrt(
            pow(a[0] - b[0], 2) +
            pow(a[1] - b[1], 2) +
            pow(a[2] - b[2], 2)
        )
    );
}

int delete_biggest_distance(vector<tuple<array<int,3>, array<int,3>>>* list) {
    int max_distance = 0;
    int max_id = -1;

    for (int i = 0; i < list->size(); i++) {
        int distance = calc_distance(std::get<0>((*list)[i]), std::get<1>((*list)[i]));
        if (distance > max_distance) {
            max_distance = distance;
            max_id = i;
        }
    }

    list->erase(list->begin() + max_id);
    return max_distance;
}

string AocDay8::part1(string filename, vector<string> extra_args)
{
    vector<array<int,3>> data = read_input(filename);
    vector<tuple<array<int,3>, array<int,3>>> min_distances;

    int max_distance = 0;
    
    for (size_t outer = 0; outer < data.size(); outer++) {
        for (size_t inner = outer + 1; inner < data.size(); inner++) {
            int distance = calc_distance(data[outer], data[inner]);
            // cout << data[outer][0] << "," << data[outer][1] << "," << data[outer][2];
            // cout << " and ";
            // cout << data[inner][0] << "," << data[inner][1] << "," << data[inner][2];
            // cout << " " << distance;
            // cout << " " << max_distance;
            // cout << endl;

            if (min_distances.size() < 1000) {
                min_distances.emplace_back(data[outer], data[inner]);
                if (distance > max_distance) {
                    max_distance = distance;
                }
                continue;
            }

            if (distance < max_distance) {
                min_distances.emplace_back(data[outer], data[inner]);
                max_distance = delete_biggest_distance(&min_distances);
            }
        }
    }

    // cout << endl << endl;

    // for (auto elem: min_distances) {
    //     cout << get<0>(elem)[0] << "," << get<0>(elem)[1] << "," << get<0>(elem)[2];
    //     cout << " and ";
    //     cout << get<1>(elem)[0] << "," << get<1>(elem)[1] << "," << get<1>(elem)[2];
    //     cout << " " << calc_distance(get<0>(elem), get<1>(elem));
    //     cout << endl;
    // }

    // cout << calc_distance({162,817,812}, {425,690,689}) << endl;

    vector<unordered_set<array<int, 3>>> circuits;
    for (auto conn: min_distances) {
        int first_id = -1;
        int second_id = -1;

        for (int i = 0; i < circuits.size(); i++) {
            if (circuits[i].contains(get<0>(conn))) {
                first_id = i;
            }
            if (circuits[i].contains(get<1>(conn))) {
                second_id = i;
            }
        }

        if (first_id != -1 && second_id != -1 && first_id != second_id) { // in both
            circuits[first_id].insert(get<0>(conn));
            circuits[first_id].insert(get<1>(conn));
            circuits[first_id].merge(circuits[second_id]);
            circuits.erase(circuits.begin() + second_id);
        }
        else if (first_id != -1) {
            circuits[first_id].insert(get<0>(conn));
            circuits[first_id].insert(get<1>(conn));
        }
        else if (second_id != -1) {
            circuits[second_id].insert(get<0>(conn));
            circuits[second_id].insert(get<1>(conn));
        }
        else {
            auto temp = unordered_set<array<int, 3>>();
            temp.insert(get<0>(conn));
            temp.insert(get<1>(conn));
            circuits.push_back(temp);
        }
    }

    vector<size_t> mapped(circuits.size());
    transform(circuits.begin(), circuits.end(), mapped.begin(),
         [](unordered_set<array<int, 3>> x) { return x.size(); });
    nth_element(mapped.begin(), mapped.end() - 3, mapped.end());

    ostringstream out;
    out << mapped[mapped.size() - 1] * mapped[mapped.size() - 2] * mapped[mapped.size() - 3];
    return out.str();
}

string AocDay8::part2(string filename, vector<string> extra_args)
{
    vector<array<int,3>> data = read_input(filename);
    long sum = 0;
    
    ostringstream out;
    out << sum;
    return out.str();
}
