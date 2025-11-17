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

bool operator==(const Coord &lhs, const Coord &rhs)
{
    if (lhs.row != rhs.row)
    {
        return false;
    }
    if (lhs.col != rhs.col)
    {
        return false;
    }

    return true;
}

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
    std::vector<Coord> findAntiNodeLocations(Coord, Coord);
    bool isCoordInRange(Coord);

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
                std::cout << "\e[1m" << 'x' << "\e[0m";
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

std::vector<Coord> AntennaMapper::findAntiNodeLocations(Coord a, Coord b)
{

    int row_diff = abs(a.row - b.row);
    int col_diff = abs(a.col - b.col);

    if (a.row > b.row)
    {
        // b .
        // . a
        if (a.col > b.col)
        {
            return {{a.row + row_diff, a.col + col_diff}, {b.row - row_diff, b.col - col_diff}};
        }
        // . b
        // a .
        else // (a.col < b.col)
        {
            return {{a.row + row_diff, a.col - col_diff}, {b.row - row_diff, b.col + col_diff}};
        }
    }
    else // (a.row < b.row)
    {
        // . a
        // b .
        if (a.col > b.col)
        {
            return {{a.row - row_diff, a.col + col_diff}, {b.row + row_diff, b.col - col_diff}};
        }
        // a .
        // . b
        else // (a.col < b.col)
        {
            return {{a.row - row_diff, a.col - col_diff}, {b.row + row_diff, b.col + col_diff}};
        }
    }
}

bool AntennaMapper::isCoordInRange(Coord coord)
{
    if (coord.row < 0)
    {
        return false;
    }
    if (coord.row >= map.size())
    {
        return false;
    }
    if (coord.col < 0)
    {
        return false;
    }
    if (coord.col >= map[0].size())
    {
        return false;
    }
    return true;
}

int AntennaMapper::countAntiNodes()
{
    int count = 0;
    logMap();
    emitter_location_map = findEmitterLocations();
    logEmitterLocationMap();

    for (auto emitter_type : emitter_location_map)
    {
        std::cout << emitter_type.first << std::endl;
        if (emitter_type.second.locations.size() == 1)
        {
            continue;
        }
        for (auto first_loc : emitter_type.second.locations)
        {
            for (auto second_loc : emitter_type.second.locations)
            {
                if (first_loc == second_loc)
                {
                    continue;
                }

                std::vector<Coord> antinode_locations = findAntiNodeLocations(first_loc, second_loc);
                for (Coord antinode : antinode_locations)
                {
                    if (isCoordInRange(antinode))
                    {
                        if (!map[antinode.row][antinode.col].isAntinode)
                        {
                            map[antinode.row][antinode.col].isAntinode = true;
                            count++;
                        }
                    }
                }
            }
        }
    }

    logMap();

    return count;
}

int main()
{
    AntennaMapper mapper;
    mapper.readMapFromFile("full_input.txt");
    std::cout << mapper.countAntiNodes() << std::endl;
}