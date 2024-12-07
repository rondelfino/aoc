#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

bool evaluate(uint64_t current_total, uint64_t target_value, int index, const std::vector<int> &operands)
{
    if (index >= operands.size())
        return current_total == target_value;

    uint64_t next_operand = operands[index];
    return evaluate(current_total + next_operand, target_value, index + 1, operands) ||
           evaluate(current_total * next_operand, target_value, index + 1, operands) ||
           evaluate(std::stoull(std::to_string(current_total) + std::to_string(next_operand)), target_value, index + 1,
                    operands);
}

int main(void)
{
    uint64_t total = 0;

    std::fstream fs;
    fs.open("input.txt", std::fstream::in);
    if (fs.is_open())
    {
        std::string line;
        while (std::getline(fs, line))
        {
            std::stringstream ss(line);

            uint64_t target_value = 0;
            uint64_t val = 0;
            char delim;
            std::vector<int> operands;

            ss >> target_value;
            ss >> delim;
            while (ss >> val)
                operands.push_back(val);

            if (!operands.empty() && evaluate(operands[0], target_value, 1, operands))
                total += target_value;
        }
    }

    std::cout << total << '\n';

    return 0;
}
