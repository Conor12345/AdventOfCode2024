#include <iostream>
#include <vector>
#include <string>
#include <fstream>

struct ListItem
{
    int item;
    bool itemHasBeenCounted;
};

std::vector<std::vector<int>>
readTwoNumberListFile(std::string filename)
{
    const std::string LINE_DIVIDER = "   ";

    std::string line_in_buffer;

    std::vector<int> list1;
    std::vector<int> list2;

    size_t divider_location;

    std::ifstream File(filename);

    while (std::getline(File, line_in_buffer))
    {
        divider_location = line_in_buffer.find(LINE_DIVIDER);

        list1.push_back(stoi(line_in_buffer.substr(0, divider_location)));
        list2.push_back(stoi(line_in_buffer.substr(divider_location + 1, line_in_buffer.size() - divider_location - 1)));
    }

    File.close();

    std::vector<std::vector<int>> number_lists;
    number_lists.push_back(list1);
    number_lists.push_back(list2);

    return number_lists;
}

void printListStates(std::vector<ListItem> list_1, std::vector<ListItem> list_2)
{
    for (int i = 0; i < list_1.size(); i++)
    {
        std::cout << list_1[i].item << "\t" << list_1[i].itemHasBeenCounted << "\t\t" << list_2[i].item << "\t" << list_2[i].itemHasBeenCounted << std::endl
                  << std::endl;
    }
}

int findSmallestItemIndex(std::vector<ListItem> list)
{
    int currentSmallestItemIndex = -1;
    for (int index = 0; index < list.size(); index++)
    {
        if (!list.at(index).itemHasBeenCounted)
        {
            if (currentSmallestItemIndex == -1)
            {
                currentSmallestItemIndex = index;
            }
            else if (list.at(index).item < list.at(currentSmallestItemIndex).item)
            {
                currentSmallestItemIndex = index;
            }
        }
    }
    return currentSmallestItemIndex;
}

int main()
{
    std::string line_in_buffer;

    std::vector<std::vector<int>> number_lists;

    std::vector<ListItem> list_1;
    std::vector<ListItem> list_2;

    number_lists = readTwoNumberListFile("test_input.txt");

    for (int item : number_lists.at(0))
    {
        list_1.push_back(ListItem());
        list_1.back().item = item;
    }

    for (int item : number_lists.at(1))
    {
        list_2.push_back(ListItem());
        list_2.back().item = item;
    }

    int total_distance = 0;

    int list_1_index = 0;
    int list_2_index = 0;

    for (auto iterate_thru : list_1)
    {
        list_1_index = findSmallestItemIndex(list_1);
        list_2_index = findSmallestItemIndex(list_2);

        total_distance += std::abs(list_1[list_1_index].item - list_2[list_2_index].item);

        list_1.at(list_1_index).itemHasBeenCounted = true;
        list_2.at(list_2_index).itemHasBeenCounted = true;

        std::cout << "List 1 Index: " << list_1_index << "\t\t" << "List 2 Index: " << list_2_index << "\t\t" << "Current Total: " << total_distance << std::endl;

        printListStates(list_1, list_2);
    }

    std::cout << std::endl
              << total_distance << std::endl;
}