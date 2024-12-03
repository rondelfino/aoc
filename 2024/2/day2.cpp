#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

bool is_safe_report(const std::vector<int> &report)
{
    if (report.size() < 2)
        return true;

    int difference = report[1] - report[0];
    bool increasing = difference > 0;
    bool safe = std::abs(difference) >= 0 && std::abs(difference) <= 3;
    for (size_t i = 1; i < report.size() && safe; i++)
    {
        difference = report[i] - report[i - 1];
        bool valid_order = (difference > 0) == increasing;
        safe = std::abs(difference) >= 0 && std::abs(difference) <= 3 && valid_order;
    }
    return safe;
}

bool dampen_problem(const std::vector<int> &report)
{
    bool safe = true;
    for (size_t i = 0; i < report.size() && safe; i++)
    {
        std::vector<int> test(report);
        test.erase(test.begin() + i);
        safe = is_safe_report(test);
    }
    return safe;
}

int main(void)
{
    std::fstream fs("input.txt", std::fstream::in);

    unsigned int safe_report_count = 0;
    if (fs.is_open())
    {
        std::string line;
        while (std::getline(fs, line))
        {
            std::vector<int> report;
            std::istringstream ss(line);
            int level;
            while (ss >> level)
            {
                report.push_back(level);
            }

            if (is_safe_report(report) || dampen_problem(report))
            {
                safe_report_count++;
            }
        }

        std::cout << safe_report_count << "\n";
        fs.close();
    }
}
