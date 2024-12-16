#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <vector>

void move_robots(std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> &robots,
                 std::vector<std::vector<int>> &tilemap,
                 int width,
                 int height)
{
    for (int i = 0; i < robots.size(); i++)
    {
        int x = robots[i].first.first;
        int y = robots[i].first.second;

        tilemap[y][x]--;

        int vx = robots[i].second.first;
        int vy = robots[i].second.second;

        int nx = x + vx;
        int ny = y + vy;
        if (nx >= width)
        {
            nx = nx - width;
        }
        else if (nx < 0)
        {
            nx = nx + width;
        }

        if (ny >= height)
        {
            ny = ny - height;
        }
        else if (ny < 0)
        {
            ny = ny + height;
        }

        tilemap[ny][nx]++;

        robots[i].first.first = nx;
        robots[i].first.second = ny;
    }
}

bool has_no_overlaps(const std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> &robots)
{
    std::set<std::pair<int, int>> positions;

    for (const auto &robot : robots)
    {
        const std::pair<int, int> &pos = robot.first;
        if (positions.find(pos) != positions.end())
        {
            return false;
        }
        positions.insert(pos);
    }

    return true;
}

int main(void)
{
    std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> robots;
    std::fstream file("input.txt");
    if (file.is_open())
    {
        std::string line;
        while (std::getline(file, line))
        {
            std::istringstream iss(line);
            std::string segment;
            while (std::getline(iss, segment, ' '))
            {
                char label;
                int x, y;
                if (std::sscanf(segment.c_str(), "%c=%d,%d", &label, &x, &y) == 3)
                {
                    if (label == 'p')
                    {
                        robots.push_back({{x, y}, {0, 0}});
                    }
                    else if (label == 'v' && !robots.empty())
                    {
                        robots.back().second = {x, y};
                    }
                }
            }
        }
    }

    int height = 103;
    int width = 101;
    std::vector<std::vector<int>> tilemap(height, std::vector<int>(width, 0));
    for (int i = 0; i < robots.size(); i++)
    {
        int x = robots[i].first.first;
        int y = robots[i].first.second;
        if (tilemap[y][x])
            tilemap[y][x] += 1;
        else
            tilemap[y][x] = 1;
    }
    std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> robots_copy(robots);
    std::vector<std::vector<int>> tilemap_copy(tilemap);

    // Part 1
    int width_midpoint = width / 2;
    int height_midpoint = height / 2;
    std::pair<int, int> q1 = {0, 0};
    std::pair<int, int> q2 = {width_midpoint + 1, 0};
    std::pair<int, int> q3 = {0, height_midpoint + 1};
    std::pair<int, int> q4 = {width_midpoint + 1, height_midpoint + 1};

    int seconds = 100;
    while (seconds--)
    {
        move_robots(robots, tilemap, width, height);
    }

    int total_safety_factor = 0;
    int sq1 = 0;
    int sq2 = 0;
    int sq3 = 0;
    int sq4 = 0;
    for (int i = 0; i < robots.size(); i++)
    {
        int x = robots[i].first.first;
        int y = robots[i].first.second;
        int tile = tilemap[y][x];
        if (tile && x != width_midpoint && y != height_midpoint)
        {
            if (x >= q4.first && y >= q4.second)
            {
                sq4++;
            }
            else if (x >= q2.first && y >= q2.second)
            {
                sq2++;
            }
            else if (x >= q3.first && y >= q3.second)
            {
                sq3++;
            }
            else
            {
                sq1++;
            }
        }
    }
    total_safety_factor = sq1 * sq2 * sq3 * sq4;
    std::cout << total_safety_factor << " " << sq1 << " " << sq2 << " " << sq3 << " " << sq4 << '\n';

    // Part 2
    std::ofstream outfile("tilemap_output.txt");

    int moves = 0;
    seconds = 100000;
    while (seconds--)
    {
        // if (has_no_overlaps(robots_copy))
        // {
        //     break;
        // }
        move_robots(robots_copy, tilemap_copy, width, height);
        moves++;

        outfile << "Move " << moves << ":\n";
        for (int y = 0; y < height; ++y)
        {
            for (int x = 0; x < width; ++x)
            {
                if (tilemap_copy[y][x])
                {
                    outfile << '#';
                }
                else
                {
                    outfile << '.';
                }
            }
            outfile << "\n";
        }

        outfile << "\n";
    }

    std::cout << moves << '\n';

    return 0;
}
