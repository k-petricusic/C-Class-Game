#pragma once
#include <libtcod.hpp>
#include <SDL3/SDL.h>
#include <optional>

#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include <sstream>

#include "Guard.h"
#include "Player.h"
#include "Utils.h"

// --------- abstract class for other screens ---------
class Screen {
protected:
    std::string error_message;

public:
    virtual ~Screen() = default;
    virtual void show(tcod::Console& console) = 0; //prints the board
    virtual void use_user_input(Screen*& current_screen, const SDL_Event& event) = 0;
};

// --------- Title screen class ---------
class Title_Screen : public Screen {
public:
    void show(tcod::Console& console) override;
    void use_user_input(Screen*& current_screen, const SDL_Event& event) override;
};

// --------- Tutorial screen class ---------
class Tutorial_Screen : public Screen {
private:
    std::vector<std::vector<std::string>> _tutorial_tasks;
    std::vector<std::vector<std::string>> _completed_tasks;
    size_t _task_phase = 0;

    std::vector<std::vector<char>> _tutorial_board;

    std::vector<Guard> _tutorial_guards;
    std::vector<Player> _tutorial_players;

    std::chrono::steady_clock::time_point _last_move_time = std::chrono::steady_clock::now();

public:
    void show(tcod::Console& console) override;
    void use_user_input(Screen*& current_screen, const SDL_Event& event) override;

    bool move(Movable& movable, size_t direction);

    void update(Screen*& current_screen);

    size_t get_x_offset(tcod::Console& console, std::string message) const;
    size_t get_y_offset(tcod::Console& console, std::string message) const;
};

// --------- Level Select screen class ---------
class Level_Select_Screen : public Screen {
public:
    void show(tcod::Console& console) override;
    void use_user_input(Screen*& current_screen, const SDL_Event& event) override;
};

// --------- Game Over screen class ---------
class Game_Over_Screen : public Screen {
private:
    int _level;

    public:
        Game_Over_Screen(int level) : _level(level) {}

    int get_level() const { return _level; }

    void show(tcod::Console& console) override;
    void use_user_input(Screen*& current_screen, const SDL_Event& event) override;
};

// --------- Game Won screen class ---------
class Game_Won_Screen : public Screen {
    private:
        int _level;


    public:
        Game_Won_Screen(int level) : _level(level) {}

        int get_level() const { return _level; }

        void show(tcod::Console& console) override;
        void use_user_input(Screen*& current_screen, const SDL_Event& event) override;
};
    

// --------- Board screen class ---------
class Board_Screen : public Screen {
private:
    static constexpr char background = ' ';
    // obstacles are █

    std::vector<Guard> _guards; // Guards on the board
    std::vector<Player> _players; // Players on the board
    std::vector<std::vector<char>> _board;
    // Player = "O", Guard = "G", Wall = "|", Empty = " "

    bool _level_started = false;
    int _level;

    std::chrono::steady_clock::time_point _last_move_time = std::chrono::steady_clock::now();

    // 0 = no move, 1 = up, 2 = right, 3 = down, 4 = left
    int _pressed_key = 0;
    std::vector<bool> _held_keys;

    bool _pending_loss = false;
    bool _pending_win = false;
    std::chrono::steady_clock::time_point _pending_transition_time;

public:
    Board_Screen(int lvl);

    void show(tcod::Console& console) override; // Prints the board
    void use_user_input(Screen*& current_screen, const SDL_Event& event) override;


    bool move(Movable& movable, size_t direction);

    void update_guards();

    void read_level_from_file(const std::string& filename);

    const std::vector<std::vector<char>>& get_board() const { return _board; }

    int get_level() const { return _level; } // Returns a copy of the level

    bool player_in_guard_sight() const;

    void update(Screen*& current_screen);

    bool has_line_of_sight(int x1, int y1, int x2, int y2) const;
};