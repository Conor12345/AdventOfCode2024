#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>

struct Coord
{
    int row;
    int col;
};

struct EmitterLocations
{
    char type;
    std::vector<Coord> locations;
};

struct MapItem
{
    char map_item;
    bool isAntinode = false;
};

class AntennaMapper
{
public:
    void readMapFromFile(std::string);
    void logMap();
    int countAntiNodes();
    void logEmitterLocationMap();
    std::map<char, EmitterLocations> findEmitterLocations();

private:
    std::vector<std::vector<MapItem>> map;
    std::map<char, EmitterLocations> emitter_location_map;
    const char EMPTY_SPACE = '.';
};

void AntennaMapper::readMapFromFile(std::string filename)
{
    std::string line_in_buffer;
    std::ifstream File(filename);

    std::vector<std::vector<MapItem>> map_buffer;
    std::vector<char> char_line;
    std::vector<MapItem> location_line;
    MapItem temp_location;

    while (std::getline(File, line_in_buffer))
    {
        std::vector<char> map_line(line_in_buffer.begin(), line_in_buffer.end());
        location_line.clear();
        for (char item : map_line)
        {
            temp_location.map_item = item;
            location_line.push_back(temp_location);
        }

        map_buffer.push_back(location_line);
    }

    File.close();

    map = map_buffer;
}

void AntennaMapper::logMap()
{
    for (std::vector<MapItem> line : map)
    {
        for (MapItem item : line)
        {
            if (item.isAntinode)
            {
                std::cout << "\e[1m" << item.map_item << "\e[0m";
            }
            else
            {
                std::cout << item.map_item;
            }
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void AntennaMapper::logEmitterLocationMap()
{
    for (auto item : emitter_location_map)
    {
        std::cout << item.second.type << std::endl;
        for (auto location : item.second.locations)
        {
            std::cout << location.row << "\t" << location.col << std::endl;
        }
        std::cout << std::endl;
    }
}

std::map<char, EmitterLocations> AntennaMapper::findEmitterLocations()
{
    std::map<char, EmitterLocations> location_map;
    for (int row = 0; row < map.size(); row++)
    {
        for (int col = 0; col < map[row].size(); col++)
        {
            if (map[row][col].map_item == EMPTY_SPACE)
            {
                continue;
            }

            if (location_map.find(map[row][col].map_item) == location_map.end())
            {
                location_map.insert({map[row][col].map_item, EmitterLocations()});
                location_map.at(map[row][col].map_item).type = map[row][col].map_item;
            }

            location_map.at(map[row][col].map_item).locations.push_back({row, col});
        }
    }
    return location_map;
}

int AntennaMapper::countAntiNodes()
{
    int count = 0;
    logMap();
    emitter_location_map = findEmitterLocations();
    logEmitterLocationMap();

    return count;
}

int main()
{
    AntennaMapper mapper;
    mapper.readMapFromFile("test_input.txt");
    mapper.countAntiNodes();
}