#include <fstream>
#include <iostream>
#include <queue>
#include <string>
#include <unordered_map>
#include <unordered_set>

std::vector<int> topological_sort(const std::vector<int> &nums, const std::unordered_map<int, std::vector<int>> &adj)
{
    std::unordered_set<int> relevant_nodes(nums.begin(), nums.end());
    std::unordered_map<int, std::vector<int>> filtered_adj;
    std::unordered_map<int, int> indegree;

    // Filter the map to include only relevant nodes
    for (const auto &[node, neighbors] : adj)
    {
        for (int neighbor : neighbors)
        {
            if (relevant_nodes.find(neighbor) != relevant_nodes.end())
            {
                filtered_adj[node].push_back(neighbor);
                indegree[neighbor]++;
            }
        }
        if (indegree.find(node) == indegree.end())
        {
            indegree[node] = 0; // Ensure every relevant node is initialized
        }
    }

    // Topological Sort using Kahn's Algorithm
    std::queue<int> q;
    for (const auto &[node, deg] : indegree)
    {
        if (deg == 0)
        {
            q.push(node);
        }
    }

    std::vector<int> topoOrder;
    while (!q.empty())
    {
        int curr = q.front();
        q.pop();
        topoOrder.push_back(curr);

        for (int neighbor : filtered_adj[curr])
        {
            indegree[neighbor]--;
            if (indegree[neighbor] == 0)
            {
                q.push(neighbor);
            }
        }
    }

    // Sort the input list based on the topological order
    std::unordered_map<int, int> position;
    for (size_t i = 0; i < topoOrder.size(); ++i)
    {
        position[topoOrder[i]] = i;
    }

    std::vector<int> sorted_nums = nums;
    sort(sorted_nums.begin(), sorted_nums.end(), [&position](int a, int b) { return position[a] < position[b]; });

    return sorted_nums;
}

int main(void)
{
    int count = 0;
    int fixed_count = 0;

    std::unordered_map<int, std::vector<int>> page_ordering;
    std::ifstream file("input.txt");

    std::string line;
    if (file.is_open())
    {
        while (std::getline(file, line))
        {
            std::vector<int> pages;
            int delimiter_index = line.find('|');
            if (delimiter_index > -1)
            {
                int page_num1 = std::stoi(line.substr(0, delimiter_index));
                int page_num2 = std::stoi(line.substr(delimiter_index + 1));
                page_ordering[page_num1].push_back(page_num2);
            }
            else
            {
                std::string page_number;
                if (!line.empty())
                {
                    for (char c : line)
                    {
                        if (std::isdigit(c))
                        {
                            page_number += c;
                        }
                        else if (c == ',')
                        {
                            if (!page_number.empty())
                            {
                                pages.push_back(std::stoi(page_number));
                                page_number.clear();
                            }
                        }
                    }
                    if (!page_number.empty())
                    {
                        pages.push_back(std::stoi(page_number));
                    }

                    std::vector<int> order = page_ordering[pages[0]];
                    bool valid_order = false;
                    for (int i = 1; i < pages.size(); i++)
                    {
                        if (order.empty())
                        {
                            valid_order = false;
                        }

                        for (int j = 0; j < order.size(); j++)
                        {
                            if (pages[i] == order[j])
                            {
                                valid_order = true;
                                order = page_ordering[pages[i]];
                                break;
                            }
                            else
                            {
                                valid_order = false;
                            }
                        }
                        if (!valid_order)
                        {
                            break;
                        }
                    }

                    if (valid_order)
                    {
                        count += pages[pages.size() / 2];
                    }
                    else
                    {
                        std::vector<int> sortedNums = topological_sort(pages, page_ordering);
                        fixed_count += sortedNums[sortedNums.size() / 2];
                    }
                }
            }
        }
    }
    file.close();

    std::cout << count << " " << fixed_count << '\n';

    return 0;
}
