#include <fstream>
#include <iostream>
#include <queue>
#include <string>
#include <vector>

enum Direction
{
    NORTH,
    EAST,
    SOUTH,
    WEST
};

void move_boxes(std::vector<std::vector<char>> &tilemap,
                std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> &encountered_boxes,
                Direction &direction)
{
    int dirx = 0, diry = 0;
    switch (direction)
    {
    case NORTH:
        diry = -1;
        break;
    case EAST:
        dirx = 1;
        break;
    case SOUTH:
        diry = 1;
        break;
    case WEST:
        dirx = -1;
        break;
    }

    for (int i = encountered_boxes.size() - 1; i >= 0; i--)
    {
        auto &[box_left, box_right] = encountered_boxes[i];

        int left_nx = box_left.first + dirx;
        int left_ny = box_left.second + diry;
        int right_nx = box_right.first + dirx;
        int right_ny = box_right.second + diry;

        tilemap[box_left.second][box_left.first] = '.';
        tilemap[box_right.second][box_right.first] = '.';

        tilemap[left_ny][left_nx] = '[';
        tilemap[right_ny][right_nx] = ']';
    }
}

std::pair<int, int> move_robot(
    std::vector<std::vector<char>> &tilemap, Direction &direction, int robotx, int roboty, bool upscaled)
{
    int dirx = 0;
    int diry = 0;
    switch (direction)
    {
    case NORTH:
        diry = -1;
        break;
    case EAST:
        dirx = 1;
        break;
    case SOUTH:
        diry = 1;
        break;
    case WEST:
        dirx = -1;
        break;
    }

    int left_bound = 0, right_bound = tilemap[0].size() - 1;
    int top_bound = 0, bottom_bound = tilemap.size() - 1;
    int temp_robotx = robotx;
    int temp_roboty = roboty;
    tilemap[roboty][robotx] = '.';

    int nx = dirx + robotx;
    int ny = diry + roboty;
    char tile = tilemap[ny][nx];
    if (tile == '.')
    {
        robotx = nx;
        roboty = ny;
    }
    else if (tile == 'O')
    {
        // Look for next empty tile
        int x = dirx + nx;
        int y = diry + ny;
        while (x < right_bound && x > left_bound && y < bottom_bound && y > top_bound)
        {
            tile = tilemap[y][x];
            if (tile == '.')
            {
                robotx = nx;
                roboty = ny;
                tilemap[y][x] = 'O';
                break;
            }
            else if (tile == '#')
                break;
            x += dirx;
            y += diry;
        }
    }
    else if (tile == '[' || tile == ']')
    {
        std::pair<int, int> box_left = {0, 0};
        std::pair<int, int> box_right = {0, 0};
        if (tile == '[')
        {
            box_left.first = nx;
            box_left.second = ny;
            // impossible to have half a box
            box_right.first = box_left.first + 1;
            box_right.second = box_left.second;
        }
        else
        {
            box_right.first = nx;
            box_right.second = ny;
            // impossible to have half a box
            box_left.first = box_right.first - 1;
            box_left.second = box_right.second;
        }

        std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> encountered_boxes;
        std::queue<std::pair<std::pair<int, int>, std::pair<int, int>>> boxes;

        // encountered_boxes.push_back({box_left, box_right});
        boxes.push({box_left, box_right});

        while (!boxes.empty())
        {
            std::pair<std::pair<int, int>, std::pair<int, int>> box = boxes.front();
            boxes.pop();

            std::pair<int, int> box_left = box.first;
            std::pair<int, int> box_right = box.second;

            int next_left_x = box_left.first + 2 * dirx;
            int next_left_y = box_left.second + diry;
            int next_right_x = box_right.first + 2 * dirx;
            int next_right_y = box_right.second + diry;

            char left = tilemap[next_left_y][next_left_x];
            char right = tilemap[next_right_y][next_right_x];

            if (((left == '.' && right == '.') && (direction == NORTH || direction == SOUTH)) ||
                ((left == '.' || right == '.') && (direction == EAST || direction == WEST)))
            {
                robotx = nx;
                roboty = ny;
            }
            else if (left == '#' || right == '#')
            {
                robotx = temp_robotx;
                roboty = temp_roboty;
                encountered_boxes.clear();
                break;
            }
            else
            {
                if (right == '[')
                {
                    boxes.push({{next_right_x, next_right_y}, {next_right_x + 1, next_right_y}});
                }

                if (left == ']')
                {
                    boxes.push({{next_left_x - 1, next_left_y}, {next_left_x, next_left_y}});
                }

                if (left == '[' && right == ']')
                {
                    boxes.push({{next_left_x, next_left_y}, {next_right_x, next_right_y}});
                }
            }
            encountered_boxes.push_back({box_left, box_right});
        }
        move_boxes(tilemap, encountered_boxes, direction);
    }

    tilemap[roboty][robotx] = '@';

    return {robotx, roboty};
}

