#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

int main(void)
{
    unsigned int total_distance = 0;
    std::vector<int> list0;
    std::vector<int> list1;

    std::fstream fs;
    fs.open("input.txt", std::fstream::in);

    if (fs.is_open())
    {
        std::string line;
        while (std::getline(fs, line))
        {
            std::istringstream ss(line);
            std::string col1;
            std::string col2;
            while (ss >> col1 >> col2)
            {
                list1.push_back(std::stoi(col1));
                list0.push_back(std::stoi(col2));
            }
        }
        fs.close();
    }
    std::sort(list0.begin(), list0.end());
    std::sort(list1.begin(), list1.end());

    // Part 1
    // assume same size list
    auto it0 = list0.begin();
    auto it1 = list1.begin();
    while (it0 != list0.end() && it1 != list1.end())
    {
        total_distance += *it0 > *it1 ? *it0++ - *it1++ : *it1++ - *it0++;
    }

    // Part 2
    unsigned int similarity_score = 0;
    for (auto it0 = list0.begin(); it0 != list0.end(); it0++)
    {
        unsigned int count = 0;
        for (auto it1 = list1.begin(); it1 != list1.end(); it1++)
        {
            count += *it0 == *it1 ? 1 : 0;
        }
        similarity_score += *it0 * count;
    }

    std::cout << "Total Distance: " << total_distance << " Similarity Score: " << similarity_score << "\n";
}
