#include <fstream>
#include <iostream>
#include <stack>
#include <string>
#include <tuple>
#include <vector>

std::tuple<int, int> search_trailhead(std::vector<std::vector<char>> &map, int col, int row)
{
    std::stack<std::tuple<int, int>> stack;
    stack.push({col, row});

    std::vector<std::vector<bool>> visited(map.size(), std::vector<bool>(map[0].size(), false));

    int rating = 0;
    int count = 0;
    while (!stack.empty())
    {
        auto [col, row] = stack.top();
        stack.pop();

        if (map[row][col] == '9')
        {
            if (!visited[row][col])
                count++;
            rating++;
        }

        visited[row][col] = true;

        int current_height = map[row][col] - '0';
        for (auto [dx, dy] : std::vector<std::tuple<int, int>>{{0, -1}, {1, 0}, {0, 1}, {-1, 0}})
        {
            int nc = col + dx;
            int nr = row + dy;
            if (nr >= 0 && nr < map.size() && nc >= 0 && nc < map[0].size() && map[row][col] != '.')
            {
                int neighbor_height = map[nr][nc] - '0';
                if (neighbor_height == current_height + 1)
                {
                    stack.push({nc, nr});
                }
            }
        }
    }

    return {count, rating};
}

std::tuple<int, int> search_map(std::vector<std::vector<char>> &map)
{
    int score = 0;
    int rating = 0;

    for (int i = 0; i < map.size(); i++)
    {
        for (int j = 0; j < map[i].size(); j++)
        {
            if (map[i][j] == '0')
            {
                auto [s, r] = search_trailhead(map, j, i);
                score += s;
                rating += r;
            }
        }
    }

    return {score, rating};
}

int main()
{
    std::ifstream in("input.txt");

    std::vector<std::vector<char>> map;
    std::string line;
    while (std::getline(in, line))
    {
        map.push_back(std::vector<char>(line.begin(), line.end()));
    }

    // auto [total_score, rating] = search_map(contents);

    int score = 0;
    int rating = 0;
    for (int i = 0; i < map.size(); i++)
    {
        for (int j = 0; j < map[i].size(); j++)
        {
            if (map[i][j] == '0')
            {
                auto [s, r] = search_trailhead(map, j, i);
                score += s;
                rating += r;
            }
        }
    }

    std::cout << "Score: " << score << " Rating: " << rating << '\n';

    return 0;
}
