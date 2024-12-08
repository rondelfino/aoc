#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

std::vector<std::vector<char>> read_grid_from_file(const std::string &filename)
{
    std::ifstream file(filename);
    std::vector<std::vector<char>> grid;

    if (!file.is_open())
    {
        std::cerr << "Error: Unable to open file " << filename << '\n';
        return grid; // Return empty grid if file cannot be opened
    }

    std::string line;
    while (getline(file, line))
    {
        std::vector<char> row(line.begin(), line.end());
        grid.push_back(row);
    }

    return grid;
}

int add_antinodes1(std::vector<std::vector<char>> &grid, std::pair<int, int> c1, std::pair<int, int> c2, int dx, int dy)
{
    int new_x0 = c1.first + dx;
    int new_y0 = c1.second + dy;
    int new_x1 = c2.first - dx;
    int new_y1 = c2.second - dy;

    int unique_antinode_count = 0;
    if (new_x0 < grid[c1.first].size() && new_y0 < grid.size())
    {
        if (grid[new_y0][new_x0] != '#')
        {
            grid[new_y0][new_x0] = '#';
            unique_antinode_count++;
        }
    }

    if (new_x1 < grid[c1.first].size() && new_y1 < grid.size())
    {
        if (grid[new_y1][new_x1] != '#')
        {
            grid[new_y1][new_x1] = '#';
            unique_antinode_count++;
        }
    }

    return unique_antinode_count;
}

int add_antinodes2(std::vector<std::vector<char>> &grid, std::pair<int, int> c1, std::pair<int, int> c2, int dx, int dy)
{
    int new_x0 = c1.first + dx;
    int new_y0 = c1.second + dy;
    int new_x1 = c2.first - dx;
    int new_y1 = c2.second - dy;

    int unique_antinode_count = 0;
    for (int i = c1.second, j = c1.first; i < grid.size() && j < grid[c1.first].size(); i += dy, j += dx)
    {
        if (grid[i][j] != '#')
        {
            grid[i][j] = '#';
            unique_antinode_count++;
        }
    }

    for (int i = c2.second, j = c2.first; i < grid.size() && j < grid[c2.first].size(); i -= dy, j -= dx)
    {
        if (grid[i][j] != '#')
        {
            grid[i][j] = '#';
            unique_antinode_count++;
        }
    }

    return unique_antinode_count;
}

int main(void)
{
    std::vector<std::vector<char>> grid = read_grid_from_file("input.txt");
    std::unordered_map<char, std::vector<std::pair<int, int>>> antenna_map;

    for (int i = 0; i < grid.size(); i++)
    {
        for (int j = 0; j < grid[i].size(); j++)
        {
            if (grid[i][j] != '.')
            {
                antenna_map[grid[i][j]].push_back({j, i});
            }
        }
    }

    int unique_antinode_count1 = 0;
    int unique_antinode_count2 = 0;
    for (const auto &pair : antenna_map)
    {
        const auto &v = pair.second;
        for (size_t i = 0; i < v.size(); i++)
        {
            for (size_t j = i + 1; j < v.size(); j++)
            {
                int dx = v[i].first - v[j].first;
                int dy = v[i].second - v[j].second;
                // Part 1
                // unique_antinode_count1 += add_antinodes1(grid, v[i], v[j], dx, dy);
                // Part 2
                unique_antinode_count2 += add_antinodes2(grid, v[i], v[j], dx, dy);
            }
        }
    }

    std::cout << unique_antinode_count1 << " " << unique_antinode_count2 << '\n';

    return 0;
}
