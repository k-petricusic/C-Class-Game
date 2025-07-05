#include "Screen.h"

#include <cctype>   // for std::tolower
#include <cstdlib>  // for exit()
#include <fstream>  // for file IO
#include <iostream>
#include <string>
#include <thread>
#include <chrono>

// --------- Title screen implementation ---------
void Title_Screen::show() {
    clear();
    std::cout << "Welcome to the Game!" << std::endl;
}

// Passes the pointer by reference so that the current_screen can be changed
void Title_Screen::get_user_input(Screen*& current_screen) {
    while (true) {
        show(); // Display the title screen
        std::cout << "Press 's' to start or 'q' to quit." << std::endl;

        char input;
        std::cin >> input;
        input = std::tolower(input); // case-insensitive match

        if (input == 's') {
            delete current_screen; // Clean up the current screen
            current_screen = new Level_Select_Screen(); // Move to level select screen
            return; // Exit the loop after starting the game
        } else if (input == 'q') {
            std::cout << "Quitting the game." << std::endl;
            exit(0); // Exit the program
        } else {
            std::cout << "Invalid input. Please try again." << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    }
}

// --------- Level Select screen implementation ---------
void Level_Select_Screen::show() {
    clear();
    std::cout << "Select a level:" << std::endl;
    std::cout << "1. Level 1" << std::endl;
    std::cout << "2. Level 2" << std::endl;
    std::cout << "3. Level 3" << std::endl;
    std::cout << "Press 'b' to go back." << std::endl;
}

void Level_Select_Screen::get_user_input(Screen*& current_screen) {
    while (true) {
        show(); // Display the level selection menu
        std::cout << "Enter your choice: ";

        char input;
        std::cin >> input;
        input = std::tolower(input); // case-insensitive match

        if (input == '1' || input == '2' || input == '3') {
            int level = input - '0';

            // Rewrite line 1 of levels.txt
            std::ifstream infile("levels.txt");
            if (!infile.is_open()) {
                std::cerr << "Failed to open levels.txt\n";
                return;
            }

            std::string throwaway, rest_of_file, line;
            std::getline(infile, throwaway);
            while (std::getline(infile, line)) {
                rest_of_file += line + "\n";
            }
            infile.close();

            std::ofstream outfile("levels.txt");
            outfile << level << "\n" << rest_of_file;
            outfile.close();

            // Transition to Board_Screen and display the level
            delete current_screen;
            Board_Screen* board = new Board_Screen(level);
            current_screen = board; 
            board->get_user_input(current_screen); 
            return;


        } else if (input == 'b') {
            delete current_screen;
            current_screen = new Title_Screen();
            return;
        } else {
            std::cout << "Not a level. Please try again." << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    }
}
