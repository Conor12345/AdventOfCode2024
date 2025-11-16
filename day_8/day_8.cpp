#include <iostream>
#include <string>
#include <fstream>
#include <vector>

class AntennaMapper
{
public:
    void readMapFromFile(std::string);
    void logMap();

private:
    std::vector<std::vector<char>> map;
};

void AntennaMapper::readMapFromFile(std::string filename)
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

void AntennaMapper::logMap()
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

int main()
{
    AntennaMapper mapper;
    mapper.readMapFromFile("test_input.txt");
    mapper.logMap();
}