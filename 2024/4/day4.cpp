#include <fstream>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

using namespace std;

// const int directions[8][2] = {
//     {0, 1},   // Horizontal right
//     {0, -1},  // Horizontal left
//     {1, 0},   // Vertical down
//     {-1, 0},  // Vertical up
//     {1, 1},   // Diagonal down-right
//     {-1, -1}, // Diagonal up-left
//     {1, -1},  // Diagonal down-left
//     {-1, 1}   // Diagonal up-right
// };

const int directions[4][2] = {
    {1, 1},   // Diagonal down-right
    {-1, -1}, // Diagonal up-left
    {1, -1},  // Diagonal down-left
    {-1, 1}   // Diagonal up-right
};

unordered_set<int> intersections;
bool search_from(vector<vector<char>> &grid, const string &word, int x, int y, int dx, int dy)
{
    int len = word.length();
    int intersection_index = 0;
    for (int i = 0; i < len; i++)
    {
        int nx = x + i * dx;
        int ny = y + i * dy;
        if (nx < 0 || nx >= grid.size() || ny < 0 || ny >= grid[0].size() || grid[nx][ny] != word[i])
        {
            return false;
        }
        if (word[i] == 'A')
        {
            intersection_index = nx * grid[0].size() + ny;
        }
    }

    if (intersection_index)
    {
        auto found = intersections.find(intersection_index);
        if (found == intersections.end())
        {
            intersections.insert(intersection_index);
        }
        else
        {
            return true;
        }
    }
    return false;
}

int get_word_count(vector<vector<char>> &grid, const string &word)
{
    int count = 0;
    int rows = grid.size();
    int cols = grid[0].size();

    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            for (const auto &dir : directions)
            {
                if (search_from(grid, word, i, j, dir[0], dir[1]))
                {
                    count++;
                }
            }
        }
    }

    return count;
}

vector<vector<char>> read_grid_from_file(const string &filename)
{
    ifstream file(filename);
    vector<vector<char>> grid;

    string line;
    if (file.is_open())
    {
        while (getline(file, line))
        {
            vector<char> row(line.begin(), line.end());
            grid.push_back(row);
        }
    }

    return grid;
}

int main()
{
    string filename = "input.txt";
    string word = "MAS";
    string word1 = "XMAS";

    vector<vector<char>> grid = read_grid_from_file(filename);

    int count = get_word_count(grid, word);

    cout << count << '\n';

    return 0;
}
