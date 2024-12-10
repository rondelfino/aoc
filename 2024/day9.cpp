#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <utility>
#include <vector>

static uint64_t id = 0;

std::vector<int> read_from_file(const std::string &filename)
{
    std::ifstream file(filename);
    std::vector<int> res;

    if (!file.is_open())
    {
        std::cerr << "Error: Unable to open file " << filename << '\n';
        return res; // Return empty grid if file cannot be opened
    }

    std::string line;
    while (getline(file, line))
    {
        for (auto i = line.begin(); i != line.end(); i++)
        {
            res.push_back(*i - '0');
        }
    }

    return res;
}

std::vector<std::string> create_disk_layout(const std::vector<int> &input)
{
    std::vector<std::string> disk_layout;

    // Ensure input has valid pairs of (blocks, free_space)
    for (size_t i = 0; i < input.size(); i += 2)
    {
        int blocks = input[i];
        int free_space = 0;
        if (i + 1 < input.size())
        {
            free_space = input[i + 1];
        }

        // Preallocate space for efficiency
        disk_layout.reserve(disk_layout.size() + blocks + free_space);

        // Add 'blocks' number of ids
        std::string id_str = std::to_string(id);
        disk_layout.insert(disk_layout.end(), blocks, id_str);

        // Add 'free_space' number of "."
        disk_layout.insert(disk_layout.end(), free_space, ".");

        id++;
    }

    return disk_layout;
}

void sort_disk1(std::vector<std::string> &disk_layout)
{
    uint64_t left = 0;
    uint64_t right = disk_layout.size() - 1;

    while (disk_layout[right] == ".")
        right--;
    while (disk_layout[left] != ".")
        left++;

    while (left != right)
    {
        std::swap(disk_layout[left], disk_layout[right]);

        while (disk_layout[right] == "." && left != right)
            right--;
        while (disk_layout[left] != "." && left != right)
            left++;
    }
}

bool move_block(
    std::vector<std::string> &disk_layout, int free_space_start, int free_space_size, int block_start, int block_size)
{
    if (block_size <= free_space_size)
    {
        for (int i = free_space_start, j = block_start;
             i < (free_space_size + free_space_start) && j < (block_size + block_start); i++, j++)
        {
            std::swap(disk_layout[i], disk_layout[j]);
        }
    }
    else
    {
        return false;
    }

    return true;
}

void sort_disk2(std::vector<std::string> &disk_layout)
{
    uint64_t left = 0;
    uint64_t right = disk_layout.size() - 1;
    uint64_t current_id = id - 1;

    while (left < right && current_id >= 0)
    {
        int free_space_size = 0;
        int block_size = 0;

        while (disk_layout[right] != std::to_string(current_id))
            right--;

        int block_start = right;
        while (disk_layout[block_start] == std::to_string(current_id))
        {
            block_start--;
            block_size++;
        }

        while (disk_layout[left] != ".")
            left++;

        int free_space_end = left;
        while (disk_layout[free_space_end] == ".")
        {
            free_space_end++;
            free_space_size++;
        }

        int temp_left = left;
        while (!move_block(disk_layout, left, free_space_size, block_start + 1, block_size) && left < right)
        {
            free_space_size = 0;
            left = free_space_end;
            while (disk_layout[left] != "." && left != right)
                left++;
            free_space_end = left;
            while (disk_layout[free_space_end] == "." && left != right)
            {
                free_space_size++;
                free_space_end++;
            }
        }

        left = temp_left;
        right = block_start;
        current_id--;
    }
}

uint64_t calc_checksum(std::vector<std::string> &disk_layout)
{
    uint64_t checksum = 0;

    for (int i = 0; i < disk_layout.size(); i++)
    {
        if (disk_layout[i] != ".")
            checksum += std::stoull((disk_layout[i])) * i;
    }

    return checksum;
}

int main(void)
{
    std::vector<int> input = read_from_file("input.txt");
    std::vector<std::string> disk_layout = create_disk_layout(input);
    // sort_disk1(disk_layout);
    sort_disk2(disk_layout);
    uint64_t checksum = calc_checksum(disk_layout);

    for (int i = 0; i < disk_layout.size(); i++)
    {
        std::cout << disk_layout[i];
    }
    std::cout << '\n';

    std::cout << checksum << '\n';

    return 0;
}
