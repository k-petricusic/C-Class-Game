#include <fstream>
#include <algorithm>
#include <filesystem>
#include <chrono>
#include <iostream>
#include <cmath>
#include <sstream>
#include <thread> // Added for std::this_thread::sleep_for
#include <memory> // Added for std::unique_ptr

#include "../include/Screen.h"
#include "../include/GuardMovementStrategies.h"
#undef min

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

Board_Screen::Board_Screen(int lvl) : _level(lvl), _held_keys(4, false) {
    std::string level_path = get_executable_dir() + "/levels.txt";
    read_level_from_file(level_path);
}

void Board_Screen::show(tcod::Console& console) {
    console.clear();

    size_t board_height = _board.size();
    size_t board_width = board_height > 0 ? _board[0].size() : 0;
    int console_height = console.get_height();
    int console_width = console.get_width();

    int y_offset = (console_height - static_cast<int>(board_height)) / 2;
    int x_offset = (console_width - static_cast<int>(board_width)) / 2;

    for (size_t i = 0; i < board_height; ++i) {
        for (size_t j = 0; j < board_width; ++j) {
            int draw_x = x_offset + static_cast<int>(j);
            int draw_y = y_offset + static_cast<int>(i);
            if (draw_x >= 0 && draw_x < console_width &&
                draw_y >= 0 && draw_y < console_height) {
                console.at({draw_x, draw_y}).ch = _board[i][j];
                console.at({draw_x, draw_y}).fg = tcod::ColorRGB{255, 255, 255};
                console.at({draw_x, draw_y}).bg = tcod::ColorRGB{0, 0, 0};
            }
        }
    }

    // Draw guards
    for (const auto& guard : _guards) {
        int draw_x = x_offset + static_cast<int>(guard.get_x());
        int draw_y = y_offset + static_cast<int>(guard.get_y());
        if (static_cast<int>(draw_x) >= 0 && static_cast<int>(draw_x) < static_cast<int>(console_width) &&
            static_cast<int>(draw_y) >= 0 && static_cast<int>(draw_y) < static_cast<int>(console_height)) {
            console.at({draw_x, draw_y}).ch = 'G';
        }
        // Draw red tiles around chasing guard
        if (guard.is_chasing_player()) {
            for (int dy = -1; dy <= 1; ++dy) {
                for (int dx = -1; dx <= 1; ++dx) {
                    int rx = draw_x + dx;
                    int ry = draw_y + dy;
                    if ((dx != 0 || dy != 0) && rx >= 0 && rx < console_width && ry >= 0 && ry < console_height) {
                        console.at({rx, ry}).bg = tcod::ColorRGB{255, 0, 0};
                    }
                }
            }
        } else {
            int gx = static_cast<int>(guard.get_x());
            int gy = static_cast<int>(guard.get_y());
            int dir = static_cast<int>(guard.get_direction()); // 1=up, 2=right, 3=down, 4=left

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
                    if (x < 0 || static_cast<size_t>(x) >= board_width || y < 0 || static_cast<size_t>(y) >= board_height)
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
                    
                    if (guard.get_see_through_walls()) {
                        if (angle <= 45.0 && has_line_of_sight(gx, gy, x, y)) {
                            int draw_x = x_offset + x;
                            int draw_y = y_offset + y;
                            if (static_cast<int>(draw_x) >= 0 && static_cast<int>(draw_x) < static_cast<int>(console_width) &&
                                static_cast<int>(draw_y) >= 0 && static_cast<int>(draw_y) < static_cast<int>(console_height)) {
                                // Color the tile blue for see through
                                console.at({draw_x, draw_y}).bg = tcod::ColorRGB{0, 0, 255};

                            }
                        }
                    }
                    else {
                        if (angle <= 45.0 && has_line_of_sight(gx, gy, x, y)) {
                            int draw_x = x_offset + x;
                            int draw_y = y_offset + y;
                            if (static_cast<int>(draw_x) >= 0 && static_cast<int>(draw_x) < static_cast<int>(console_width) &&
                                static_cast<int>(draw_y) >= 0 && static_cast<int>(draw_y) < static_cast<int>(console_height)) {
                                // Color the tile yellow for sight
                                console.at({draw_x, draw_y}).bg = tcod::ColorRGB{255, 255, 0};

                            }
                        }
                    }
                }
            }
        }
    }

    // Draw players
    for (const auto& player : _players) {
        int player_x = x_offset + static_cast<int>(player.get_x());
        int player_y = y_offset + static_cast<int>(player.get_y());
        if (static_cast<int>(player_x) >= 0 && static_cast<int>(player_x) < static_cast<int>(console_width) &&
            static_cast<int>(player_y) >= 0 && static_cast<int>(player_y) < static_cast<int>(console_height)) {
            // If standing on red, set pending loss
            if (console.at({player_x, player_y}).bg == tcod::ColorRGB{255, 0, 0} && !_pending_loss) {
                _pending_loss = true;
                _pending_transition_time = std::chrono::steady_clock::now();
            }
            console.at({player_x, player_y}).ch = 'O';
        }

        // Highlight player for win/loss if pending
        if (_pending_win) {
            console.at({player_x, player_y}).fg = 
            tcod::ColorRGB{0, 255, 0}; // Green for win
        }
        if (_pending_loss) {
            console.at({player_x, player_y}).fg = 
            tcod::ColorRGB{255, 255, 0}; // Yellow for loss
        }
    }

    // Draw text above the board
    if (!_level_started) {
        std::string msg = "Press any key to start!";
        int msg_x = (console_width - msg.size()) / 2;
        int msg_y = y_offset - 2; // 2 lines above the board

        if (msg_y >= 0) {
            for (size_t i = 0; i < msg.size(); ++i) {
                console.at({msg_x + static_cast<int>(i), msg_y}).ch = msg[i];
                console.at({msg_x + static_cast<int>(i), msg_y}).fg = tcod::ColorRGB{0, 255, 0}; // green
                console.at({msg_x + static_cast<int>(i), msg_y}).bg = tcod::ColorRGB{0, 0, 0};
            }
        }
    }

    // Draw timing indicator below the board
    if (_level_started) {
        int bar_y = y_offset + static_cast<int>(board_height) + 1;
        int bar_y2 = bar_y + 1;
        int bar_width = console_width;
        float progress = _timing_progress;
        int center = (bar_width - 1) / 2;
        int left_pos = static_cast<int>(center - (center * (1.0f - progress)));
        int right_pos = bar_width - 1 - left_pos;
        int chasing_count = count_chasing_guards();
        tcod::ColorRGB color = get_timing_color(chasing_count);
        int flash_range = 1; // Allow a range of 1 around the center for the flash
        for (int i = 0; i < bar_width; ++i) {
            int draw_x = i;
            // Flash blip: more generous range
            if (std::abs(i - center) <= flash_range && 
                std::abs(left_pos - center) <= flash_range && 
                std::abs(right_pos - center) <= flash_range) {
                console.at({draw_x, bar_y}).ch = ' ';
                console.at({draw_x, bar_y}).fg = tcod::ColorRGB{0, 0, 0};
                console.at({draw_x, bar_y}).bg = color;
                console.at({draw_x, bar_y2}).ch = ' ';
                console.at({draw_x, bar_y2}).fg = tcod::ColorRGB{0, 0, 0};
                console.at({draw_x, bar_y2}).bg = color;
            } else if (i == left_pos) {
                // '\xB3' is hexadecimal for 179, which is the vertical bar │ in code page 437
                console.at({draw_x, bar_y}).ch = 'I';
                console.at({draw_x, bar_y}).fg = color;
                console.at({draw_x, bar_y}).bg = tcod::ColorRGB{0, 0, 0};
                console.at({draw_x, bar_y2}).ch = 'I';
                console.at({draw_x, bar_y2}).fg = color;
                console.at({draw_x, bar_y2}).bg = tcod::ColorRGB{0, 0, 0};
            } else if (i == right_pos) {
                console.at({draw_x, bar_y}).ch = 'I';
                console.at({draw_x, bar_y}).fg = color;
                console.at({draw_x, bar_y}).bg = tcod::ColorRGB{0, 0, 0};
                console.at({draw_x, bar_y2}).ch = 'I';
                console.at({draw_x, bar_y2}).fg = color;
                console.at({draw_x, bar_y2}).bg = tcod::ColorRGB{0, 0, 0};
            } else if (i == center) {
                console.at({draw_x, bar_y}).ch = ':';
                console.at({draw_x, bar_y}).fg = tcod::ColorRGB{128, 128, 128};
                console.at({draw_x, bar_y}).bg = tcod::ColorRGB{0, 0, 0};
                console.at({draw_x, bar_y2}).ch = ':';
                console.at({draw_x, bar_y2}).fg = tcod::ColorRGB{128, 128, 128};
                console.at({draw_x, bar_y2}).bg = tcod::ColorRGB{0, 0, 0};
            } else {
                console.at({draw_x, bar_y}).ch = ' ';
                console.at({draw_x, bar_y}).fg = tcod::ColorRGB{0, 0, 0};
                console.at({draw_x, bar_y}).bg = tcod::ColorRGB{0, 0, 0};
                console.at({draw_x, bar_y2}).ch = ' ';
                console.at({draw_x, bar_y2}).fg = tcod::ColorRGB{0, 0, 0};
                console.at({draw_x, bar_y2}).bg = tcod::ColorRGB{0, 0, 0};
            }
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
        if (line == "LEVEL " + std::to_string(_level)) {
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

    _tcod_map = std::make_unique<TCODMap>(width, height);

    // Read the actual board
    while (row_index < height && std::getline(file, line)) {
        for (size_t x = 0; x < std::min(width, line.size()); ++x) {
            char c = line[x];
            if (c == 'O') {
                _players.emplace_back(x, row_index);
                _tcod_map->setProperties(x, row_index, true /* transparent */, true);
                continue; // Doesn't place player/guard on board
            } else if (c == 'G') {
                _guards.emplace_back(x, row_index);
                _tcod_map->setProperties(x, row_index, true /* transparent */, true);
                continue;
            }
            _board[row_index][x] = c;
            bool walkable = (c != '#');
            _tcod_map->setProperties(x, row_index, true /* transparent */, walkable);
        }
        ++row_index;
    }

    if (std::getline(file, line) && line == "GUARD_STRATEGIES") {
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

    // Initialize pathfinding map here
    _pathfinding_map = std::make_unique<TCODPath>(_tcod_map.get(), 99999.0f);

    file.close();
}

void Board_Screen::use_user_input(Screen*& current_screen, const SDL_Event& event) {
    if (!_level_started) {
        if (event.type == SDL_EVENT_KEY_DOWN) {
            _level_started = true;
        }
    }

    if (event.type == SDL_EVENT_KEY_DOWN) {
        int dir = 0;
        switch (event.key.key) {
            case SDLK_W: case SDLK_UP:    dir = 1; break;
            case SDLK_D: case SDLK_RIGHT: dir = 2; break;
            case SDLK_S: case SDLK_DOWN:  dir = 3; break;
            case SDLK_A: case SDLK_LEFT:  dir = 4; break;
        }
        if (dir) {
            _held_keys[dir-1] = true;
            _pressed_key = dir;
        }
    }
    if (event.type == SDL_EVENT_KEY_UP) {
        int dir = 0;
        switch (event.key.key) {
            case SDLK_W: case SDLK_UP:    dir = 1; break;
            case SDLK_D: case SDLK_RIGHT: dir = 2; break;
            case SDLK_S: case SDLK_DOWN:  dir = 3; break;
            case SDLK_A: case SDLK_LEFT:  dir = 4; break;
        }
        if (dir) {
            _held_keys[dir-1] = false;
        }
    }

    switch (event.key.key) {
        case SDLK_ESCAPE:
            delete current_screen;
            current_screen = new Level_Select_Screen();
            break;
        default: break;
    }
}

void Board_Screen::update_guards() {
    for (auto& guard : _guards) {
        if (guard.is_chasing_player()) {
            int guardX = guard.get_x();
            int guardY = guard.get_y();
            int playerX = _players[0].get_x();
            int playerY = _players[0].get_y();

            bool is_transparent = true;
            bool is_walkable = true;
            _tcod_map->setProperties(guardX, guardY, is_transparent, is_walkable);
            _tcod_map->setProperties(playerX, playerY, is_transparent, is_walkable);
            _pathfinding_map->compute(guardX, guardY, playerX, playerY);
            int nextX = guardX, nextY = guardY;
            if (!_pathfinding_map->isEmpty()) {
                _pathfinding_map->walk(&nextX, &nextY, true);
                guard.set_x(nextX);
                guard.set_y(nextY);
            }
        } else if (guard.get_movement_strategy()) {
            guard.get_movement_strategy()->move(guard, *this);
        }
    }
}

bool Board_Screen::player_in_guard_sight() const {
    if (_players.empty()) return false;
    const auto& player = _players[0];
    int px = static_cast<int>(player.get_x());
    int py = static_cast<int>(player.get_y());
    bool seen = false;
    for (auto& guard : const_cast<std::vector<Guard>&>(_guards)) {
        if (guard.is_chasing_player()) continue;
        int gx = static_cast<int>(guard.get_x());
        int gy = static_cast<int>(guard.get_y());
        int dir = static_cast<int>(guard.get_direction());
        int fx = 0, fy = 0;
        switch (dir) {
            case 1: fy = -1; break;
            case 2: fx = 1; break;
            case 3: fy = 1; break;
            case 4: fx = -1; break;
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
        if (guard.get_see_through_walls()) {
            if (angle <= 45.0) {
                guard.set_chasing_player(true);
                seen = true;
            }
        }
        else {
            if (angle <= 45.0 && has_line_of_sight(gx, gy, px, py)) {
                guard.set_chasing_player(true);
                seen = true;
            }
        }
    }
    return seen;
}

void Board_Screen::update(Screen*& current_screen) {
    auto now = std::chrono::steady_clock::now();
    if (!_level_started) return;

    // Update timing progress
    float interval = float(_frame_delay) / 1000.0f;
    float elapsed = std::chrono::duration<float>(now - _last_beat_time).count();
    if (elapsed >= interval) {
        // On the beat
        _timing_progress = 1.0f;
        _last_beat_time = now;
    } else {
        _timing_progress = elapsed / interval;
    }

    // Handle pending win/loss state
    if (_pending_win || _pending_loss) {
        if (std::chrono::duration_cast<std::chrono::milliseconds>(now - _pending_transition_time).count() >= 1500) {
            int level = _level; // Save the level number before deleting the current screen
            if (_pending_win) {
                _pending_win = false;
                delete current_screen;
                current_screen = new Game_Won_Screen(level);
                return;
            } else if (_pending_loss) {
                _pending_loss = false;
                delete current_screen;
                current_screen = new Game_Over_Screen(level);
                return;
            }
        }
        // Don't process further input/movement while pending
        return;
    }

    if (std::chrono::duration_cast<std::chrono::milliseconds>(now - _last_move_time).count() >= _frame_delay) {
        if (_pressed_key != 0) {
            if (!_held_keys[_pressed_key - 1]) {
                move(_players[0], _pressed_key);
                _pressed_key = 0;
            }
        }

        for (size_t dir = 0; dir < _held_keys.size(); ++dir) {
            if (_held_keys[dir]) {
                move(_players[0], dir + 1); // 1=up, 2=right, 3=down, 4=left
            }
        }

        if (_board[_players[0].get_y()][_players[0].get_x()] == 'E') {
            _pending_win = true;
            _pending_transition_time = std::chrono::steady_clock::now();
            return;
        }

        update_guards();
        if (player_in_guard_sight()) {
            _frame_delay /= 2;
        }
        _last_move_time = now;
    }
}

bool Board_Screen::has_line_of_sight(int x1, int y1, int x2, int y2) const {
    int dx = std::abs(x2 - x1);
    int dy = std::abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;
    if (_board[y2][x2] == '#') return false;
    //Weird edge case when you're LOS is perpendicular with a diagonal wall
    if (_board[y2][x2 - sx] == '#' && _board[y2 - sy][x2] == '#') return false;

    while (x1 != x2 || y1 != y2) {
        if (_board[y1][x1] == '#') return false;
        //Weird edge case when you're LOS is perpendicular with a diagonal wall
        if (_board[y1][x1 - sx] == '#' && _board[y1 - sy][x1] == '#') return false;

        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
    return true;
}

// Helper: count guards chasing player
int Board_Screen::count_chasing_guards() const {
    int count = 0;
    for (const auto& guard : _guards) {
        if (guard.is_chasing_player()) ++count;
    }
    return count;
}

// Helper for timing color
// Green (slow, 0 chasing), orange (1+ chasing), red (3+ chasing or 125ms interval)
tcod::ColorRGB Board_Screen::get_timing_color(int chasing_count) const {
    int min_delay = 125; // ms
    int max_delay = 500; // ms
    int d = std::clamp(_frame_delay, min_delay, max_delay);
    float t = float(max_delay - d) / float(max_delay - min_delay);
    // Default: green to yellow to red
    int r = static_cast<int>(255 * t);
    int g = static_cast<int>(255 * (1.0f - t));
    // If 1-2 guards chasing, orange; 3+ or 125ms, red
    if (chasing_count >= 3 || d <= min_delay) {
        r = 255; g = 0;
    } else if (chasing_count >= 1) {
        r = 255; g = 128;
    }
    return tcod::ColorRGB{static_cast<uint8_t>(r), static_cast<uint8_t>(g), static_cast<uint8_t>(0)};
}

