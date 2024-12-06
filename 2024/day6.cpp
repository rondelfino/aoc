#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <tuple>
#include <vector>

std::vector<std::pair<int, int>> directions = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}};

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

std::pair<int, int> get_starting_pos(std::vector<std::vector<char>> &grid)
{
    for (size_t i = 0; i < grid.size(); i++)
    {
        for (size_t j = 0; j < grid[i].size(); j++)
        {
            if (grid[i][j] == '^')
            {
                return {j, i};
            }
        }
    }

    return {-1, -1};
}

int count_unique_positions(std::vector<std::vector<char>> &grid, std::pair<int, int> starting_pos)
{

    int current_direction_index = 0;
    std::pair<int, int> current_direction = directions[current_direction_index];
    int i = starting_pos.second;
    int j = starting_pos.first;

    int unique_position_count = 0;
    while (i >= 0 && i < grid.size() && j >= 0 && j < grid[i].size())
    {
        if (grid[i][j] != 'X' && grid[i][j] != '#')
        {
            grid[i][j] = 'X';
            unique_position_count++;
        }
        else if (grid[i][j] == '#')
        {
            // Move one step back
            i -= current_direction.second;
            j -= current_direction.first;

            // Change direction
            current_direction_index = (current_direction_index + 1) % directions.size();
            current_direction = directions[current_direction_index];
        }

        i += current_direction.second;
        j += current_direction.first;
    }

    return unique_position_count;
}

int count_blocking_obstacles(std::vector<std::vector<char>> &grid, std::pair<int, int> starting_pos)
{
    int blocking_obstacles = 0;

    for (int y = 0; y < grid.size(); y++)
    {
        for (int x = 0; x < grid[y].size(); x++)
        {
            if (grid[y][x] != '#')
            {
                char temp = grid[y][x];
                grid[y][x] = '#';

                // Attempt to traverse and detect loops
                std::set<std::tuple<int, int, int>> visited_states;
                bool loop_detected = false;

                int current_direction_index = 0;
                std::pair<int, int> current_direction = directions[current_direction_index];
                int i = starting_pos.second;
                int j = starting_pos.first;

                while (i >= 0 && i < grid.size() && j >= 0 && j < grid[i].size())
                {
                    if (grid[i][j] == '#')
                    {
                        // Move one step back
                        i -= current_direction.second;
                        j -= current_direction.first;

                        // Change direction
                        current_direction_index = (current_direction_index + 1) % directions.size();
                        current_direction = directions[current_direction_index];
                    }

                    // Check for loops
                    auto state = std::make_tuple(i, j, current_direction_index);
                    if (visited_states.find(state) != visited_states.end())
                    {
                        loop_detected = true;
                        break;
                    }
                    visited_states.insert(state);

                    i += current_direction.second;
                    j += current_direction.first;
                }

                if (loop_detected)
                {
                    blocking_obstacles++;
                }

                // Restore original grid cell
                grid[y][x] = temp;
            }
        }
    }

    return blocking_obstacles;
}

int main(void)
{
    std::vector<std::vector<char>> grid = read_grid_from_file("input.txt");
    std::pair<int, int> starting_pos = get_starting_pos(grid);
    int unique_positions = count_unique_positions(grid, starting_pos);
    int blocking_obstacles = count_blocking_obstacles(grid, starting_pos);

    std::cout << unique_positions << " " << blocking_obstacles << '\n';
    return 0;
}
