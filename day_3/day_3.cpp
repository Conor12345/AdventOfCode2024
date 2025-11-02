#include <iostream>
#include <string>
#include <fstream>

int calculateMulStatement(std::string statement)
{
    int comma_location = statement.find(",");
    if (comma_location == -1)
    {
        return 0;
    }

    std::string num_1_string = statement.substr(4, comma_location - 4);
    std::string num_2_string = statement.substr(comma_location + 1, statement.size() - comma_location - 2);

    for (char letter : num_1_string + num_2_string)
    {
        if (!std::isdigit(letter))
        {
            return 0;
        }
    }

    return std::stoi(num_1_string) * std::stoi(num_2_string);

    return 0;
}

int main()
{
    std::string line_in_buffer;

    std::ifstream File("full_input.txt");

    int current_total = 0;

    while (std::getline(File, line_in_buffer))
    {
        while (line_in_buffer.size() > 0)
        {
            // std::cout << line_in_buffer << std::endl;
            int mul_location = line_in_buffer.find("mul(");
            // std::cout << "mul " << mul_location << std::endl;
            if (mul_location == -1)
            {
                break;
            }

            std::string search_string = line_in_buffer.substr(mul_location);
            int bracket_location = search_string.find(")");
            // std::cout << "bracket " << bracket_location << std::endl;
            if (bracket_location == -1)
            {
                break;
            }

            std::string test_string = search_string.substr(0, bracket_location + 1);
            if (test_string.size() > 12)
            {
                line_in_buffer = line_in_buffer.substr(mul_location + 4);
            }
            else
            {
                int multiplcation_value = calculateMulStatement(test_string);
                current_total += multiplcation_value;
                std::cout << test_string << "\t" << multiplcation_value << "\t" << current_total << std::endl;

                line_in_buffer = line_in_buffer.substr(mul_location + test_string.size());
            }
        }
    }

    File.close();

    std::cout << "Multiplication Total: " << current_total << std::endl;
}