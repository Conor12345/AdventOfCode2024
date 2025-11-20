#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>

struct Location
{
    int row, col;

    bool operator==(const Location &other) const
    {
        if (this->row == other.row && this->col == other.col)
        {
            return true;
        }
        return false;
    }
};

struct LocationOffset
{
    int row, col;
    bool vertical;
};

bool location_sorter(Location lhs, Location rhs)
{
    if (lhs.row != rhs.row)
    {
        return lhs.row < rhs.row;
    }
    return lhs.col < rhs.col;
}

struct MapLocation
{
    char item;
    bool isCounted;
};

class FenceCalculator
{
public:
    void readMapFromFile(std::string);
    void logMap();
    void logCountedMap();
    int calculateTotalFenceCost();
    int calculateBulkFenceCost();

private:
    std::vector<Location> findContiguousLocations(int, int);
    std::vector<Location> findContiguousNeighbours(int, int);
    bool isNeighbourValid(MapLocation, int, int);
    void logLocationList(std::vector<Location>);
    int calculateFenceAreaCost(std::vector<Location>);
    int countAdjacent(Location, std::vector<Location>);
    std::vector<std::vector<MapLocation>> generateRegionMap(std::vector<Location>, char);
    void logMapRegion(std::vector<std::vector<MapLocation>>);
    int calculateRegionBulkPerimeter(std::vector<std::vector<MapLocation>>);
    std::vector<std::vector<MapLocation>> addOffsetFencesToRegion(std::vector<std::vector<MapLocation>>, int, int);
    bool isLocationInRange(std::vector<std::vector<MapLocation>>, int, int);
    int countFenceLines(std::vector<std::vector<MapLocation>>, bool);
    std::vector<Location> findFenceLocations(std::vector<std::vector<MapLocation>>, int, int, bool);
    int countArea(std::vector<std::vector<MapLocation>>);

    std::vector<std::vector<MapLocation>> map;

    const char EMPTY = '.';
    const char FENCE = '!';
};

void FenceCalculator::readMapFromFile(std::string filename)
{
    std::string line_in_buffer;
    std::ifstream File(filename);

    std::vector<MapLocation> line_buffer;
    std::vector<std::vector<MapLocation>> map_buffer;

    while (std::getline(File, line_in_buffer))
    {
        std::vector<char> map_line(line_in_buffer.begin(), line_in_buffer.end());
        line_buffer.clear();
        for (char item : map_line)
        {
            line_buffer.push_back({item, false});
        }

        map_buffer.push_back(line_buffer);
    }

    File.close();

    map = map_buffer;
}

void FenceCalculator::logMap()
{
    logMapRegion(map);
}

