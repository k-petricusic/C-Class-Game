#include "Screen.h"

#include <cctype>   // for std::tolower
#include <cstdlib>  // for exit()
#include <fstream>  // for file IO
#include <iostream>
#include <string>
#include <thread>
#include <chrono>

// --------- Title screen implementation ---------
void Title_Screen::show(tcod::Console& console) {
    tcod::print(console, {1, 1}, "Welcome to the Game!", std::nullopt, std::nullopt);
    tcod::print(console, {1, 3}, "Press 's' to start or 'q' to quit.", std::nullopt, std::nullopt);
}

// Passes the pointer by reference so that the current_screen can be changed
void Title_Screen::use_user_input(Screen*& current_screen, const SDL_Event& event) {
    if (event.type == SDL_EVENT_KEY_DOWN) {
        switch (event.key.keysym.sym) {
            case SDLK_s:
                delete current_screen; // Clean up the current screen
                current_screen = new Level_Select_Screen(); // Move to level select screen
                break;
            case SDLK_q:
                std::exit(0); // Exit the program
                break;
            default:
                tcod::print(console, {1, 5}, "Invalid input. Please try again.", std::nullopt, std::nullopt);
                current_screen->show(console);
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
                break;
        }
    }
}

// --------- Level Select screen implementation ---------
void Level_Select_Screen::show(tcod::Console& console) {
    tcod::print(console, {1, 1}, "Select a level:", std::nullopt, std::nullopt);
    tcod::print(console, {1, 3}, "1. Level 1", std::nullopt, std::nullopt);
    tcod::print(console, {1, 5}, "2. Level 2", std::nullopt, std::nullopt);
    tcod::print(console, {1, 7}, "3. Level 3", std::nullopt, std::nullopt);
    tcod::print(console, {1, 9}, "Press 'b' to go back.", std::nullopt, std::nullopt);
    tcod::print(console, {1, 11}, "Enter your choice: ", std::nullopt, std::nullopt);
}

void Level_Select_Screen::use_user_input(Screen*& current_screen, const SDL_Event& event) {
    if (event.type == SDL_EVENT_KEY_DOWN) {
        switch (event.key.keysym.sym) {
            case SDLK_1:
                delete current_screen;
                current_screen = new Board_Screen(1);
                return;
            case SDLK_2:
                delete current_screen;
                current_screen = new Board_Screen(2);
                return;
            case SDLK_3:
                delete current_screen;
                current_screen = new Board_Screen(3);
                return;
            case SDLK_b:
                delete current_screen;
                current_screen = new Title_Screen();
                return;
            default:
                tcod::print(console, {1, 13}, "Invalid input. Please try again.", std::nullopt, std::nullopt);
                current_screen->show(console);
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
                return;
        }
    }
}
