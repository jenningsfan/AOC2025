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

#include "aoc_day_8.h"
#include "file_utils.h"

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

AocDay8::AocDay8():AocDay(8)
{
}

AocDay8::~AocDay8()
{
}

vector<array<long,3>> AocDay8::read_input(string filename)
{
    FileUtils fileutils;
    vector<vector<long>> data;
    vector<array<long,3>> converted_data;

    if (!fileutils.read_as_list_of_split_longs(filename, data, ',', '\0', '\0'))
    {
        cerr << "Error reading in the data from " << filename << endl;
        return converted_data;
    }
    
    for (const auto& obj : data) {
        converted_data.push_back({ 
            static_cast<long>(obj[0]),
            static_cast<long>(obj[1]),
            static_cast<long>(obj[2])
        });
    }

    return converted_data;
}

long calc_distance(const array<long,3>& a, const array<long,3>& b) {
    return static_cast<long>(
        sqrt(
            pow(a[0] - b[0], 2) +
            pow(a[1] - b[1], 2) +
            pow(a[2] - b[2], 2)
        )
    );
}

long delete_biggest_distance(vector<tuple<array<long,3>, array<long,3>>>* list) {
    long max_distance = 0;
    long max_id = -1;

    for (long i = 0; i < list->size(); i++) {
        long distance = calc_distance(std::get<0>((*list)[i]), std::get<1>((*list)[i]));
        if (distance > max_distance) {
            max_distance = distance;
            max_id = i;
        }
    }

    list->erase(list->begin() + max_id);
    return max_distance;
}

template <typename T>
vector<size_t> sort_indexes(const vector<T> &v) {

    // initialize original index locations
    vector<size_t> idx(v.size());
    iota(idx.begin(), idx.end(), 0);

    // sort indexes based on comparing values in v
    // using std::stable_sort instead of std::sort
    // to avoid unnecessary index re-orderings
    // when v contains elements of equal values 
    stable_sort(idx.begin(), idx.end(),
       [&v](size_t i1, size_t i2) {return v[i1] < v[i2];});

    return idx;
}

string AocDay8::part1(string filename, vector<string> extra_args)
{
    vector<array<long,3>> data = read_input(filename);
    vector<tuple<array<long,3>, array<long,3>>> min_distances;

    long max_distance = 0;
    
    for (size_t outer = 0; outer < data.size(); outer++) {
        for (size_t inner = outer + 1; inner < data.size(); inner++) {
            long distance = calc_distance(data[outer], data[inner]);

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

    for (auto elem: min_distances) {
        cout << get<0>(elem)[0] << " " << get<1>(elem)[0] << endl;
    }

    // cout << calc_distance({162,817,812}, {425,690,689}) << endl;

    vector<unordered_set<array<long, 3>>> circuits;
    for (auto conn: min_distances) {
        long first_id = -1;
        long second_id = -1;

        for (long i = 0; i < circuits.size(); i++) {
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
            auto temp = unordered_set<array<long, 3>>();
            temp.insert(get<0>(conn));
            temp.insert(get<1>(conn));
            circuits.push_back(temp);
        }
    }

    vector<size_t> mapped(circuits.size());
    transform(circuits.begin(), circuits.end(), mapped.begin(),
         [](unordered_set<array<long, 3>> x) { return x.size(); });
    nth_element(mapped.begin(), mapped.end() - 3, mapped.end());

    ostringstream out;
    out << mapped[mapped.size() - 1] * mapped[mapped.size() - 2] * mapped[mapped.size() - 3];
    return out.str();
}

string AocDay8::part2(string filename, vector<string> extra_args)
{
    vector<array<long,3>> data = read_input(filename);
    vector<long> distances = vector<long>(data.size() * data.size());
    
    for (size_t outer = 0; outer < data.size(); outer++) {
        for (size_t inner = outer + 1; inner < data.size(); inner++) {
            long distance = calc_distance(data[outer], data[inner]);
            distances[outer * data.size() + inner] = distance;
        }
    }

    cout << distances.size() << endl;
    vector<size_t> min_indexs = sort_indexes(distances);

    cout << min_indexs.size() << endl;

    vector<unordered_set<array<long, 3>>> circuits;
    for (auto index: min_indexs) {
        array<long, 3> outer = data[index / data.size()];
        array<long, 3> inner = data[index % data.size()];
        
        if (distances[index] == 0) {
            continue;
        }

        cout << outer[0] << "," << outer[1] << "," << outer[2];
        cout << " " << inner[0] << "," << inner[1] << "," << inner[2];
        cout << " " << distances[index] << " " << index << endl;

        long first_id = -1;
        long second_id = -1;

        for (long i = 0; i < circuits.size(); i++) {
            if (circuits[i].contains(outer)) {
                first_id = i;
            }
            if (circuits[i].contains(inner)) {
                second_id = i;
            }
        }

        if (first_id != -1 && second_id != -1 && first_id != second_id) { // in both
            circuits[first_id].insert(outer);
            circuits[first_id].insert(inner);
            circuits[first_id].merge(circuits[second_id]);
            circuits.erase(circuits.begin() + second_id);
        }
        else if (first_id != -1) {
            circuits[first_id].insert(outer);
            circuits[first_id].insert(inner);
        }
        else if (second_id != -1) {
            circuits[second_id].insert(outer);
            circuits[second_id].insert(inner);
        }
        else {
            auto temp = unordered_set<array<long, 3>>();
            temp.insert(outer);
            temp.insert(inner);
            circuits.push_back(temp);
        }

        if (circuits.size() == 1 && circuits[0].size() == data.size()) {
            ostringstream out;
            out << outer[0] * inner[0];
            return out.str();
        }
    }

    cout << "hellooo" << endl;

    for (auto c: circuits) {
        cout << c.size() << endl;
    }

    ostringstream out;
    out << "NO ANSWER FOUND!!!";
    return out.str();
}
