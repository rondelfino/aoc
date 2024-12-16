#include <fstream>
#include <iostream>
#include <string>
#include <vector>

// Return minimum cost to get prizex and prizey
// If it cannot be reached return 0
uint64_t solve(int ax, int ay, int bx, int by, uint64_t prizex, uint64_t prizey)
{
    int det = ax * by - ay * bx;
    int64_t a = (int64_t)(prizex * by - prizey * bx) / det;
    int64_t b = (int64_t)(ax * prizey - ay * prizex) / det;

    if (ax * a + bx * b == prizex && ay * a + by * b == prizey)
    {
        return a * 3 + b;
    }
    else
    {
        return 0;
    }
}

int main(void)
{
    std::vector<std::pair<uint64_t, uint64_t>> a;
    std::vector<std::pair<uint64_t, uint64_t>> b;
    std::vector<std::pair<uint64_t, uint64_t>> prize_coordinates;

    std::ifstream file("input.txt");

    uint64_t offset = 10000000000000;
    if (file.is_open())
    {
        std::string line;
        while (std::getline(file, line))
        {
            if (line.find("Button A:") != std::string::npos)
            {
                uint64_t x, y = 0;
                sscanf_s(line.c_str(), "Button A: X+%d, Y+%d", &x, &y);
                a.emplace_back(x, y);
            }
            else if (line.find("Button B:") != std::string::npos)
            {
                uint64_t x, y = 0;
                sscanf_s(line.c_str(), "Button B: X+%d, Y+%d", &x, &y);
                b.emplace_back(x, y);
            }
            else if (line.find("Prize:") != std::string::npos)
            {
                uint64_t x, y = 0;
                sscanf_s(line.c_str(), "Prize: X=%llu, Y=%llu", &x, &y);
                x += offset;
                y += offset;
                prize_coordinates.emplace_back(x, y);
            }
        }
    }

    file.close();

    uint64_t total_cost = 0;
    size_t min_size = std::min({a.size(), b.size(), prize_coordinates.size()});
    for (int i = 0; i < min_size; i++)
    {
        uint64_t x = 0;
        uint64_t y = 0;
        auto [ax, ay] = a[i];
        auto [bx, by] = b[i];
        auto [prizex, prizey] = prize_coordinates[i];

        total_cost += solve(ax, ay, bx, by, prizex, prizey);
    }

    std::cout << total_cost << '\n';

    return 0;
}
