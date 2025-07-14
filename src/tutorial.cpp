#include <fstream>
#include <algorithm>
#include <filesystem>
#include <chrono>
#include <iostream>
#include <cmath>
#include <sstream>
#include <thread> // Added for std::this_thread::sleep_for

#include "../include/Screen.h"
#include "../include/GuardMovementStrategies.h"
#undef min

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

Tutorial_Screen::Tutorial_Screen() {
    // Original 10x10 board
    std::vector<std::vector<char>> base_board = {
        {'#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
        {'#', '#', '#', '#', '#', '#', ' ', ' ', ' ', '#'},
        {'#', '#', '#', '#', '#', '#', ' ', '#', ' ', '#'},
        {'#', ' ', ' ', ' ', '#', ' ', ' ', '#', ' ', '#'},
        {'#', ' ', ' ', ' ', '#', ' ', ' ', '#', 'E', '#'},
        {'#', ' ', ' ', ' ', '#', ' ', ' ', '#', '#', '#'},
        {'#', '#', ' ', ' ', ' ', ' ', 'G', '#', '#', '#'},
        {'#', '#', '#', ' ', ' ', ' ', '#', '#', '#', '#'},
        {'#', '#', '#', '#', ' ', '#', '#', '#', '#', '#'},
        {'#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
    };
    // Scale the board by 2x
    _tutorial_board.clear();
    for (const auto& row : base_board) {
        std::vector<char> row1, row2;
        for (char c : row) {
            row1.push_back(c); row1.push_back(c);
            row2.push_back(c); row2.push_back(c);
        }
        _tutorial_board.push_back(row1);
        _tutorial_board.push_back(row2);
    }
    // Update tasks as before
    _tutorial_tasks = {
        {"Press W to move up", "Press D to move right", "Press S to move down", "Press A to move left"},
        {"Run in front of the guard's flashlight", " ", "Run to the 'E's to finish the level"},
        {"You have completed the tutorial!", " ", "Press ESC to go back to the title screen."}
    };
    // Player originally at (2,4) -> (4,8)
    _tutorial_players.clear();
    _tutorial_players.emplace_back(Player(4, 8));
    // Guard originally at (6,6) -> (12,12)
    _tutorial_guards.clear();
    _tutorial_guards.emplace_back(Guard(12, 12));
    _tutorial_board[12][13] = ' ';
    _tutorial_board[13][12] = ' ';
    _tutorial_board[13][13] = ' ';
    // _completed_tasks as before
    _completed_tasks.resize(_tutorial_tasks.size());
    
    for (size_t i = 0; i < _tutorial_tasks.size(); ++i) {
        _completed_tasks[i].resize(_tutorial_tasks[i].size());
    }
    _held_keys = std::vector<bool>(4, false);
    _pressed_key = 0;
}

size_t Tutorial_Screen::get_x_offset(tcod::Console& console, std::string message) const {
    if (static_cast<int>(message.size()) > console.get_width()) return 0;
    return (console.get_width() - static_cast<int>(message.size())) / 2;
}

size_t Tutorial_Screen::get_y_offset(tcod::Console& console, std::string message) const {
    if (static_cast<int>(message.size()) > console.get_height()) return 0;
    return (console.get_height() - static_cast<int>(_tutorial_board.size())) / 2;
}

void Tutorial_Screen::show(tcod::Console& console) {
    console.clear();
    // Displaying the tasks
    tcod::print(console, {static_cast<int>(get_x_offset(console, "Tasks To-Do")), static_cast<int>(get_y_offset(console, "Tasks To-Do") - 7)},
    "Tasks To-Do:", tcod::ColorRGB{255, 255, 255}, std::nullopt);
    for (size_t i = 0; i < _tutorial_tasks[_task_phase].size(); i++) {
        std::string message = _tutorial_tasks[_task_phase][i];
        size_t x_position = get_x_offset(console, message);
        size_t y_position = get_y_offset(console, message);
        if (_completed_tasks[_task_phase][i] != "") {
            tcod::print(console, {static_cast<int>(x_position), static_cast<int>(y_position - 5 + i)}, message, tcod::ColorRGB{0, 255, 0}, std::nullopt);
        } else {
            tcod::print(console, {static_cast<int>(x_position), static_cast<int>(y_position - 5 + i)}, message, tcod::ColorRGB{255, 255, 255}, std::nullopt);
        }
    }
    // Board offset for centering
    int board_x_offset = (console.get_width() - static_cast<int>(_tutorial_board[0].size())) / 2;
    int board_y_offset = (console.get_height() - static_cast<int>(_tutorial_board.size())) / 2;
    // Displaying the board
    for (size_t i = 0; i < _tutorial_board.size(); ++i) {
        for (size_t j = 0; j < _tutorial_board[0].size(); ++j) {
            int draw_x = board_x_offset + static_cast<int>(j);
            int draw_y = board_y_offset + static_cast<int>(i);
            if (draw_x >= 0 && draw_x < console.get_width() &&
                draw_y >= 0 && draw_y < console.get_height()) {
                console.at({draw_x, draw_y}).ch = _tutorial_board[i][j];
                console.at({draw_x, draw_y}).fg = tcod::ColorRGB{255, 255, 255};
                console.at({draw_x, draw_y}).bg = tcod::ColorRGB{0, 0, 0};
            }
        }
    }
    // Draw guard sight cones (90-degree, radius 10 for scale)
    for (const auto& guard : _tutorial_guards) {
        int gx = static_cast<int>(guard.get_x());
        int gy = static_cast<int>(guard.get_y());
        int dir = static_cast<int>(guard.get_direction()); // 1=up, 2=right, 3=down, 4=left
        int fx = 0, fy = 0;
        switch (dir) {
            case 1: fy = -1; break; // up
            case 2: fx = 1; break;  // right
            case 3: fy = 1; break;  // down
            case 4: fx = -1; break; // left
            default: break;
        }
        for (int dy = -10; dy <= 10; ++dy) {
            for (int dx = -10; dx <= 10; ++dx) {
                int x = gx + dx;
                int y = gy + dy;
                if (x < 0 || static_cast<size_t>(x) >= _tutorial_board[0].size() || y < 0 || static_cast<size_t>(y) >= _tutorial_board.size())
                    continue;
                if (dx*dx + dy*dy > 100 || (dx == 0 && dy == 0))
                    continue;
                double vx = dx;
                double vy = dy;
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
                    if (angle <= 45.0 && has_line_of_sight(gx, gy, x, y)) {
                        int draw_x = board_x_offset + x;
                        int draw_y = board_y_offset + y;
                        if (static_cast<int>(draw_x) >= 0 && static_cast<int>(draw_x) < static_cast<int>(console.get_width()) &&
                            static_cast<int>(draw_y) >= 0 && static_cast<int>(draw_y) < static_cast<int>(console.get_height())) {
                            console.at({draw_x, draw_y}).bg = tcod::ColorRGB{0, 0, 255};
                        }
                    }
                } else {
                    if (angle <= 45.0 && has_line_of_sight(gx, gy, x, y)) {
                        int draw_x = board_x_offset + x;
                        int draw_y = board_y_offset + y;
                        if (static_cast<int>(draw_x) >= 0 && static_cast<int>(draw_x) < static_cast<int>(console.get_width()) &&
                            static_cast<int>(draw_y) >= 0 && static_cast<int>(draw_y) < static_cast<int>(console.get_height())) {
                            console.at({draw_x, draw_y}).bg = tcod::ColorRGB{255, 255, 0};
                        }
                    }
                }
            }
        }
    }
    // Displaying the player
    int player_x = board_x_offset + static_cast<int>(_tutorial_players[0].get_x());
    int player_y = board_y_offset + static_cast<int>(_tutorial_players[0].get_y());
    console.at({player_x, player_y}).ch = 'O';
    console.at({player_x, player_y}).fg = tcod::ColorRGB{255, 255, 255};
    if (console.at({player_x, player_y}).bg == tcod::ColorRGB{255, 255, 0}) {
        console.at({player_x, player_y}).fg = tcod::ColorRGB{255, 0, 0}; // Red for guard's flashlight
        if (_task_phase == 1) _completed_tasks[1][0] = "Completed";
    } else if (_tutorial_board[static_cast<int>(_tutorial_players[0].get_y())][static_cast<int>(_tutorial_players[0].get_x())] == 'E') {
        console.at({player_x, player_y}).fg = tcod::ColorRGB{0, 255, 0}; // Green for win
        if (_task_phase == 1) _completed_tasks[1][2] = "Completed";
    }
}

bool Tutorial_Screen::move(Movable& obj, size_t direction) {
    char new_pos;
    switch(direction) {
        case 1:
            if (obj.get_y() > 0) {
                new_pos = _tutorial_board[obj.get_y() - 1][obj.get_x()];
                if (new_pos == background || new_pos == 'E') {
                    obj.set_y(obj.get_y() - 1);
                    return true;
                }
            }
            return false;
        case 2:
            if (obj.get_x() + 1 < _tutorial_board[0].size()) {
                new_pos = _tutorial_board[obj.get_y()][obj.get_x() + 1];
                if (new_pos == background || new_pos == 'E') {
                    obj.set_x(obj.get_x() + 1);
                    return true;
                }
            }
            return false;
        case 3:
            if (obj.get_y() + 1 < _tutorial_board.size()) {
                new_pos = _tutorial_board[obj.get_y() + 1][obj.get_x()];
                if (new_pos == background || new_pos == 'E') {
                    obj.set_y(obj.get_y() + 1);
                    return true;
                }
            }
            return false;
        case 4:
            if (obj.get_x() > 0) {
                new_pos = _tutorial_board[obj.get_y()][obj.get_x() - 1];
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

void Tutorial_Screen::use_user_input(Screen*& current_screen, const SDL_Event& event) {
    if (event.type == SDL_EVENT_KEY_DOWN) {
        int dir = 0;
        switch (event.key.key) {
            case SDLK_W: case SDLK_UP:    
            dir = 1; if (_task_phase == 0) _completed_tasks[0][0] = "Completed"; break;
            case SDLK_D: case SDLK_RIGHT: 
            dir = 2; if (_task_phase == 0) _completed_tasks[0][1] = "Completed"; break;
            case SDLK_S: case SDLK_DOWN:  
            dir = 3; if (_task_phase == 0) _completed_tasks[0][2] = "Completed"; break;
            case SDLK_A: case SDLK_LEFT:  
            dir = 4; if (_task_phase == 0) _completed_tasks[0][3] = "Completed"; break;
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
            current_screen = new Title_Screen();
            break;
        default: break;
    }
}

void Tutorial_Screen::update(Screen*&) {
    auto now = std::chrono::steady_clock::now();

    if (std::chrono::duration_cast<std::chrono::milliseconds>(now - _last_move_time).count() >= 250) {
        if (_pressed_key != 0) {
            if (!_held_keys[_pressed_key - 1]) {
                move(_tutorial_players[0], _pressed_key);
                _pressed_key = 0;
            }
        }

        for (size_t dir = 0; dir < _held_keys.size(); ++dir) {
            if (_held_keys[dir]) {
                move(_tutorial_players[0], dir + 1); // 1=up, 2=right, 3=down, 4=left
            }
        }

        _last_move_time = now;

        bool task_phase_completed = true;
        for (size_t i = 0; i < _completed_tasks[_task_phase].size(); i++) {
            if (_completed_tasks[_task_phase][i] == "" && _tutorial_tasks[_task_phase][i] != " ") {
                task_phase_completed = false;
                break;
            }
        }
        if (task_phase_completed) {
            _time_after_task_phase += 1;
            if (_time_after_task_phase >= 4) {
                _task_phase++;
                _time_after_task_phase = 0;
            }
        }
    }
}

bool Tutorial_Screen::has_line_of_sight(int x1, int y1, int x2, int y2) const {
    int dx = std::abs(x2 - x1);
    int dy = std::abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;
    if (_tutorial_board[y2][x2] == '#') return false;
    if (_tutorial_board[y2][x2 - sx] == '#' && _tutorial_board[y2 - sy][x2] == '#') return false;
    while (x1 != x2 || y1 != y2) {
        if (_tutorial_board[y1][x1] == '#') return false;
        if (_tutorial_board[y1][x1 - sx] == '#' && _tutorial_board[y1 - sy][x1] == '#') return false;
        int e2 = 2 * err;
        if (e2 > -dy) { err -= dy; x1 += sx; }
        if (e2 < dx) { err += dx; y1 += sy; }
    }
    return true;
}