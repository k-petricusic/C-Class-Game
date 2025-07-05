#include "../include/Screen.h"

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
    if (error_message != "") {
        tcod::print(console, {1, 5}, error_message, std::nullopt, std::nullopt);
    }
}

// Passes the pointer by reference so that the current_screen can be changed
void Title_Screen::use_user_input(Screen*& current_screen, const SDL_Event& event) {
    if (event.type == SDL_EVENT_KEY_DOWN) {
        switch (event.key.key) {
            case SDLK_S:
                delete current_screen; // Clean up the current screen
                current_screen = new Level_Select_Screen(); // Move to level select screen
                break;
            case SDLK_Q:
                std::exit(0); // Exit the program
                break;
            default:
                error_message = "Invalid input. Please try again.";
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
    tcod::print(console, {1, 9}, "Press 'q' to go back.", std::nullopt, std::nullopt);
    tcod::print(console, {1, 11}, "Enter your choice: ", std::nullopt, std::nullopt);
    if (error_message != "") {
        tcod::print(console, {1, 13}, error_message, std::nullopt, std::nullopt);
    }
}

void Level_Select_Screen::use_user_input(Screen*& current_screen, const SDL_Event& event) {
    if (event.type == SDL_EVENT_KEY_DOWN) {
        switch (event.key.key) {
            case SDLK_1: // Level 1
                delete current_screen;
                current_screen = new Board_Screen(1);
                return;
            case SDLK_2: // Level 2
                delete current_screen;
                current_screen = new Board_Screen(2);
                return;
            case SDLK_3: // Level 3
                delete current_screen;
                current_screen = new Board_Screen(3);
                return;
            case SDLK_Q: // Go back to title screen
                delete current_screen;
                current_screen = new Title_Screen();
                return;
            default:
                error_message = "Invalid input. Please try again.";
                return;
        }
    }
}

// --------- Game Won screen implementation ---------
void Game_Won_Screen::show(tcod::Console& console) {
    tcod::print(console, {1, 1}, "You won! Congratulations!", std::nullopt, std::nullopt);
    tcod::print(console, {1, 3}, "Press 'q' to go back to the level select screen.", std::nullopt, std::nullopt);
    //tcod::print(console, {1, 5}, "Press 'r' to restart the level.", std::nullopt, std::nullopt);
    //tcod::print(console, {1, 7}, "Press 'n' to go to the next level.", std::nullopt, std::nullopt);
    if (error_message != "") {
        tcod::print(console, {1, 5}, error_message, std::nullopt, std::nullopt);
    }
}

void Game_Won_Screen::use_user_input(Screen*& current_screen, const SDL_Event& event) {
    if (event.type == SDL_EVENT_KEY_DOWN) {
        switch (event.key.key) {
            case SDLK_Q:
                delete current_screen;
                current_screen = new Level_Select_Screen();
                return;
            /*case SDLK_R:
                delete current_screen;
                current_screen = new Board_Screen(_level);
                return;
            case SDLK_N:
                delete current_screen;
                current_screen = new Board_Screen(_level + 1);
                return;*/
            default:
                error_message = "Invalid input. Please try again.";
                return;
        }
    }
}