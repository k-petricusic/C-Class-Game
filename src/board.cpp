#include <fstream>
#include <algorithm>
#include <filesystem>
#include <chrono>
#include <iostream>
#include <cmath>

#include "../include/Screen.h"
#include "../include/GuardMovementStrategies.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

Board_Screen::Board_Screen(int lvl) : _level(lvl) {
    std::cout << "Board_Screen _level: " << _level << std::endl;
    std::string level_path = get_executable_dir() + "/levels.txt";
    read_level_from_file(level_path);
}

void Board_Screen::show(tcod::Console& console) {
    size_t board_height = _board.size();
    size_t board_width = board_height > 0 ? _board[0].size() : 0;
    size_t console_height = console.get_height();
    size_t console_width = console.get_width();

    size_t y_offset = (console_height - board_height) / 2;
    size_t x_offset = (console_width - board_width) / 2;

    for (size_t i = 0; i < board_height; ++i) {
        for (size_t j = 0; j < board_width; ++j) {
            size_t draw_x = x_offset + j;
            size_t draw_y = y_offset + i;
            if (draw_x >= 0 && draw_x < console_width && draw_y >= 0 && draw_y < console_height) {
                console.at({draw_x, draw_y}).ch = _board[i][j];
                console.at({draw_x, draw_y}).fg = tcod::ColorRGB{255, 255, 255};
                console.at({draw_x, draw_y}).bg = tcod::ColorRGB{0, 0, 0};
            }
        }
    }

    // Draw guards
    for (const auto& guard : _guards) {
        size_t draw_x = x_offset + guard.get_x();
        size_t draw_y = y_offset + guard.get_y();
        if (draw_x >= 0 && draw_x < console_width && draw_y >= 0 && draw_y < console_height) {
            console.at({draw_x, draw_y}).ch = 'G';
        }
    }
    
    // Draw guard sight cones (90-degree, radius 5)
    for (const auto& guard : _guards) {
        size_t gx = guard.get_x();
        size_t gy = guard.get_y();
        int dir = guard.get_direction(); // 1=up, 2=right, 3=down, 4=left

        // Facing vector
        int fx = 0, fy = 0;
        switch (dir) {
            case 1: fy = -1; break; // up
            case 2: fx = 1; break;  // right
            case 3: fy = 1; break;  // down
            case 4: fx = -1; break; // left
            default: break;
        }

        for (int dy = -5; dy <= 5; ++dy) {
            for (int dx = -5; dx <= 5; ++dx) {
                int x = gx + dx;
                int y = gy + dy;
                // Check bounds and radius
                if (x < 0 || x >= board_width || y < 0 || y >= board_height)
                    continue;
                if (dx*dx + dy*dy > 25 || (dx == 0 && dy == 0))
                    continue;

                // Vector from guard to tile
                double vx = dx;
                double vy = dy;
                // Normalize
                double vlen = std::sqrt(vx*vx + vy*vy);
                vx /= vlen;
                vy /= vlen;

                // Facing vector normalized
                double flen = std::sqrt(fx*fx + fy*fy);
                double fvx = (flen == 0) ? 0 : fx / flen;
                double fvy = (flen == 0) ? 0 : fy / flen;

                // Dot product gives cos(angle)
                double dot = vx * fvx + vy * fvy;
                // Clamp for safety
                if (dot > 1.0) dot = 1.0;
                if (dot < -1.0) dot = -1.0;
                double angle = std::acos(dot) * 180.0 / M_PI; // in degrees

                if (angle <= 45.0) {
                    int draw_x = x_offset + x;
                    int draw_y = y_offset + y;
                    if (draw_x >= 0 && draw_x < console_width && draw_y >= 0 && draw_y < console_height) {
                        // Color the tile yellow for sight
                        console.at({draw_x, draw_y}).bg = tcod::ColorRGB{255, 255, 0};
                    }
                }
            }
        }
    }

    // Draw players
    for (const auto& player : _players) {
        int draw_x = x_offset + static_cast<int>(player.get_x());
        int draw_y = y_offset + static_cast<int>(player.get_y());
        if (draw_x >= 0 && draw_x < console_width && draw_y >= 0 && draw_y < console_height) {
            console.at({draw_x, draw_y}).ch = 'O';
        }
    }
}

