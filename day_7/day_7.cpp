#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>

struct Equation
{
    long int total;
    std::vector<int> values;
};

void logEquationList(std::vector<Equation> equation_list)
{
    for (Equation eq : equation_list)
    {
        std::cout << eq.total << "\t";
        for (int item : eq.values)
        {
            std::cout << item << " ";
        }
        std::cout << std::endl;
    }
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

std::vector<Equation> readEquationsFromFile(std::string filename)
{
    std::string line_in_buffer;
    std::ifstream File(filename);

    std::vector<Equation> equation_list;

    while (std::getline(File, line_in_buffer))
    {
        Equation equation_buffer;
        int colon_position = line_in_buffer.find(':');
        equation_buffer.total = stol(line_in_buffer.substr(0, colon_position));

        equation_buffer.values = intDelimeterSeparatedStringToVector(line_in_buffer.substr(colon_position + 2), ' ');

        equation_list.push_back(equation_buffer);
    }

    File.close();

    return equation_list;
}

int main()
{
    std::vector<Equation> equation_list = readEquationsFromFile("test_input.txt");
    logEquationList(equation_list);
}