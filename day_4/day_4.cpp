#include <iostream>
#include <string>
#include <fstream>
#include <vector>

void print2dVector(std::vector<std::vector<char>> vector)
{
    for (std::vector<char> line : vector)
    {
        for (char item : line)
        {
            std::cout << item << "\t";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

std::vector<std::vector<char>> readFile2dCharVector(std::string filename)
{
    std::string line_in_buffer;
    std::vector<std::vector<char>> file_contents;

    std::ifstream File(filename);

    while (std::getline(File, line_in_buffer))
    {
        std::vector<char> line;
        for (char letter : line_in_buffer)
        {
            line.push_back(letter);
        }
        file_contents.push_back(line);
    }

    File.close();

    return file_contents;
}

std::string concatenateWordSearch(std::vector<std::vector<char>> word_search)
{
    std::string concatenated_search;

    for (std::vector<char> row : word_search)
    {
        for (char letter : row)
        {
            concatenated_search += letter;
        }
    }

    for (int col_index = 0; col_index < word_search[0].size(); col_index++)
    {
        for (int row_index = 0; row_index < word_search.size(); row_index++)
        {
            concatenated_search += word_search[row_index][col_index];
        }
    }

    // TODO: Add diagonal rows

    return concatenated_search;
}

int substrCount(std::string content, std::string search_phrase)
{
    int count = 0;
    int last_index = 0;
    int search_index = 0;
    std::string substring;

    while (search_index != -1)
    {
        substring = content.substr(last_index);
        search_index = substring.find(search_phrase);
        if (search_index != -1)
        {
            count += 1;
        }
        last_index += search_index + search_phrase.size();
    }

    return count;
}

int countXMAS(std::vector<std::vector<char>> word_search)
{
    const std::string XMAS = "XMAS";
    const std::string XMAS_REVERSE = "SAMX";
    int count = 0;

    std::string concatenated_word_search = concatenateWordSearch(word_search);

    std::cout << concatenated_word_search << std::endl;

    count += substrCount(concatenated_word_search, XMAS);
    count += substrCount(concatenated_word_search, XMAS_REVERSE);

    return count;
}

int main()
{
    std::vector<std::vector<char>> word_search = readFile2dCharVector("test_input_simple.txt");

    print2dVector(word_search);

    int XMAS_count = countXMAS(word_search);

    std::cout << "XMAS Count: " << XMAS_count << std::endl;
}