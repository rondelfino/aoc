#include <chrono>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <utility>
#include <vector>

static uint64_t id;

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
    size_t total_size = 0;
    for (size_t i = 0; i < input.size(); i += 2)
    {
        total_size += input[i];
        if (i + 1 < input.size())
            total_size += input[i + 1];
    }

    std::vector<std::string> disk_layout(total_size);

    size_t pos = 0;
    for (size_t i = 0; i < input.size(); i += 2)
    {
        int blocks = input[i];
        int free_space = (i + 1 < input.size()) ? input[i + 1] : 0;

        std::string id_str = std::to_string(id);

        std::fill_n(disk_layout.begin() + pos, blocks, id_str);
        pos += blocks;

        std::fill_n(disk_layout.begin() + pos, free_space, ".");
        pos += free_space;

        id++;
    }

    return disk_layout;
}

void sort_disk1(std::vector<std::string> &disk_layout)
{
    int left = 0, right = disk_layout.size() - 1;

    while (left < right)
    {
        while (left < right && disk_layout[left] != ".")
            left++;

        while (left < right && disk_layout[right] == ".")
            right--;

        if (left < right)
        {
            std::swap(disk_layout[left], disk_layout[right]);
            left++;
            right--;
        }
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
        std::string id_str = std::to_string(current_id);
        int free_space_size = 0;
        int block_size = 0;

        while (disk_layout[right] != id_str)
            right--;

        int block_start = right;
        while (disk_layout[block_start] == id_str)
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

    auto start = std::chrono::high_resolution_clock::now();
    sort_disk2(disk_layout);
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Execution time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
              << "ms\n";
    uint64_t checksum = calc_checksum(disk_layout);

    std::cout << checksum << '\n';

    return 0;
}
