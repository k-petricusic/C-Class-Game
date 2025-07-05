#include <fstream>
#include <algorithm>
#include <filesystem>
#include <chrono>

#include "../include/Screen.h"

Board_Screen::Board_Screen(int lvl) : _level(lvl) {
    std::string level_path = get_executable_dir() + "/levels.txt";
    read_level_from_file(level_path);
}

void Board_Screen::show(tcod::Console& console) {
    int height = static_cast<int>(_board.size());
    for (int i = 0; i < height; ++i) { // i = board y (top to bottom)
        for (size_t j = 0; j < _board[i].size(); ++j) { // j = board x (left to right)
            console.at({static_cast<int>(j), i}).ch = _board[i][j];
            console.at({static_cast<int>(j), i}).fg = tcod::ColorRGB{255, 255, 255};
            console.at({static_cast<int>(j), i}).bg = tcod::ColorRGB{0, 0, 0};
        }
    }

    // Superimpose the player and guards on top of the board
    for (const auto& guard : _guards) {
        console.at({static_cast<int>(guard.get_x()), static_cast<int>(guard.get_y())}).ch = 'G';
    }
    for (const auto& player : _players) {
        console.at({static_cast<int>(player.get_x()), static_cast<int>(player.get_y())}).ch = 'O';
    }
}

bool Board_Screen::move(Movable& obj, size_t direction) {
    char new_pos;
    switch(direction) {
        case 1:
            if (obj.get_y() > 0) {
                new_pos = _board[obj.get_y() - 1][obj.get_x()];
                if (new_pos == background) {
                    obj.set_y(obj.get_y() - 1);
                    return true;
                }
            }
            return false;
        case 2:
            if (obj.get_x() + 1 < _board[0].size()) {
                new_pos = _board[obj.get_y()][obj.get_x() + 1];
                if (new_pos == background) {
                    obj.set_x(obj.get_x() + 1);
                    return true;
                }
            }
            return false;
        case 3:
            if (obj.get_y() + 1 < _board.size()) {
                new_pos = _board[obj.get_y() + 1][obj.get_x()];
                if (new_pos == background) {
                    obj.set_y(obj.get_y() + 1);
                    return true;
                }
            }
            return false;
        case 4:
            if (obj.get_x() > 0) {
                new_pos = _board[obj.get_y()][obj.get_x() - 1];
                if (new_pos == background) {
                    obj.set_x(obj.get_x() - 1);
                    return true;
                }
            }
            return false;
        default:
            return false;
    }
}

void Board_Screen::read_level_from_file(const std::string& filename) {
    _guards.clear();
    _players.clear();
    _board.clear();

    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open level file: " << filename << "\n";
        return;
    }

    std::string line;
    bool found_level = false;
    size_t width = 0, height = 0;
    size_t row_index = 0;

    // Look for matching #<level>
    while (std::getline(file, line)) {
        if (line == "#" + std::to_string(_level)) {
            found_level = true;
            break;
        }
    }

    if (!found_level) {
        std::cerr << "Level #" << _level << " not found.\n";
        return;
    }

    // Read dimensions
    if (!std::getline(file, line)) {
        std::cerr << "Expected dimensions line after #" << _level << "\n";
        return;
    }

    std::istringstream dim_stream(line);
    dim_stream >> width >> height;

    _board.resize(height, std::vector<char>(width, ' '));
    row_index = 0;

    // Read the actual board
    while (row_index < height && std::getline(file, line)) {
        for (size_t x = 0; x < std::min(width, line.size()); ++x) {
            char c = line[x];
            if (c == 'O') {
                _players.emplace_back(x, row_index);
                continue; // Doesn't place player/guard on board
            } else if (c == 'G') {
                _guards.emplace_back(x, row_index);
                continue;
            }
            _board[row_index][x] = c;
        }
        if (row_index == 0) break;
        ++row_index;
    }

    std::getline(file, line); // Reads the next line after the board
    if (line == "GUARD_STRATEGIES") {
        while (std::getline(file, line) && !line.empty() && line[0] != '#') {
            std::istringstream iss(line);
            int x, y, direction;
            std::string strategy;
            iss >> x >> y >> strategy >> direction;
            for (auto& guard : _guards) {
                if (guard.get_x() == x && guard.get_y() == y) {
                    guard.set_movement_strategy(create_guard_movement_strategy(strategy));
                    guard.set_direction(direction);
                }
            }
        }
    }

    file.close();
}

void Board_Screen::use_user_input(Screen*& current_screen, const SDL_Event& event) {
    if (event.type == SDL_EVENT_KEY_DOWN) {
        switch (event.key.key) {
            case SDLK_W: pending_move_direction = 1; break;
            case SDLK_D: pending_move_direction = 2; break;
            case SDLK_S: pending_move_direction = 3; break;
            case SDLK_A: pending_move_direction = 4; break;
            case SDLK_Q:
                delete current_screen;
                current_screen = new Level_Select_Screen();
                break;
            default: break;
        }
    }
}

void Board_Screen::update_guards() {
    for (auto& guard : _guards) {
        if (guard._movement_strategy) {
            guard._movement_strategy->move(guard, *this);
        }
    }
}

void Board_Screen::update() {
    auto now = std::chrono::steady_clock::now();
    if (pending_move_direction != 0 &&
        std::chrono::duration_cast<std::chrono::milliseconds>(now - last_move_time).count() >= 250) {
        move(_players[0], pending_move_direction);
        pending_move_direction = 0;
    } 
    if (std::chrono::duration_cast<std::chrono::milliseconds>(now - last_move_time).count() >= 250) {
        update_guards();
        last_move_time = now;
    }
}
