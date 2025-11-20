#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>

struct Coord
{
    int x;
    int y;
};

struct ArcadeMachine
{
    Coord button_a;
    Coord button_b;
    Coord final_location;
    Coord current_location;
};

void logArcadeMachine(ArcadeMachine machine)
{
    std::cout << "Button A:\tX " << machine.button_a.x << "\tY " << machine.button_a.y << "\n";
    std::cout << "Button B:\tX " << machine.button_b.x << "\tY " << machine.button_b.y << "\n";
    std::cout << "Prize:\t\tX " << machine.final_location.x << "\tY " << machine.final_location.y << "\n";
    std::cout << "Current:\tX " << machine.current_location.x << "\tY " << machine.current_location.y << "\n\n";
}

std::vector<ArcadeMachine> readArcadeInputFromFile(std::string filename)
{
    std::string line_in_buffer;
    std::ifstream File(filename);

    std::vector<ArcadeMachine> arcade_machine_inputs;
    ArcadeMachine input_buffer;

    while (std::getline(File, line_in_buffer))
    {
        // Button A
        auto X_loc = line_in_buffer.find("X");
        auto Y_loc = line_in_buffer.find("Y");

        input_buffer.button_a.x = stoi(line_in_buffer.substr(X_loc + 2, Y_loc - X_loc - 4));
        input_buffer.button_a.y = stoi(line_in_buffer.substr(Y_loc + 1));

        // Button B
        std::getline(File, line_in_buffer);
        X_loc = line_in_buffer.find("X");
        Y_loc = line_in_buffer.find("Y");

        input_buffer.button_b.x = stoi(line_in_buffer.substr(X_loc + 2, Y_loc - X_loc - 4));
        input_buffer.button_b.y = stoi(line_in_buffer.substr(Y_loc + 1));

        // Prize
        std::getline(File, line_in_buffer);
        X_loc = line_in_buffer.find("X");
        Y_loc = line_in_buffer.find("Y");

        input_buffer.final_location.x = stoi(line_in_buffer.substr(X_loc + 2, Y_loc - X_loc - 4));
        input_buffer.final_location.y = stoi(line_in_buffer.substr(Y_loc + 2));

        // Spacer Line
        std::getline(File, line_in_buffer);
        arcade_machine_inputs.push_back(input_buffer);
    }

    File.close();

    return arcade_machine_inputs;
}

int main()
{
    std::vector<ArcadeMachine> arcade_states = readArcadeInputFromFile("test_input.txt");
    for (ArcadeMachine state : arcade_states)
    {
        logArcadeMachine(state);
    }
}
