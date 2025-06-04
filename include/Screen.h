#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "Guard.h"
#include "Player.h"

// --------- abstract class for other screens ---------
class Screen {
public:
    virtual ~Screen() = default;
    virtual void show() = 0; //prints the board
    virtual void get_user_input() = 0;

    virtual bool move(Movable& movable, size_t direction) { return false; }

    void clear();
};

// --------- Title screen class ---------
class Title_Screen : public Screen {
public:
    void show() override;
    void get_user_input() override;
};

// --------- Level Select screen class ---------
class Level_Select_Screen : public Screen {
public:
    void show() override;
    void get_user_input() override;
};

// --------- Game Over screen class ---------
class Game_Over_Screen : public Screen {
public:
    void show() override;
    void get_user_input() override;
};

// --------- Board screen class ---------
class Board_Screen : public Screen {
private:
    std::vector<std::vector<char>> _board;
    // Player = "O", Guard = "G", Wall = "|", Empty = " "

public:
    void show() override; // Prints the board
    void get_user_input() override {}

    bool move(Movable& movable, size_t direction) override;

    const std::vector<std::vector<char>>& get_board() const { return _board; }
    //need to put in a way to set the board/update it with new levels
};