bool Board_Screen::move(Movable& obj, size_t direction) {
    char new_pos;
    switch(direction) {
        case 1:
            if (obj.get_y() > 0) {
                new_pos = _board[obj.get_y() - 1][obj.get_x()];
                if (new_pos == background || new_pos == 'E') {
                    obj.set_y(obj.get_y() - 1);
                    return true;
                }
            }
            return false;
        case 2:
            if (obj.get_x() + 1 < _board[0].size()) {
                new_pos = _board[obj.get_y()][obj.get_x() + 1];
                if (new_pos == background || new_pos == 'E') {
                    obj.set_x(obj.get_x() + 1);
                    return true;
                }
            }
            return false;
        case 3:
            if (obj.get_y() + 1 < _board.size()) {
                new_pos = _board[obj.get_y() + 1][obj.get_x()];
                if (new_pos == background || new_pos == 'E') {
                    obj.set_y(obj.get_y() + 1);
                    return true;
                }
            }
            return false;
        case 4:
            if (obj.get_x() > 0) {
                new_pos = _board[obj.get_y()][obj.get_x() - 1];
                if (new_pos == background || new_pos == 'E') {
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
        ++row_index;
    }

    std::getline(file, line); // Reads the next line after the board
    if (line == "GUARD_STRATEGIES") {
        while (std::getline(file, line) && !line.empty() && line[0] != '#') {
            std::istringstream iss(line);
            size_t x, y, direction;
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
        if (guard.get_movement_strategy()) {
            guard.get_movement_strategy()->move(guard, *this);
        }
    }
}

bool Board_Screen::player_in_guard_sight() const {
    if (_players.empty()) return false;
    const auto& player = _players[0];
    int px = static_cast<int>(player.get_x());
    int py = static_cast<int>(player.get_y());

    for (const auto& guard : _guards) {
        int gx = static_cast<int>(guard.get_x());
        int gy = static_cast<int>(guard.get_y());
        int dir = guard.get_direction();

        int fx = 0, fy = 0;
        switch (dir) {
            case 1: fy = -1; break; // up
            case 2: fx = 1; break;  // right
            case 3: fy = 1; break;  // down
            case 4: fx = -1; break; // left
            default: break;
        }

        int dx = px - gx;
        int dy = py - gy;
        if (dx*dx + dy*dy > 25 || (dx == 0 && dy == 0))
            continue;

        double vx = dx, vy = dy;
        double vlen = std::sqrt(vx*vx + vy*vy);
        vx /= vlen;
        vy /= vlen;

        double flen = std::sqrt(fx*fx + fy*fy);
        double fvx = (flen == 0) ? 0 : fx / flen;
        double fvy = (flen == 0) ? 0 : fy / flen;

        double dot = vx * fvx + vy * fvy;
        if (dot > 1.0) dot = 1.0;
        if (dot < -1.0) dot = -1.0;
        double angle = std::acos(dot) * 180.0 / M_PI;

        if (angle <= 45.0) {
            return true;
        }
    }
    return false;
}

void Board_Screen::update(Screen*& current_screen) {
    auto now = std::chrono::steady_clock::now();
    if (pending_move_direction != 0 &&
        std::chrono::duration_cast<std::chrono::milliseconds>(now - last_move_time).count() >= 250) {
        move(_players[0], pending_move_direction);
        pending_move_direction = 0;
        if (_board[_players[0].get_y()][_players[0].get_x()] == 'E') {
            delete current_screen;
            current_screen = new Game_Won_Screen(_level);
            return;
        }
    } 
    if (std::chrono::duration_cast<std::chrono::milliseconds>(now - last_move_time).count() >= 250) {
        update_guards();
        last_move_time = now;
    }

    if (player_in_guard_sight()) {
        delete current_screen;
        current_screen = new Game_Over_Screen();
        return;
    }

}
