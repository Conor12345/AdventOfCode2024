#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>

std::vector<char> operators;

struct Equation
{
    long int total;
    std::vector<long int> values;
    std::vector<char> operators;
};

void logEquation(Equation eq)
{
    std::cout << eq.total << "\t";
    for (long int item : eq.values)
    {
        std::cout << item << " ";
    }
    std::cout << std::endl;
}

void logEquationList(std::vector<Equation> equation_list)
{
    for (Equation eq : equation_list)
    {
        std::cout << eq.total << "\t";
        for (long int item : eq.values)
        {
            std::cout << item << " ";
        }
        std::cout << std::endl;
    }
}

std::vector<long int> intDelimeterSeparatedStringToVector(std::string input, char delimiter = ',')
{
    std::vector<long int> data;

    std::stringstream ss(input);
    std::string current_item;
    while (std::getline(ss, current_item, delimiter))
    {
        data.push_back(stol(current_item));
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

char isEquationPossible(Equation eq)
{
    if (eq.values.size() == 2)
    {
        if (eq.values[0] + eq.values[1] == eq.total)
        {
            operators.push_back('+');
            return true;
        }
        if (eq.values[0] * eq.values[1] == eq.total)
        {
            operators.push_back('*');
            return true;
        }
        return false;
    }
    // Take the first two items
    // Add them together in slot 1, send to isEquation possible
    // Multiply them together in slot, send to isEquation possible
    Equation new_eq = eq;
    new_eq.values[1] = eq.values[0] + eq.values[1];
    new_eq.values.erase(new_eq.values.begin());
    if (isEquationPossible(new_eq))
    {
        operators.push_back('+');
        return true;
    }
    new_eq.values = eq.values;
    new_eq.values[1] = eq.values[0] * eq.values[1];
    new_eq.values.erase(new_eq.values.begin());
    if (isEquationPossible(new_eq))
    {
        operators.push_back('*');
        return true;
    }
    return false;
}

int main()
{
    std::vector<Equation> equation_list = readEquationsFromFile("full_input.txt");
    logEquationList(equation_list);
    std::cout << std::endl;

    long long running_total = 0;
    for (Equation eq : equation_list)
    {
        operators.clear();
        std::cout << "Start: ";
        logEquation(eq);
        bool result = isEquationPossible(eq);
        if (result)
        {
            running_total += eq.total;
        }

        for (char letter : operators)
        {
            std::cout << letter << " ";
        }
        std::cout << std::endl;

        std::cout << result << "\t" << running_total << std::endl;
    }

    std::cout << std::endl
              << "Total: " << running_total << std::endl;
}