int sum_box_coordinates(const std::vector<std::vector<char>> &tilemap)
{
    int sum = 0;
    for (int i = 0; i < tilemap.size(); i++)
    {
        for (int j = 0; j < tilemap[i].size(); j++)
        {
            char tile = tilemap[i][j];
            if (tile == 'O' || tile == '[')
            {
                sum += 100 * i + j;
            }
        }
    }
    return sum;
}

std::pair<int, int> get_robot_position(const std::vector<std::vector<char>> &tilemap)
{
    std::pair<int, int> robot_pos = {0, 0};
    for (int i = 0; i < tilemap.size(); i++)
    {
        for (int j = 0; j < tilemap[i].size(); j++)
        {
            if (tilemap[i][j] == '@')
            {
                robot_pos.first = j;
                robot_pos.second = i;
            }
        }
    }
    return robot_pos;
}

std::vector<std::vector<char>> upscale(const std::vector<std::vector<char>> &tilemap)
{
    std::vector<std::vector<char>> upscaled;
    for (const auto &row : tilemap)
    {
        std::vector<char> new_row;
        for (char tile : row)
        {
            if (tile == 'O')
            {
                new_row.push_back('[');
                new_row.push_back(']');
            }
            else if (tile == '@')
            {
                new_row.push_back(tile);
                new_row.push_back('.');
            }
            else
            {
                new_row.push_back(tile);
                new_row.push_back(tile);
            }
        }

        upscaled.push_back(new_row);
    }
    return upscaled;
}

int main(void)
{
    std::vector<std::vector<char>> tilemap;
    std::vector<char> moves;

    std::fstream file("input.txt");

    bool reading_moves = false;

    if (file.is_open())
    {
        std::string line;
        while (std::getline(file, line))
        {
            if (!reading_moves)
            {
                if (!line.empty() && (line[0] == '<' || line[0] == '^' || line[0] == 'v' || line[0] == '>'))
                {
                    reading_moves = true;
                }
                else
                {
                    tilemap.emplace_back(line.begin(), line.end());
                    continue;
                }
            }

            if (reading_moves)
            {
                moves.insert(moves.end(), line.begin(), line.end());
            }
        }
    }

    std::pair<int, int> robot_pos = get_robot_position(tilemap);

    // Part 1
    // for (int i = 0; i < moves.size(); i++)
    // {
    //     Direction direction;
    //     if (moves[i] == '<')
    //         direction = WEST;
    //     else if (moves[i] == '>')
    //         direction = EAST;
    //     else if (moves[i] == '^')
    //         direction = NORTH;
    //     else
    //         direction = SOUTH;
    //     robot_pos = move_robot(tilemap, direction, robot_pos.first, robot_pos.second, false);
    // }

    // int sum = sum_box_coordinates(tilemap);
    // std::cout << sum << '\n';

    std::vector<std::vector<char>> upscaled = upscale(tilemap);
    robot_pos = get_robot_position(upscaled);
    for (int i = 0; i < upscaled.size(); i++)
    {
        for (int j = 0; j < upscaled[i].size(); j++)
        {
            std::cout << upscaled[i][j];
        }
        std::cout << '\n';
    }
    std::cout << '\n';

    // Part 2
    for (int i = 0; i < moves.size(); i++)
    {
        Direction direction;
        if (moves[i] == '<')
            direction = WEST;
        else if (moves[i] == '>')
            direction = EAST;
        else if (moves[i] == '^')
            direction = NORTH;
        else
            direction = SOUTH;
        robot_pos = move_robot(upscaled, direction, robot_pos.first, robot_pos.second, true);
    }

    for (int i = 0; i < upscaled.size(); i++)
    {
        for (int j = 0; j < upscaled[i].size(); j++)
        {
            std::cout << upscaled[i][j];
        }
        std::cout << '\n';
    }
    std::cout << '\n';

    int sum = sum_box_coordinates(upscaled);
    std::cout << sum << '\n';

    return 0;
}