void FenceCalculator::logMapRegion(std::vector<std::vector<MapLocation>> map_region)
{
    for (auto line : map_region)
    {
        for (MapLocation item : line)
        {
            std::cout << item.item;
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

void FenceCalculator::logCountedMap()
{
    for (auto line : map)
    {
        for (MapLocation item : line)
        {
            std::cout << item.isCounted;
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

void FenceCalculator::logLocationList(std::vector<Location> list)
{
    std::cout << "ROW\tCOL\n";
    for (auto item : list)
    {
        std::cout << item.row << "\t" << item.col << "\n";
    }
    std::cout << "\n";
}

bool FenceCalculator::isLocationInRange(std::vector<std::vector<MapLocation>> test_map, int row, int col)
{
    if (row < 0)
    {
        return false;
    }
    if (row >= test_map.size())
    {
        return false;
    }
    if (col < 0)
    {
        return false;
    }
    if (col >= test_map.at(row).size())
    {
        return false;
    }
    return true;
}

bool FenceCalculator::isNeighbourValid(MapLocation current, int row, int col)
{
    if (!isLocationInRange(map, row, col))
    {
        return false;
    }
    if (map.at(row).at(col).isCounted)
    {
        return false;
    }

    return map.at(row).at(col).item == current.item;
}

std::vector<Location> FenceCalculator::findContiguousNeighbours(int row, int col)
{
    std::vector<Location> contiguous_locations;
    MapLocation current = map.at(row).at(col);

    if (isNeighbourValid(current, row - 1, col))
    {
        contiguous_locations.push_back({row - 1, col});
    }
    if (isNeighbourValid(current, row + 1, col))
    {
        contiguous_locations.push_back({row + 1, col});
    }
    if (isNeighbourValid(current, row, col - 1))
    {
        contiguous_locations.push_back({row, col - 1});
    }
    if (isNeighbourValid(current, row, col + 1))
    {
        contiguous_locations.push_back({row, col + 1});
    }
    return contiguous_locations;
}

std::vector<Location> FenceCalculator::findContiguousLocations(int row, int col)
{
    map.at(row).at(col).isCounted = true;

    std::vector<Location> contiguous_locations = {{row, col}};

    std::vector<Location> contiguous_neighbours = findContiguousNeighbours(row, col);

    if (contiguous_neighbours.size() == 0)
    {
        return {{row, col}};
    }

    for (Location location : contiguous_neighbours)
    {
        contiguous_locations.push_back(location);
        map.at(location.row).at(location.col).isCounted = true;
        std::vector<Location> new_contiguous_neighbours = findContiguousLocations(location.row, location.col);
        for (Location new_location : new_contiguous_neighbours)
        {
            map.at(new_location.row).at(new_location.col).isCounted = true;
            contiguous_locations.push_back(new_location);
        }
    }
    return contiguous_locations;
}

int FenceCalculator::countAdjacent(Location item, std::vector<Location> locations)
{
    int count = 0;
    for (Location compare : locations)
    {
        if (item.row == compare.row + 1 && item.col == compare.col)
        {
            count++;
        }
        else if (item.row == compare.row - 1 && item.col == compare.col)
        {
            count++;
        }
        else if (item.row == compare.row && item.col == compare.col + 1)
        {
            count++;
        }
        else if (item.row == compare.row && item.col == compare.col - 1)
        {
            count++;
        }

        if (count == 4)
        {
            return count;
        }
    }
    return count;
}

int FenceCalculator::calculateFenceAreaCost(std::vector<Location> locations)
{
    std::sort(locations.begin(), locations.end(), &location_sorter);
    std::vector<Location>::iterator ip = std::unique(locations.begin(), locations.end());
    locations.resize(std::distance(locations.begin(), ip));
    // logLocationList(locations);

    int area = locations.size();
    int perimeter = 0;
    for (Location location : locations)
    {
        perimeter += 4 - countAdjacent(location, locations);
    }

    std::cout << area << "\t" << perimeter << "\n";

    return area * perimeter;
}

int FenceCalculator::calculateTotalFenceCost()
{
    int cost = 0;
    for (int row = 0; row < map.size(); row++)
    {
        for (int col = 0; col < map.at(0).size(); col++)
        {
            if (map.at(row).at(col).isCounted)
            {
                continue;
            }
            std::vector<Location> contiguous_locations = findContiguousLocations(row, col);
            std::cout << map.at(row).at(col).item << " AT " << row << ", " << col << "\n";
            // logLocationList(contiguous_locations);
            cost += calculateFenceAreaCost(contiguous_locations);
            std::cout << "ROLLING COST: " << cost << "\n\n\n";
        }
    }
    return cost;
}

std::vector<std::vector<MapLocation>> FenceCalculator::generateRegionMap(std::vector<Location> locations, char item)
{
    int min_row = 99999;
    int min_col = 99999;

    int max_row = 0;
    int max_col = 0;

    for (Location loc : locations)
    {
        if (loc.row < min_row)
        {
            min_row = loc.row;
        }
        if (loc.row > max_row)
        {
            max_row = loc.row;
        }
        if (loc.col < min_col)
        {
            min_col = loc.col;
        }
        if (loc.col > max_col)
        {
            max_col = loc.col;
        }
    }

    std::vector<std::vector<MapLocation>> region_map(max_row + 3 - min_row, std::vector<MapLocation>(max_col + 3 - min_col, {EMPTY, false}));

    for (Location loc : locations)
    {
        region_map.at(loc.row + 1 - min_row).at(loc.col + 1 - min_col).item = item;
    }

    return region_map;
}

std::vector<std::vector<MapLocation>> FenceCalculator::addOffsetFencesToRegion(std::vector<std::vector<MapLocation>> map_region, int row_offset, int col_offset)
{
    for (int row = 0; row < map_region.size(); row++)
    {
        for (int col = 0; col < map_region.at(row).size(); col++)
        {
            if (map_region.at(row).at(col).item == EMPTY || map_region.at(row).at(col).item == FENCE)
            {
                continue;
            }
            if (!isLocationInRange(map_region, row + row_offset, col + col_offset))
            {
                continue;
            }
            if (map_region.at(row + row_offset).at(col + col_offset).item == EMPTY)
            {
                map_region.at(row + row_offset).at(col + col_offset).item = FENCE;
            }
        }
    }
    return map_region;
}

std::vector<Location> FenceCalculator::findFenceLocations(std::vector<std::vector<MapLocation>> map_region, int row, int col, bool vertical)
{
    std::vector<Location> locations = {{row, col}};
    if (vertical)
    {
        for (int row_offset = 1; row + row_offset < map_region.size(); row_offset++)
        {
            if (map_region.at(row + row_offset).at(col).item == FENCE)
            {
                locations.push_back({row + row_offset, col});
            }
            else
            {
                break;
            }
        }
        for (int row_offset = -1; row + row_offset > 0; row_offset--)
        {
            if (map_region.at(row + row_offset).at(col).item == FENCE)
            {
                locations.push_back({row + row_offset, col});
            }
            else
            {
                break;
            }
        }
        return locations;
    }
    else
    {
        for (int col_offset = +1; col + col_offset < map_region.at(row).size(); col_offset++)
        {
            if (map_region.at(row).at(col + col_offset).item == FENCE)
            {
                locations.push_back({row, col + col_offset});
            }
            else
            {
                break;
            }
        }
        for (int col_offset = -1; col + col_offset > 0; col_offset--)
        {
            if (map_region.at(row).at(col + col_offset).item == FENCE)
            {
                locations.push_back({row, col + col_offset});
            }
            else
            {
                break;
            }
        }
        return locations;
    }
}

int FenceCalculator::countFenceLines(std::vector<std::vector<MapLocation>> map_region, bool vertical)
{
    int count = 0;
    for (int row = 0; row < map_region.size(); row++)
    {
        for (int col = 0; col < map_region.at(row).size(); col++)
        {
            if (map_region.at(row).at(col).isCounted)
            {
                continue;
            }
            if (!(map_region.at(row).at(col).item == FENCE))
            {
                continue;
            }
            std::vector<Location> fence_locations = findFenceLocations(map_region, row, col, vertical);
            count += 1;
            for (Location loc : fence_locations)
            {
                map_region.at(loc.row).at(loc.col).isCounted = true;
            }
        }
    }
    return count;
}

int FenceCalculator::calculateRegionBulkPerimeter(std::vector<std::vector<MapLocation>> map_region)
{
    int count = 0;
    const std::vector<LocationOffset> offsets = {{-1, 0, false}, {+1, 0, false}, {0, -1, true}, {0, +1, true}};
    for (LocationOffset offset : offsets)
    {
        std::vector<std::vector<MapLocation>> fenced_region = addOffsetFencesToRegion(map_region, offset.row, offset.col);
        count += countFenceLines(fenced_region, offset.vertical);
    }

    return count;
}

int FenceCalculator::countArea(std::vector<std::vector<MapLocation>> map_region)
{
    int count = 0;
    for (auto row : map_region)
    {
        for (auto item : row)
        {
            if (item.item == EMPTY)
            {
                continue;
            }
            if (item.item == FENCE)
            {
                continue;
            }
            count++;
        }
    }
    return count;
}

int FenceCalculator::calculateBulkFenceCost()
{
    int cost = 0;
    for (int row = 0; row < map.size(); row++)
    {
        for (int col = 0; col < map.at(0).size(); col++)
        {
            if (map.at(row).at(col).isCounted)
            {
                continue;
            }
            std::vector<Location> contiguous_locations = findContiguousLocations(row, col);
            std::cout << map.at(row).at(col).item << " AT " << row << ", " << col << "\n";
            // logLocationList(contiguous_locations);
            std::vector<std::vector<MapLocation>> map_region = generateRegionMap(contiguous_locations, map.at(row).at(col).item);
            logMapRegion(map_region);
            cost += calculateRegionBulkPerimeter(map_region) * countArea(map_region);
        }
    }
    return cost;
}

int main()
{
    std::string file_name = "full_input.txt";
    FenceCalculator fence_calculator;
    fence_calculator.readMapFromFile(file_name);
    fence_calculator.logMap();
    std::cout << fence_calculator.calculateTotalFenceCost() << "\n\n\n\n";

    fence_calculator.readMapFromFile(file_name);
    std::cout << fence_calculator.calculateBulkFenceCost() << "\n";
}