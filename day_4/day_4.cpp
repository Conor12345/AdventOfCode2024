#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cmath>

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

    // Horizontal
    for (std::vector<char> row : word_search)
    {
        for (char letter : row)
        {
            concatenated_search += letter;
        }
        concatenated_search += "_";
    }

    concatenated_search += "__";

    // Vertical
    for (int col_index = 0; col_index < word_search[0].size(); col_index++)
    {
        for (int row_index = 0; row_index < word_search.size(); row_index++)
        {
            concatenated_search += word_search[row_index][col_index];
        }
        concatenated_search += "_";
    }

    concatenated_search += "__";

    int x = 0;
    int y = 0;

    int width = word_search[0].size();
    int height = word_search.size();
    int diagonal_rows = std::sqrt(width * width + height * height);

    std::cout << diagonal_rows << std::endl;

    // Diagonals starting in top left
    for (int row_num = 0; row_num < width; row_num++)
    {
        x = 0;
        y = row_num;
        while (y >= 0)
        {
            concatenated_search += word_search[y][x];
            x++;
            y--;
        }
        concatenated_search += "_";
    }

    for (int row_num = 1; row_num < width; row_num++)
    {
        x = row_num;
        y = height - 1;

        while (x < width)
        {
            concatenated_search += word_search[y][x];
            x++;
            y--;
        }
        concatenated_search += "_";
    }

    concatenated_search += "__";

    // Diagonals starting in top right
    for (int row_num = 0; row_num < width; row_num++)
    {
        x = width - 1 - row_num;
        y = 0;
        while (x < width)
        {
            concatenated_search += word_search[y][x];
            x++;
            y++;
        }
        concatenated_search += "_";
    }

    for (int row_num = 1; row_num < width; row_num++)
    {
        x = 0;
        y = row_num;
        while (y < height)
        {
            concatenated_search += word_search[y][x];
            x++;
            y++;
        }
        concatenated_search += "_";
    }

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

bool isCrossMAS(std::vector<std::vector<char>> word_search, int row_num, int col_num)
{
    char top_left = word_search[row_num - 1][col_num - 1];
    char top_right = word_search[row_num - 1][col_num + 1];
    char bottom_left = word_search[row_num + 1][col_num - 1];
    char bottom_right = word_search[row_num + 1][col_num + 1];

    // M X S
    // X A X
    // M X S
    if (top_left == 'M' && bottom_left == 'M' && top_right == 'S' && bottom_right == 'S')
    {
        return true;
    }

    // S X M
    // X A X
    // S X M
    if (top_left == 'S' && bottom_left == 'S' && top_right == 'M' && bottom_right == 'M')
    {
        return true;
    }

    // M X M
    // X A X
    // S X S
    if (top_left == 'S' && bottom_left == 'M' && top_right == 'S' && bottom_right == 'M')
    {
        return true;
    }

    // S X S
    // X A X
    // M X M
    if (top_left == 'M' && bottom_left == 'S' && top_right == 'M' && bottom_right == 'S')
    {
        return true;
    }

    // std::cout << top_left << " " << top_right << std::endl;
    // std::cout << " A " << std::endl;
    // std::cout << bottom_left << " " << bottom_right << std::endl;

    // std::cout << "Failed" << std::endl;

    return false;
}

int countCrossMAS(std::vector<std::vector<char>> word_search)
{
    int count = 0;
    for (int row = 1; row < word_search.size() - 1; row++)
    {
        for (int col = 1; col < word_search[row].size() - 1; col++)
        {
            if (word_search[row][col] == 'A')
            {
                if (isCrossMAS(word_search, row, col))
                {
                    count++;
                }
            }
        }
    }

    return count;
}

int main()
{
    std::vector<std::vector<char>> word_search = readFile2dCharVector("full_input.txt");

    print2dVector(word_search);

    int XMAS_count = countXMAS(word_search);

    std::cout << "XMAS Count: " << XMAS_count << std::endl;

    int cross_MAS_count = countCrossMAS(word_search);

    std::cout << "X-MAS Count: " << cross_MAS_count << std::endl;
}