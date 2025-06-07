#include "Screen.h"

#include <cctype>  // for std::tolower
#include <cstdlib> // for exit()

// --------- Title screen implementation ---------
void Title_Screen::show() {
    std::cout << "\033[2J\033[H"; // Clear the console screen
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
    std::cout << "\033[2J\033[H"; // Clear the console screen
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
            // Start the selected level
            int level = input - '0'; // Convert char to int using ASCII value logic
            delete current_screen;
            current_screen = new Board_Screen(level);
            return; // Exit the loop after starting the level
        } else if (input == 'b') {
            // Go back to Title Screen
            delete current_screen;
            current_screen = new Title_Screen();
            return; // Exit the loop after going back
        } else {
            std::cout << "Not a level, Please try again." << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    }
}
