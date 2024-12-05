#include <fstream>
#include <iostream>
#include <string>
#include <vector>

bool is_num(std::string s)
{
    for (int i = 0; i < s.size(); i++)
    {
        if (!isdigit(s[i]))
            return false;
    }
    return true;
}

int main(void)
{
    std::fstream fs("input.txt", std::fstream::in);

    std::vector<std::pair<int, int>> nums;
    if (fs.is_open())
    {
        bool in_dont_context = false;
        std::string line;
        while (std::getline(fs, line))
        {
            for (int i = 0; i < line.size(); i++)
            {
                if (i + 6 < line.size() && line.substr(i, 7) == "don't()")
                {
                    in_dont_context = true; // Enter "don't()" context
                    i += 7;                 // Skip past "don't()"
                }
                else if (i + 3 < line.size() && line.substr(i, 4) == "do()")
                {
                    in_dont_context = false; // Leave "don't()" context
                    i += 4;                  // Skip past "do()"
                }

                if (!in_dont_context && i + 3 < line.size() && line.substr(i, 4) == "mul(")
                {
                    int j = i + 3;
                    while (j++ < line.size())
                    {
                        if (line[j] == ')')
                        {
                            break;
                        }
                    }

                    std::string num1;
                    std::string num2;
                    int k = i + 3;
                    for (k += 1; k < j + 1 && line[k] != ','; k++)
                    {
                        num1 += line[k];
                    }

                    for (k += 1; k < j; k++)
                    {
                        num2 += line[k];
                    }

                    if (is_num(num1) && is_num(num2))
                    {
                        int x = std::stoi(num1);
                        int y = std::stoi(num2);
                        std::pair<int, int> pair = {x, y};
                        nums.push_back(pair);
                    }
                }
            }
        }

        fs.close();
    }

    int sum = 0;
    for (int i = 0; i < nums.size(); i++)
    {
        sum += nums[i].first * nums[i].second;
    }
    std::cout << sum << '\n';
}
