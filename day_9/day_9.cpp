#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>

struct Location
{
    bool isFile;
    int id;
};

std::vector<char> readDiskMapFromFile(std::string filename)
{
    std::string line_in_buffer;
    std::ifstream File(filename);

    std::getline(File, line_in_buffer);

    std::vector<char> map_line(line_in_buffer.begin(), line_in_buffer.end());
    File.close();
    return map_line;
}

void logCharVector(std::vector<char> vec)
{
    for (char item : vec)
    {
        std::cout << item;
    }
    std::cout << std::endl;
}

void logLocationVector(std::vector<Location> vec)
{
    for (Location item : vec)
    {
        if (item.isFile)
        {
            std::cout << item.id;
        }
        else
        {
            std::cout << '.';
        }
    }
    std::cout << std::endl;
}

std::vector<Location> expandDiskMap(std::vector<char> disk_map)
{
    int id = 0;
    std::vector<Location> expanded_map;
    for (int i = 0; i < disk_map.size() - 2; i = i + 2)
    {
        for (int file = 0; file < disk_map[i] - '0'; file++)
        {
            expanded_map.push_back({true, id});
        }
        for (int space = 0; space < disk_map[i + 1] - '0'; space++)
        {
            expanded_map.push_back({false, -1});
        }
        id++;
    }
    for (int file = 0; file < disk_map.back() - '0'; file++)
    {
        expanded_map.push_back({true, id});
    }
    return expanded_map;
}

int countTotalFileBlocks(std::vector<Location> disk_map)
{
    int count = 0;
    for (auto item : disk_map)
    {
        if (item.isFile)
        {
            count++;
        }
    }
    return count;
}

int findFirstSpace(std::vector<Location> disk_map)
{
    int count = 0;
    for (auto item : disk_map)
    {
        if (!item.isFile)
        {
            return count;
        }
        count++;
    }
    return -1;
}

int findLastFile(std::vector<Location> disk_map)
{
    for (int i = disk_map.size() - 1; i > 0; i--)
    {
        if (disk_map.at(i).isFile)
        {
            return i;
        }
    }
    return -1;
}

std::vector<Location> compressDiskMap(std::vector<Location> disk_map)
{
    int total_file_blocks = countTotalFileBlocks(disk_map);
    int first_space_loc = findFirstSpace(disk_map);
    while (first_space_loc != total_file_blocks)
    {
        int last_space_loc = findLastFile(disk_map);
        disk_map.at(first_space_loc) = disk_map.at(last_space_loc);
        disk_map.at(last_space_loc).id = -1;
        disk_map.at(last_space_loc).isFile = false;

        std::cout << first_space_loc << "\t" << last_space_loc << std::endl;
        // logLocationVector(disk_map);

        first_space_loc = findFirstSpace(disk_map);
    }

    return disk_map;
}

long long calculateCheckSum(std::vector<Location> disk_map)
{
    long long total = 0;
    for (int loc = 0; loc < disk_map.size(); loc++)
    {
        if (!disk_map.at(loc).isFile)
        {
            return total;
        }
        total += loc * disk_map.at(loc).id;
    }
    return -1;
}

int main()
{
    std::vector<char> disk_map = readDiskMapFromFile("full_input.txt");
    logCharVector(disk_map);
    std::vector<Location> expanded_map = expandDiskMap(disk_map);
    logLocationVector(expanded_map);
    std::vector<Location> compressed_map = compressDiskMap(expanded_map);
    logLocationVector(compressed_map);
    std::cout << "Checksum " << calculateCheckSum(compressed_map) << std::endl;
}