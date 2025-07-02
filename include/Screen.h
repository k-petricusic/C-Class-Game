#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include <sstream>

#include "Guard.h"
#include "Player.h"

// --------- abstract class for other screens ---------
class Screen {
public:
    virtual ~Screen() = default;
    virtual void show() = 0; //prints the board
    virtual void get_user_input(Screen*& current_screen) = 0;

    virtual bool move(Movable& movable, size_t direction) { return false; }

    void clear() { std::cout << "\033[2J\033[H"; } // Clears the console screen
};

// --------- Title screen class ---------
class Title_Screen : public Screen {
public:
    void show() override;
    void get_user_input(Screen*& current_screen) override;
};

// --------- Level Select screen class ---------
class Level_Select_Screen : public Screen {
public:
    void show() override;
    void get_user_input(Screen*& current_screen) override;
};

// --------- Game Over screen class ---------
class Game_Over_Screen : public Screen {
public:
    void show() override;
    void get_user_input(Screen*& current_screen) override;
};

// --------- Board screen class ---------
class Board_Screen : public Screen {
private:
    char background = ' ';
    char obstacle = 'X';

    std::vector<Guard> _guards; // Guards on the board
    std::vector<std::vector<char>> _board;
    // Player = "O", Guard = "G", Wall = "|", Empty = " "

    int _level;

public:
    Board_Screen(int lvl = 1) : _level(lvl) {} // Default level is 1

    void show() override; // Prints the board
    // Wonder if this should handle user input instead of player to make main more uniform
    void get_user_input(Screen*& current_screen) override {}

    bool move(Movable& movable, size_t direction) override;

    void update_guard_los();

    void read_level_from_file(const int level);

    //if we need to we can add a fill command

    const std::vector<std::vector<char>>& get_board() const { return _board; }

    int get_level() const { return _level; } // Returns a copy of the level
    void set_level(int lvl) { _level = lvl; }
};