#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <tuple>
#include <sstream>
#include <algorithm>

struct PageRule
{
    int first;
    int second;
};

struct InstructionData
{
    std::vector<PageRule> page_rules;
    std::vector<std::vector<int>> page_lists;
};

void logInstructionData(InstructionData data)
{
    std::cout << "Page Rules:" << std::endl;
    for (PageRule rule : data.page_rules)
    {
        std::cout << rule.first << "\t" << rule.second << std::endl;
    }
    std::cout << std::endl;

    std::cout << "Page Lists:" << std::endl;
    for (std::vector<int> page_list : data.page_lists)
    {
        for (int item : page_list)
        {
            std::cout << item << " ";
        }
        std::cout << std::endl;
    }
}

PageRule stringToPageRule(std::string input)
{
    PageRule page_rule;
    int divider = input.find("|");

    page_rule.first = std::stoi(input.substr(0, divider));
    page_rule.second = std::stoi(input.substr(divider + 1));

    return page_rule;
}

std::vector<int> intDelimeterSeparatedStringToVector(std::string input, char delimiter = ',')
{
    std::vector<int> data;

    std::stringstream ss(input);
    std::string current_item;
    while (std::getline(ss, current_item, delimiter))
    {
        data.push_back(stoi(current_item));
    }
    return data;
}

InstructionData readInstructionsFromFile(std::string filename)
{
    std::string line_in_buffer;
    std::ifstream File(filename);

    InstructionData instructions;

    bool reading_page_rules = true;

    while (std::getline(File, line_in_buffer))
    {
        if (line_in_buffer.size() < 1)
        {
            reading_page_rules = false;
            continue;
        }
        if (reading_page_rules)
        {
            instructions.page_rules.push_back(stringToPageRule(line_in_buffer));
        }
        else
        {
            instructions.page_lists.push_back(intDelimeterSeparatedStringToVector(line_in_buffer, ','));
        }
    }

    File.close();

    return instructions;
}

bool isItemInList(std::vector<int> list, int item)
{
    return std::find(list.begin(), list.end(), item) != list.end();
}

bool isPageRuleFollowed(std::vector<int> page_list, PageRule rule)
{
    std::vector<int>::iterator first_location = std::find(page_list.begin(), page_list.end(), rule.first);
    std::vector<int>::iterator second_location = std::find(page_list.begin(), page_list.end(), rule.second);

    if (first_location == page_list.end() || second_location == page_list.end())
    {
        return true;
    }

    if (std::distance(page_list.begin(), first_location) < std::distance(page_list.begin(), second_location))
    {
        return true;
    }
    return false;
}

bool isPageListValid(std::vector<int> page_list, std::vector<PageRule> page_rules)
{
    for (PageRule rule : page_rules)
    {
        if (!isPageRuleFollowed(page_list, rule))
        {
            return false;
        }
    }

    return true;
}

int returnMiddlePage(std::vector<int> page_list)
{
    int middle_page = page_list.size() / 2;
    return page_list[middle_page];
}

int sumValidMiddlePages(InstructionData data)
{
    int sum = 0;
    for (std::vector<int> page_list : data.page_lists)
    {
        if (isPageListValid(page_list, data.page_rules))
        {
            sum += returnMiddlePage(page_list);
        }
    }
    return sum;
}

int main()
{
    InstructionData complete_instructions = readInstructionsFromFile("full_input.txt");

    logInstructionData(complete_instructions);

    int valid_list_middle_page_sum = sumValidMiddlePages(complete_instructions);

    std::cout << "Sum: " << valid_list_middle_page_sum << std::endl;
}