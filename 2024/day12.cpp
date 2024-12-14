#include <algorithm>
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
        return grid;
    }

    std::string line;
    while (getline(file, line))
    {
        std::vector<char> row(line.begin(), line.end());
        grid.push_back(row);
    }

    return grid;
}

void explore_region(const std::vector<std::vector<char>> &grid,
                    std::vector<std::vector<bool>> &visited,
                    std::unordered_map<int, std::vector<int>> &row_info,
                    int row,
                    int col,
                    int &area,
                    int &perimeter,
                    int &sides)
{
    row_info[row].push_back(col);

    for (const auto &[dx, dy] : std::vector<std::tuple<int, int>>{{0, -1}, {1, 0}, {0, 1}, {-1, 0}})
    {
        int nr = row + dy;
        int nc = col + dx;
        if (nr >= 0 && nr < grid.size() && nc >= 0 && nc < grid[row].size())
        {
            if (grid[nr][nc] == grid[row][col])
            {
                perimeter--;
                if (!visited[nr][nc])
                {
                    perimeter += 4;
                    area++;
                    visited[nr][nc] = true;
                    explore_region(grid, visited, row_info, nr, nc, area, perimeter, sides);
                }
            }
        }
    }
}

int count_sides(std::unordered_map<int, std::vector<int>> &row_info)
{
    int sides = 0;
    // column indicies of consecutive plants and their length
    std::vector<std::vector<std::vector<int>>> groups(row_info.size());

    auto row_offset = row_info.begin()->first;
    for (auto &[row, cols] : row_info)
    {
        std::vector<int> plants;
        std::sort(cols.begin(), cols.end());

        plants.push_back(cols[0]);
        bool consecutive = false;
        for (int i = 1; i < cols.size(); i++)
        {
            consecutive = cols[i] - cols[i - 1] == 1;

            if (!consecutive)
            {
                if (!plants.empty())
                    groups[row - row_offset].push_back(plants);
                plants.clear();
            }
            plants.push_back(cols[i]);
        }
        if (!plants.empty())
            groups[row - row_offset].push_back(plants);
    }

    std::unordered_map<int, bool> seen_indices;
    std::vector<int> previous_row;
    int top = 0;
    for (auto &row : groups)
    {
        std::unordered_map<int, bool> previous_row;
        for (auto &group : row)
        {
            bool valid_group = false;
            for (auto &col : group)
            {
                if (seen_indices.find(col) != seen_indices.end())
                {
                    previous_row[col] = true;
                    if (valid_group)
                        top++;
                    valid_group = false;
                    continue;
                }
                else
                {
                    previous_row[col] = true;
                    valid_group = true;
                }
            }
            if (valid_group)
                top++;
        }
        seen_indices = std::move(previous_row);
    }

    seen_indices.clear();
    int bottom = 0;
    for (auto row_it = groups.rbegin(); row_it != groups.rend(); row_it++)
    {
        std::unordered_map<int, bool> previous_row;
        for (auto group_it = row_it->rbegin(); group_it != row_it->rend(); group_it++)
        {
            bool valid_group = false;
            for (auto &col : *group_it)
            {
                if (seen_indices.find(col) != seen_indices.end())
                {
                    previous_row[col] = true;
                    if (valid_group)
                        bottom++;
                    valid_group = false;
                    continue;
                }
                else
                {
                    previous_row[col] = true;
                    valid_group = true;
                }
            }
            if (valid_group)
                bottom++;
        }
        seen_indices = std::move(previous_row);
    }

    seen_indices.clear();
    int left = 0;
    for (auto &row : groups)
    {
        std::unordered_map<int, bool> previous_row;
        for (auto &group : row)
        {
            int start = group[0];
            bool valid_group = false;
            if (seen_indices.find(start) != seen_indices.end())
            {
                previous_row[start] = true;
                if (valid_group)
                    left++;
                valid_group = false;
                continue;
            }
            else
            {
                previous_row[start] = true;
                valid_group = true;
            }
            if (valid_group)
                left++;
        }
        seen_indices = std::move(previous_row);
    }

    seen_indices.clear();
    int right = 0;
    for (auto &row : groups)
    {
        std::unordered_map<int, bool> previous_row;
        for (auto &group : row)
        {
            int end = group[group.size() - 1];
            bool valid_group = false;
            if (seen_indices.find(end) != seen_indices.end())
            {
                previous_row[end] = true;
                if (valid_group)
                    right++;
                valid_group = false;
                continue;
            }
            else
            {
                previous_row[end] = true;
                valid_group = true;
            }
            if (valid_group)
                right++;
        }
        seen_indices = std::move(previous_row);
    }

    return top + bottom + left + right;
}

int main(void)
{
    std::vector<std::vector<char>> grid = read_grid_from_file("input.txt");
    std::vector<std::vector<bool>> visited(grid.size(), std::vector<bool>(grid[0].size(), false));

    int total_price_perimeter = 0;
    int total_price_sides = 0;
    for (int row = 0; row < grid.size(); row++)
    {
        for (int col = 0; col < grid[row].size(); col++)
        {
            if (!visited[row][col])
            {
                std::unordered_map<int, std::vector<int>> row_info;

                visited[row][col] = true;

                char plant = grid[row][col];
                int area = 1;
                int perimeter = 4;
                int sides = 0;

                explore_region(grid, visited, row_info, row, col, area, perimeter, sides);

                sides = count_sides(row_info);

                total_price_sides += sides * area;
                total_price_perimeter += perimeter * area;
            }
        }
    }

    std::cout << total_price_perimeter << " " << total_price_sides << '\n';

    return 0;
}
