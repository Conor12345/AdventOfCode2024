#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>

std::vector<std::vector<int>> readSpaceSeparatedIntLogs(std::string filename)
{
    const std::string DIVIDER = " ";

    std::vector<std::vector<int>> logs;

    std::string line_in_buffer;

    std::ifstream File(filename);

    while (std::getline(File, line_in_buffer))
    {
        std::vector<int> current_line;
        while (line_in_buffer.size() > 0)
        {
            int divider_location = line_in_buffer.find(DIVIDER);
            if (divider_location == -1)
            {
                current_line.push_back(std::stoi(line_in_buffer));
                line_in_buffer = "";
            }
            else
            {

                current_line.push_back(std::stoi(line_in_buffer.substr(0, divider_location)));
                line_in_buffer = line_in_buffer.substr(divider_location + 1);
            }
        }
        logs.push_back(current_line);
    }

    File.close();

    return logs;
}

void printLogs(std::vector<std::vector<int>> logs)
{
    for (std::vector<int> line : logs)
    {
        for (int item : line)
        {
            std::cout << item << "\t";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

bool isListIncreasingOnly(std::vector<int> list)
{
    for (int index = 0; index < list.size() - 1; index++)
    {
        if (list.at(index) > list.at(index + 1))
        {
            return false;
        }
    }
    return true;
}

bool isListItemDiffInRange(std::vector<int> list, int min_diff, int max_diff)
{
    for (int index = 0; index < list.size() - 1; index++)
    {
        int diff = abs(list[index] - list[index + 1]);
        if ((diff < min_diff) || (diff > max_diff))
        {
            return false;
        }
    }
    return true;
}

bool isLogSafe(std::vector<int> log)
{
    bool all_increasing_or_decreasing;
    if (log[1] > log[0])
    {
        all_increasing_or_decreasing = isListIncreasingOnly(log);
    }
    else
    {
        std::vector<int> reversed_log = log;
        std::reverse(reversed_log.begin(), reversed_log.end());

        all_increasing_or_decreasing = isListIncreasingOnly(reversed_log);
    }
    if (!all_increasing_or_decreasing)
    {
        return false;
    }
    return isListItemDiffInRange(log, 1, 3);
}

int main()
{
    std::vector<std::vector<int>> logs = readSpaceSeparatedIntLogs("full_input.txt");
    int safe_log_count = 0;

    for (std::vector<int> log : logs)
    {
        if (isLogSafe(log))
        {
            safe_log_count += 1;
        }
    }
    std::cout << "Safe log count: " << safe_log_count << std::endl;
}