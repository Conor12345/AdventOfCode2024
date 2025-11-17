#include <iostream>
#include <string>
#include <fstream>
#include <vector>

bool PART_2 = true;

struct Location
{
    int value;
    bool trail_found = false;
};

class TrailheadCalculator
{
public:
    void readMapFromFile(std::string);
    int sumMapTrailheadScores();
    void logMap();

private:
    bool isRowColValid(int, int, int, int);
    int countTrails(int, int);
    void resetFoundLocations();
    void logMapFound();

    std::vector<std::vector<Location>> map;
};

void TrailheadCalculator::readMapFromFile(std::string filename)
{
    std::string line_in_buffer;
    std::ifstream File(filename);

    std::vector<Location> line_buffer;
    std::vector<std::vector<Location>> map_buffer;

    while (std::getline(File, line_in_buffer))
    {
        std::vector<char> map_line(line_in_buffer.begin(), line_in_buffer.end());
        line_buffer.clear();
        for (char item : map_line)
        {
            line_buffer.push_back({item - '0', false});
        }

        map_buffer.push_back(line_buffer);
    }

    File.close();

    map = map_buffer;
}

void TrailheadCalculator::logMap()
{
    for (auto line : map)
    {
        for (auto item : line)
        {
            std::cout << item.value;
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

void TrailheadCalculator::logMapFound()
{
    for (auto line : map)
    {
        for (auto item : line)
        {
            std::cout << item.trail_found;
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

bool TrailheadCalculator::isRowColValid(int new_row, int new_col, int last_row, int last_col)
{
    if (new_row < 0)
    {
        return false;
    }
    if (new_row >= map.size())
    {
        return false;
    }
    if (new_col < 0)
    {
        return false;
    }
    if (new_col >= map.at(0).size())
    {
        return false;
    }
    if (map.at(new_row).at(new_col).value == map.at(last_row).at(last_col).value + 1)
    {
        return true;
    }

    return false;
}

int TrailheadCalculator::countTrails(int row, int col)
{
    std::cout << row << "\t" << col << "\t" << map.at(row).at(col).value << "\n";
    if (map.at(row).at(col).value == 9)
    {
        if (map.at(row).at(col).trail_found && !PART_2)
        {
            std::cout << "BASE CASE SKIP" << "\n";
            return 0;
        }
        std::cout << "BASE CASE" << "\n";
        map.at(row).at(col).trail_found = true;
        return 1;
    }

    int count = 0;
    if (isRowColValid(row + 1, col, row, col))
    {
        count += countTrails(row + 1, col);
    }
    if (isRowColValid(row - 1, col, row, col))
    {
        count += countTrails(row - 1, col);
    }
    if (isRowColValid(row, col + 1, row, col))
    {
        count += countTrails(row, col + 1);
    }
    if (isRowColValid(row, col - 1, row, col))
    {
        count += countTrails(row, col - 1);
    }

    return count;
}

void TrailheadCalculator::resetFoundLocations()
{
    for (int row = 0; row < map.size(); row++)
    {
        for (int col = 0; col < map.at(row).size(); col++)
        {
            map.at(row).at(col).trail_found = false;
        }
    }
}

int TrailheadCalculator::sumMapTrailheadScores()
{
    int total = 0;
    for (int row = 0; row < map.size(); row++)
    {
        for (int col = 0; col < map.at(row).size(); col++)
        {
            if (map.at(row).at(col).value == 0)
            {
                resetFoundLocations();
                total += countTrails(row, col);
                std::cout << total << "\n\n";
            }
        }
    }
    return total;
}

int main()
{
    TrailheadCalculator calc;
    calc.readMapFromFile("full_input.txt");
    calc.logMap();
    std::cout << calc.sumMapTrailheadScores() << std::endl;
}