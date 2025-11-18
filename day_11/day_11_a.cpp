#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>

class PebbleTracker
{
public:
    void readListFromFile(std::string);
    void logPebbleList();
    void executeBlink();
    void executeNBlinks(int);

private:
    void splitPebbleAtLocation(int);
    bool hasEvenNumDigits(long long);
    std::vector<long long> intDelimeterSeparatedStringToVector(std::string, char);
    std::vector<long long> pebble_list;
};

std::vector<long long> PebbleTracker::intDelimeterSeparatedStringToVector(std::string input, char delimiter = ',')
{
    std::vector<long long> data;

    std::stringstream ss(input);
    std::string current_item;
    while (std::getline(ss, current_item, delimiter))
    {
        data.push_back(stol(current_item));
    }
    return data;
}

void PebbleTracker::readListFromFile(std::string filename)
{
    std::string line_in_buffer;
    std::ifstream File(filename);

    std::getline(File, line_in_buffer);
    File.close();
    pebble_list = intDelimeterSeparatedStringToVector(line_in_buffer, ' ');
}

bool PebbleTracker::hasEvenNumDigits(long long num)
{
    std::string num_str = std::to_string(num);
    return num_str.size() % 2 == 0;
}
void PebbleTracker::logPebbleList()
{
    for (long long item : pebble_list)
    {
        std::cout << item << " ";
    }
    std::cout << "\n";
}

void PebbleTracker::splitPebbleAtLocation(int location)
{
    std::string num_str = std::to_string(pebble_list.at(location));

    long long num1 = std::stoll(num_str.substr(0, num_str.size() / 2));
    long long num2 = std::stoll(num_str.substr(num_str.size() / 2));

    pebble_list.at(location) = num1;
    pebble_list.insert(pebble_list.begin() + location + 1, num2);
}

void PebbleTracker::executeBlink()
{
    for (int pebble_loc = 0; pebble_loc < pebble_list.size(); pebble_loc++)
    {
        if (pebble_list.at(pebble_loc) == 0)
        {
            pebble_list.at(pebble_loc) = 1;
        }
        else if (hasEvenNumDigits(pebble_list.at(pebble_loc)))
        {
            splitPebbleAtLocation(pebble_loc);
            pebble_loc++;
        }
        else
        {
            pebble_list.at(pebble_loc) = pebble_list.at(pebble_loc) * 2024;
        }
    }
}

void PebbleTracker::executeNBlinks(int n)
{
    std::cout << 0 << "/" << n << "\t";
    logPebbleList();
    for (auto i = 0; i < n; i++)
    {
        std::cout << i + 1 << "/" << n << "\n";
        executeBlink();
        // logPebbleList();
    }
    std::cout << "Pebble count: " << pebble_list.size() << "\n";
}

int main()
{
    PebbleTracker pebble_tracker;
    pebble_tracker.readListFromFile("full_input.txt");

    pebble_tracker.executeNBlinks(25);
}