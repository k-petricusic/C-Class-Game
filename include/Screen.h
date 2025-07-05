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
    virtual void show() = 0; // prints the board
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

    Player _player; 
    std::vector<Guard> _guards; // Guards on the board
    std::vector<std::vector<char>> _board;

    int _level;

public:
    Board_Screen(int lvl = 1) : _level(lvl) {}

    void show() override;
    void get_user_input(Screen*& current_screen) override;

    bool move(Movable& movable, size_t direction) override;

    void update_guard_los();

    void read_levels_from_file(const std::string& filename); 
    void load_and_display_level(const std::string& filename); 


    const std::vector<std::vector<char>>& get_board() const { return _board; }

    int get_level() const { return _level; }
    void set_level(int lvl) { _level = lvl; }

    Player& get_player() { return _player; } 
};
