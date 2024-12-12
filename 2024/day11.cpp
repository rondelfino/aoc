#include <cmath>
#include <cstdint>
#include <iostream>
#include <tuple>
#include <unordered_map>

int count_digits(uint64_t n)
{
    int count = 0;
    while (n)
    {
        n /= 10;
        count++;
    }
    return count;
}

std::tuple<uint64_t, uint64_t> split_number(uint64_t n)
{
    int count = count_digits(n);
    uint64_t left = n;
    uint64_t right = n % static_cast<uint64_t>(std::pow(10, count / 2));

    for (int i = 0; i < count / 2; i++)
    {
        left /= 10;
    }

    return {left, right};
}

int main()
{
    std::unordered_map<uint64_t, uint64_t> stone_counts;
    stone_counts[7725]++;
    stone_counts[185]++;
    stone_counts[2]++;
    stone_counts[132869]++;
    stone_counts[0]++;
    stone_counts[1840437]++;
    stone_counts[62]++;
    stone_counts[26310]++;

    int blinks = 75;
    while (blinks--)
    {
        std::unordered_map<uint64_t, uint64_t> new_counts;

        for (const auto &[stone, count] : stone_counts)
        {
            if (!stone)
            {
                new_counts[1] += count;
            }
            else if (count_digits(stone) % 2 == 0)
            {
                auto [left, right] = split_number(stone);
                new_counts[left] += count;
                new_counts[right] += count;
            }
            else
            {
                new_counts[stone * 2024] += count;
            }
        }

        stone_counts = std::move(new_counts);
    }

    uint64_t total_stones = 0;
    for (const auto &[stone, count] : stone_counts)
        total_stones += count;

    std::cout << total_stones << '\n';

    return 0;
}
