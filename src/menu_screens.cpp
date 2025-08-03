#include "../include/Screen.h"

#include <cctype>   // for std::tolower
#include <cstdlib>  // for exit()
#include <fstream>  // for file IO
#include <iostream>
#include <string>
#include <thread>
#include <vector>

// --------- Title screen implementation ---------
void Title_Screen::show(tcod::Console& console) {
    console.clear();
    tcod::print(console, {1, 1}, "Welcome to the Game!", std::nullopt, std::nullopt);
    tcod::print(console, {1, 3}, "Press 'l' to start the levels.", std::nullopt, std::nullopt);
    tcod::print(console, {1, 5}, "Press 't' to view the tutorial.", std::nullopt, std::nullopt);
    tcod::print(console, {1, 7}, "Press 'c' to view credits.", std::nullopt, std::nullopt);
    tcod::print(console, {1, 9}, "Press 'ESC' to quit.", std::nullopt, std::nullopt);

    if (error_message != "") {
        tcod::print(console, {1, 11}, error_message, std::nullopt, std::nullopt);
    }
}

// Passes the pointer by reference so that the current_screen can be changed
void Title_Screen::use_user_input(Screen*& current_screen, const SDL_Event& event) {
    if (event.type == SDL_EVENT_KEY_DOWN) {
        switch (event.key.key) {
            case SDLK_L:
                delete current_screen; // Clean up the current screen
                current_screen = new Level_Select_Screen(); // Move to level select screen
                break;
            case SDLK_T:
                delete current_screen; // Clean up the current screen
                current_screen = new Tutorial_Screen(); // Move to tutorial screen
                break;
            case SDLK_C:
                delete current_screen;
                current_screen = new Credits_Screen();
                break;
            case SDLK_ESCAPE:
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
    console.clear();
    tcod::print(console, {1, 1}, "Select a level:", std::nullopt, std::nullopt);
    tcod::print(console, {1, 3}, "1. Level 1", std::nullopt, std::nullopt);
    tcod::print(console, {1, 5}, "2. Level 2", std::nullopt, std::nullopt);
    tcod::print(console, {1, 7}, "3. Level 3", std::nullopt, std::nullopt);
    tcod::print(console, {1, 9}, "4. Level 4", std::nullopt, std::nullopt);
    tcod::print(console, {1, 11}, "5. Level 5", std::nullopt, std::nullopt);
    tcod::print(console, {1, 13}, "6. Level 6", std::nullopt, std::nullopt);
    tcod::print(console, {1, 15}, "7. Level 7", std::nullopt, std::nullopt);
    tcod::print(console, {1, 17}, "8. Level 8", std::nullopt, std::nullopt);
    tcod::print(console, {1, 19}, "Press 'ESC' to go back.", std::nullopt, std::nullopt);
    tcod::print(console, {1, 21}, "Enter your choice: ", std::nullopt, std::nullopt);
    if (error_message != "") {
        tcod::print(console, {1, 23}, error_message, std::nullopt, std::nullopt);
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
            case SDLK_4: // Level 4
                delete current_screen;
                current_screen = new Board_Screen(4);
                return;
            case SDLK_5: // Level 5
                delete current_screen;
                current_screen = new Board_Screen(5);
                return;
            case SDLK_6: // Level 6
                delete current_screen;
                current_screen = new Board_Screen(6);
                return;
            case SDLK_7: // Level 7
                delete current_screen;
                current_screen = new Board_Screen(7);
                return;
            case SDLK_8: // Level 8
                delete current_screen;
                current_screen = new Board_Screen(8);
                return;
            case SDLK_ESCAPE: // Go back to title screen
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
    console.clear();
    tcod::print(console, {1, 1}, "You won! Congratulations!", std::nullopt, std::nullopt);
    tcod::print(console, {1, 3}, "Press 'ESC' to go back to the level select screen.", std::nullopt, std::nullopt);
    tcod::print(console, {1, 5}, "Press 'r' to restart the level.", std::nullopt, std::nullopt);
    tcod::print(console, {1, 7}, "Press 'n' to go to the next level.", std::nullopt, std::nullopt);
    tcod::print(console, {1, 9}, "The current level is " + std::to_string(_level), std::nullopt, std::nullopt);
    if (error_message != "") {
        tcod::print(console, {1, 11}, error_message, std::nullopt, std::nullopt);
    }
}

void Game_Won_Screen::use_user_input(Screen*& current_screen, const SDL_Event& event) {
    if (event.type == SDL_EVENT_KEY_DOWN) {
        int level = _level;
        int max_level;
        // Dynamically determine max level by reading levels.txt
        std::ifstream file(get_executable_dir() + "/levels.txt");
        std::string line;
        int found_levels = 0;
        while (std::getline(file, line)) {
            if (!line.empty() && line[0] == 'L') ++found_levels;
        }
        max_level = found_levels;
        file.close();
        
        switch (event.key.key) {
            case SDLK_ESCAPE:
                delete current_screen;
                current_screen = new Level_Select_Screen();
                return;
            case SDLK_R:
                delete current_screen;
                current_screen = new Board_Screen(level);
                return;
            case SDLK_N:
                if (level >= max_level) {
                    delete current_screen;
                    current_screen = new Credits_Screen();
                } else {
                    delete current_screen;
                    current_screen = new Board_Screen(level + 1);
                }
                return;
            default:
                error_message = "Invalid input. Please try again.";
                return;
        }
    }
}

void Game_Over_Screen::show(tcod::Console& console) {
    console.clear();
    tcod::print(console, {1, 1}, "Game Over! You were caught by a guard.", std::nullopt, std::nullopt);
    tcod::print(console, {1, 3}, "Press 'ESC' to go back to the level select screen.", std::nullopt, std::nullopt);
    tcod::print(console, {1, 5}, "Press 'r' to restart the level.", std::nullopt, std::nullopt);
    tcod::print(console, {1, 7}, "The current level is " + std::to_string(_level), std::nullopt, std::nullopt);
    if (error_message != "") {
        tcod::print(console, {1, 9}, error_message, std::nullopt, std::nullopt);
    }
}

void Game_Over_Screen::use_user_input(Screen*& current_screen, const SDL_Event& event) {
    if (event.type == SDL_EVENT_KEY_DOWN) {
        int level = _level;
        switch (event.key.key) {
            case SDLK_ESCAPE:
                delete current_screen;
                current_screen = new Level_Select_Screen();
                return;
            case SDLK_R:
                delete current_screen;
                current_screen = new Board_Screen(level);
                return;
            default:
                error_message = "Invalid input. Please try again.";
                return;
        }
    }
}

// --------- Credits screen implementation ---------
Credits_Screen::Credits_Screen() {
    std::string filename = get_executable_dir() + "/CREDITS.md";
    load_credits(filename);
}

void Credits_Screen::load_credits(const std::string& filename) {
    credits_lines.clear();
    std::ifstream file(filename);
    std::string line;
    while (std::getline(file, line)) {
        credits_lines.push_back(line);
    }
    file.close();
}

void Credits_Screen::show(tcod::Console& console) {
    console.clear();
    int y = 1;
    for (const auto& line : credits_lines) {
        tcod::print(console, {1, y}, line, std::nullopt, std::nullopt);
        y++;
    }
    tcod::print(console, {1, y+1}, "Press ESC to return to the title screen.", std::nullopt, std::nullopt);
    if (error_message != "") {
        tcod::print(console, {1, y+2}, error_message, std::nullopt, std::nullopt);
    }
}

void Credits_Screen::use_user_input(Screen*& current_screen, const SDL_Event& event) {
    if (event.type == SDL_EVENT_KEY_DOWN) {
        if (event.key.key == SDLK_ESCAPE) {
            delete current_screen;
            current_screen = new Title_Screen();
            return;
        }
    }
}