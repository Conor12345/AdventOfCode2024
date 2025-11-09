#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>

struct GuardPosition
{
    int x;
    int y;
};

class GuardTracker
{
public:
    void readMapFromFile(std::string);
    void logMap();
    void execute();
    int countVisitedLocations();

private:
    GuardPosition findNextGuardPosition();
    bool isGuardPositionInMap(GuardPosition);
    void findGuardLocation();
    bool updateGuardLocation();
    char rotateGuard(char);

    std::vector<std::vector<char>> map;

    GuardPosition current_postion;

    const char EMPTY = '.';
    const char VISITED = 'X';
    const char OBSTRUCTION = '#';

    const char UPWARDS = '^';
    const char DOWNWARDS = 'v';
    const char LEFTWARDS = '<';
    const char RIGHTWARDS = '>';
};

void GuardTracker::readMapFromFile(std::string filename)
{
    std::string line_in_buffer;
    std::ifstream File(filename);

    std::vector<std::vector<char>> map_buffer;
    std::vector<char> map_line;

    while (std::getline(File, line_in_buffer))
    {
        std::vector<char> map_line(line_in_buffer.begin(), line_in_buffer.end());
        map_buffer.push_back(map_line);
    }

    File.close();

    map = map_buffer;
}

void GuardTracker::logMap()
{
    for (std::vector<char> line : map)
    {
        for (char item : line)
        {
            std::cout << item;
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

GuardPosition GuardTracker::findNextGuardPosition()
{
    char guard = map[current_postion.y][current_postion.x];
    GuardPosition next;
    if (guard == UPWARDS)
    {
        next.x = current_postion.x;
        next.y = current_postion.y - 1;
    }
    else if (guard == DOWNWARDS)
    {
        next.x = current_postion.x;
        next.y = current_postion.y + 1;
    }
    else if (guard == LEFTWARDS)
    {
        next.x = current_postion.x - 1;
        next.y = current_postion.y;
    }
    else if (guard == RIGHTWARDS)
    {
        next.x = current_postion.x + 1;
        next.y = current_postion.y;
    }
    return next;
}

bool GuardTracker::isGuardPositionInMap(GuardPosition position)
{
    if (position.x < 0 || position.x >= map[0].size())
    {
        return false;
    }
    if (position.y < 0 || position.y >= map.size())
    {
        return false;
    }
    return true;
}

bool GuardTracker::updateGuardLocation()
{
    findGuardLocation();

    GuardPosition nextPosition = findNextGuardPosition();

    if (!isGuardPositionInMap(nextPosition))
    {
        map[current_postion.y][current_postion.x] = VISITED;
        return false;
    }

    char next_location = map[nextPosition.y][nextPosition.x];
    if (next_location == EMPTY || next_location == VISITED)
    {
        map[nextPosition.y][nextPosition.x] = map[current_postion.y][current_postion.x];
        map[current_postion.y][current_postion.x] = VISITED;
        return true;
    }

    if (next_location == OBSTRUCTION)
    {
        map[current_postion.y][current_postion.x] = rotateGuard(map[current_postion.y][current_postion.x]);
        return true;
    }

    return false;
}

char GuardTracker::rotateGuard(char guard)
{
    if (guard == UPWARDS)
    {
        return RIGHTWARDS;
    }
    if (guard == RIGHTWARDS)
    {
        return DOWNWARDS;
    }
    if (guard == DOWNWARDS)
    {
        return LEFTWARDS;
    }

    return UPWARDS;
}

void GuardTracker::findGuardLocation()
{
    for (int row = 0; row < map.size(); row++)
    {
        for (int col = 0; col < map[row].size(); col++)
        {
            char item = map[row][col];
            if (item == UPWARDS || item == DOWNWARDS || item == LEFTWARDS || item == RIGHTWARDS)
            {
                current_postion.y = row;
                current_postion.x = col;
                return;
            }
        }
    }
}

int GuardTracker::countVisitedLocations()
{
    int count = 0;
    for (std::vector<char> line : map)
    {
        for (char item : line)
        {
            if (item == VISITED)
            {
                count++;
            }
        }
    }
    return count;
}

void GuardTracker::execute()
{
    while (updateGuardLocation())
    {
        // logMap();
    }
    logMap();
}

int main()
{
    GuardTracker guard_tracker;
    guard_tracker.readMapFromFile("full_input.txt");
    guard_tracker.logMap();
    guard_tracker.execute();

    std::cout << "Guard Visited " << guard_tracker.countVisitedLocations() << " Locations." << std::endl;
}