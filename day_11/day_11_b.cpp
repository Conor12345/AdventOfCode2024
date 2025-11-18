#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <unordered_map>

struct PebbleKey
{
    long long value;
    long int blinks_remaining;

    bool operator==(const PebbleKey &other) const
    {
        if (this->value == other.value && this->blinks_remaining == other.blinks_remaining)
        {
            return true;
        }
        return false;
    }

    struct HashFunction
    {
        size_t operator()(const PebbleKey &key) const
        {
            size_t valueHash = std::hash<long long>()(key.value);
            size_t remainingHash = std::hash<long int>()(key.blinks_remaining) << 1;
            return valueHash ^ remainingHash;
        }
    };
};

class PebbleTracker
{
public:
    void readListFromFile(std::string);
    void logPebbleList();
    void executeBlink();
    void executeNBlinks(int);

private:
    bool hasEvenNumDigits(long long);
    long int countPebblesAfterNBlinks(long long, int);
    std::vector<long long> executeBlinkItem(long long);
    std::vector<long long> intDelimeterSeparatedStringToVector(std::string, char);
    std::vector<long long> splitPebble(long long);
    long int checkHashMap(long long, int);

    std::vector<long long> pebble_list;
    std::unordered_map<PebbleKey, long int, PebbleKey::HashFunction> pebble_hash_map;
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

std::vector<long long> PebbleTracker::splitPebble(long long value)
{
    std::string num_str = std::to_string(value);

    long long num1 = std::stoll(num_str.substr(0, num_str.size() / 2));
    long long num2 = std::stoll(num_str.substr(num_str.size() / 2));

    return {num1, num2};
}

std::vector<long long> PebbleTracker::executeBlinkItem(long long value)
{
    if (value == 0)
    {
        return {1};
    }
    else if (hasEvenNumDigits(value))
    {
        return splitPebble(value);
    }
    else
    {
        if (value * 2024 < 0)
        {
            std::cout << "Exe fail " << value << "\t" << value * 2024 << "\n";
            throw;
        }
        return {value * 2024};
    }
}

long int PebbleTracker::checkHashMap(long long value, int blinks_remaining)
{
    auto item = pebble_hash_map.find({value, blinks_remaining});

    if (item != pebble_hash_map.end())
    {
        if (item->second < 0)
        {
            std::cout << "Cache fail\t" << blinks_remaining << "\t" << value << "\t" << item->second << "\n";
            throw;
        }
        return item->second;
    }
    return -1;
}

long int PebbleTracker::countPebblesAfterNBlinks(long long value, int blinks_remaining)
{
    if (blinks_remaining == 0)
    {
        return 1;
    }

    long int hash_check = checkHashMap(value, blinks_remaining);
    if (hash_check != -1)
    {
        return hash_check;
    }

    long int count = 0;
    std::vector<long long> blink_results = executeBlinkItem(value);
    for (long long item : blink_results)
    {
        count += countPebblesAfterNBlinks(item, blinks_remaining - 1);
        if (count < 0)
        {
            std::cout << "Calc loop fail\t" << blinks_remaining << "\t" << value << "\t" << count << "\n";
            throw;
        }
    }

    if (count < 0)
    {
        std::cout << "Calc fail\t" << blinks_remaining << "\t" << value << "\t" << count << "\n";
        throw;
    }

    pebble_hash_map.insert({{value, blinks_remaining}, count});
    return count;
}

void PebbleTracker::executeNBlinks(int n)
{
    long int count = 0;
    logPebbleList();
    for (long long pebble : pebble_list)
    {
        count += countPebblesAfterNBlinks(pebble, n);
    }

    std::cout << "Pebble count: " << count << "\n";
}

int main()
{
    PebbleTracker pebble_tracker;
    pebble_tracker.readListFromFile("full_input.txt");

    pebble_tracker.executeNBlinks(75);
}