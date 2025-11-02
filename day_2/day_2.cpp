#include <iostream>
#include <string>
#include <vector>
#include <fstream>

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

int main()
{
    std::vector<std::vector<int>> logs = readSpaceSeparatedIntLogs("test_input.txt");
    printLogs(logs);
}