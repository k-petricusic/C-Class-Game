#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

#include "Move.h"
#include "Screen.h"

void Board_Screen::show() {
    clear();

    if (_board.empty()) {
        std::cout << "[ERROR] _board is empty!\n";
        return;
    }

    size_t rows = _board.size();
    size_t cols = _board[0].size();

    std::vector<std::vector<char>> display = _board;

    // Mark player
    if (_player.get_y() < rows && _player.get_x() < cols) {
        display[_player.get_y()][_player.get_x()] = 'O';
    }

    // Mark guards
    for (const Guard& g : _guards) {
        if (g.get_y() < rows && g.get_x() < cols) {
            display[g.get_y()][g.get_x()] = 'G';
        }
    }

    // Print from bottom to top
    std::cout << "\n======== MAP ========\n";
    for (int i = static_cast<int>(rows) - 1; i >= 0; --i) {
        for (char c : display[i]) {
            std::cout << c;
        }
        std::cout << "\n";
    }

    std::cout << "======================\n";
}
 

bool Board_Screen::move(Movable& obj, size_t direction) {
    char new_pos;
    switch (direction) {
        case 1:
            if (obj.get_y() + 1 < _board.size()) {
                new_pos = _board[obj.get_y() + 1][obj.get_x()];
                if (new_pos != obstacle) obj.set_y(obj.get_y() + 1);
            }
            break;
        case 2:
            if (obj.get_x() + 1 < _board[0].size()) {
                new_pos = _board[obj.get_y()][obj.get_x() + 1];
                if (new_pos != obstacle) obj.set_x(obj.get_x() + 1);
            }
            break;
        case 3:
            if (obj.get_y() > 0) {
                new_pos = _board[obj.get_y() - 1][obj.get_x()];
                if (new_pos != obstacle) obj.set_y(obj.get_y() - 1);
            }
            break;
        case 4:
            if (obj.get_x() > 0) {
                new_pos = _board[obj.get_y()][obj.get_x() - 1];
                if (new_pos != obstacle) obj.set_x(obj.get_x() - 1);
            }
            break;
    }

    return true;
}

void Board_Screen::update_guard_los() {
    bool exit_loop;
    for (size_t i = 0; i < _guards.size(); ++i) {
        exit_loop = false;
        for (size_t j = 0; j < _board.size() && j < 6 && !exit_loop; j++) {
            switch (_guards[i].get_direction()) {
                case 1:
                    if (_guards[i].get_y() + j < _board.size()) {
                        if (_board[_guards[i].get_y() + j][_guards[i].get_x()] == obstacle) {
                            exit_loop = true;
                        }
                    }
                    break;
                case 2:
                    if (_guards[i].get_x() + j < _board[0].size()) {
                        if (_board[_guards[i].get_y()][_guards[i].get_x() + j] == obstacle) {
                            exit_loop = true;
                        }
                    }
                    break;
                case 3:
                    if (_guards[i].get_y() >= j) {
                        if (_board[_guards[i].get_y() - j][_guards[i].get_x()] == obstacle) {
                            exit_loop = true;
                        }
                    }
                    break;
                case 4:
                    if (_guards[i].get_x() >= j) {
                        if (_board[_guards[i].get_y()][_guards[i].get_x() - j] == obstacle) {
                            exit_loop = true;
                        }
                    }
                    break;
            }
        }
    }
}

void Board_Screen::read_levels_from_file(const std::string& filename) {
    std::cout << "[DEBUG] Attempting to open: " << filename << "\n";

    _guards.clear();
    _board.clear();

    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "[ERROR] Failed to open level file: " << filename << "\n";
        return;
    }

    std::string line;
    int current_level = 0;
    bool found_level = false;
    size_t width = 0, height = 0;
    size_t row_index = 0;

    // Read the level number (first line of file)
    if (std::getline(file, line)) {
        std::istringstream iss(line);
        iss >> current_level;
        std::cout << "[DEBUG] Requested level: #" << current_level << "\n";
    } else {
        std::cerr << "[ERROR] Failed to read first line from file\n";
        return;
    }

    // Look for the level marker like "#1"
    while (std::getline(file, line)) {
        if (line == "#" + std::to_string(current_level)) {
            std::cout << "[DEBUG] Found level marker: " << line << "\n";
            found_level = true;
            break;
        }
    }

    if (!found_level) {
        std::cerr << "[ERROR] Could not find marker: #" << current_level << " in file\n";
        return;
    }

    // Read dimensions line like "10 6"
    if (!std::getline(file, line)) {
        std::cerr << "[ERROR] Missing dimensions line after level marker\n";
        return;
    }

    std::istringstream dim_stream(line);
    if (!(dim_stream >> width >> height)) {
        std::cerr << "[ERROR] Could not parse dimensions from line: " << line << "\n";
        return;
    }

    std::cout << "[DEBUG] Parsed dimensions: width=" << width << ", height=" << height << "\n";

    _board.resize(height, std::vector<char>(width, ' '));
    row_index = height - 1;

    // Read each row of the board
    while (row_index != static_cast<size_t>(-1) && std::getline(file, line)) {
        std::cout << "[DEBUG] Reading row " << row_index << ": " << line << "\n";

        for (size_t x = 0; x < std::min(width, line.size()); ++x) {
            char c = line[x];
            _board[row_index][x] = c;

            if (c == 'O') {
                _player.set_position(x, row_index);
                std::cout << "[DEBUG] Player set at (" << x << ", " << row_index << ")\n";
            } else if (c == 'G') {
                _guards.emplace_back(x, row_index, 1);
                std::cout << "[DEBUG] Guard added at (" << x << ", " << row_index << ")\n";
            }
        }

        if (row_index == 0) break;
        --row_index;
    }

    std::cout << "[DEBUG] Board loaded successfully.\n";
    file.close();
}


void Board_Screen::load_and_display_level(const std::string& filename) {
    read_levels_from_file(filename);
    std::cout << "\nRendering Level #" << _level << ":\n\n";
    show();
    std::cout << "\n\n";
}

void Board_Screen::get_user_input(Screen*& current_screen) {
    bool exit_game = false;

    while (!exit_game) {
        show();

        std::cout << "\n[DEBUG] Use WASD to move, ESC to exit.\n";

        if (kbhit()) {
            int input = getch();
            switch (input) {
                case 27: exit_game = true; break;
                case 'w': case 'W': move(_player, 1); break;
                case 'd': case 'D': move(_player, 2); break;
                case 's': case 'S': move(_player, 3); break;
                case 'a': case 'A': move(_player, 4); break;
            }
            update_guard_los();  // Optional logic for future
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(75));
    }

    delete current_screen;
    current_screen = new Level_Select_Screen();
}
