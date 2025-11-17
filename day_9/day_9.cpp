#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <tuple>

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
        if (disk_map.at(loc).isFile)
        {
            total += loc * disk_map.at(loc).id;
        }
    }
    return total;
}

int findMaxID(std::vector<Location> disk_map)
{
    int max = 0;
    for (Location item : disk_map)
    {
        if (item.id > max)
        {
            max = item.id;
        }
    }
    return max;
}

std::tuple<int, int> findIDStartStop(std::vector<Location> disk_map, int target_id)
{
    int first = 0;
    for (int i = 0; i < disk_map.size(); i++)
    {
        if (disk_map.at(i).id == target_id)
        {
            first = i;
            break;
        }
    }

    int second = first;
    while (second + 1 < disk_map.size())
    {
        if (disk_map.at(second + 1).id == target_id)
        {
            second++;
        }
        else
        {
            break;
        }
    }
    return {first, second};
}

std::tuple<int, int> findSpaceWithLengthBeforeIndex(std::vector<Location> disk_map, int length, int index_limit)
{
    int first_space = findFirstSpace(disk_map);
    int space_start = first_space;
    for (int i = first_space; i < index_limit; i++)
    {
        if (disk_map.at(i).isFile)
        {
            space_start = -1;
        }
        else
        {
            if (space_start == -1)
            {
                space_start = i;
            }
            if (i - space_start + 1 >= length)
            {
                return {space_start, i};
            }
        }
    }
    return {-1, -1};
}

std::vector<Location> contiguousCompressDiskMap(std::vector<Location> disk_map)
{
    // logLocationVector(disk_map);
    int current_id = findMaxID(disk_map);
    std::cout << "Max ID: " << current_id << std::endl;
    for (current_id; current_id > 0; current_id--)
    {
        std::tuple<int, int> start_stop = findIDStartStop(disk_map, current_id);
        int start = std::get<0>(start_stop);
        int stop = std::get<1>(start_stop);
        int block_length = stop - start + 1;

        std::cout << current_id << "\t" << start << "\t" << stop << "\t" << block_length << "\t";

        std::tuple<int, int> space_start_stop = findSpaceWithLengthBeforeIndex(disk_map, block_length, start);
        std::cout << std::get<0>(space_start_stop) << "\t" << std::get<1>(space_start_stop)
                  << std::endl
                  << std::endl;

        int space_start = std::get<0>(space_start_stop);
        int space_stop = std::get<1>(space_start_stop);
        if (space_start == -1)
        {
            continue;
        }

        for (int i = 0; i < block_length; i++)
        {
            disk_map.at(space_start + i) = disk_map.at(start + i);
            disk_map.at(start + i).id = -1;
            disk_map.at(start + i).isFile = false;
        }
        // logLocationVector(disk_map);
    }
    return disk_map;
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
    std::vector<Location> contiguous_compressed_map = contiguousCompressDiskMap(expanded_map);
    logLocationVector(contiguous_compressed_map);
    std::cout << "Checksum 2 " << calculateCheckSum(contiguous_compressed_map) << std::endl;
}