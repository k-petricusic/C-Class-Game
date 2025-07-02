#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

#include "Screen.h"

void Board_Screen::show() {
    clear();
    std::ostringstream oss;

    for (int i = static_cast<int>(_board.size()) - 1; i >= 0; --i) { // Print from bottom to top
        for (size_t j = 0; j < _board[i].size(); ++j) {
            oss << _board[i][j];
        }
        oss << "\n";
    }
    std::cout << oss.str();
}

bool Board_Screen::move(Movable& obj, size_t direction) {
    char new_pos;
    switch(direction) {
        case 1:
            if (obj.get_y() + 1 < _board.size()) {
                new_pos = _board[obj.get_y() + 1][obj.get_x()];
                if (new_pos != obstacle) {
                    obj.set_y(obj.get_y() + 1);
                }
            }
            break;
        case 2:
            if (obj.get_x() + 1 < _board[0].size()) {
                new_pos = _board[obj.get_y()][obj.get_x() + 1];
                if (new_pos != obstacle) {
                    obj.set_x(obj.get_x() + 1);
                }
            }
            break;
        case 3:
            if (obj.get_y() > 0) {
                new_pos = _board[obj.get_y() - 1][obj.get_x()];
                if (new_pos != obstacle) {
                    obj.set_y(obj.get_y() - 1);
                }
            }
            break;
        case 4:
            if (obj.get_x() > 0) {
                new_pos = _board[obj.get_y()][obj.get_x() - 1];
                if (new_pos != obstacle) {
                    obj.set_x(obj.get_x() - 1);
                }
            }
            break;
    }

    return true; // Always returns true for now (could return false if no move occurred)
}

void Board_Screen::update_guard_los() {
    bool exit_loop;
    for (size_t i = 0; i < _guards.size(); ++i) {
        exit_loop = false;
        for (size_t j = 0; j < _board.size() && j < 6 && !exit_loop; j++) { // will have to change if board is not square and add edge cases
            switch(_guards[i].get_direction()) {
                case 1:
                    if (_guards[i].get_y() + j < _board.size()) {
                        if (_board[_guards[i].get_y() + j][_guards[i].get_x()] == obstacle) {
                            exit_loop = true;
                        } 
                        else {
                            //set guard los in array or something
                        }
                    }
                    break;
                case 2:
                    if (_guards[i].get_x() + j < _board.size()) {
                        if (_board[_guards[i].get_y()][_guards[i].get_x() + j] == obstacle) {
                            exit_loop = true;
                        } 
                        else {
                            //set guard los in array or something
                        }
                    }
                    break;
                case 3:
                    if (_guards[i].get_y() - j >= 0) {
                        if (_board[_guards[i].get_y() - j][_guards[i].get_x()] == obstacle) {
                            exit_loop = true;
                        } 
                        else {
                            //set guard los in array or something
                            if (_guards[i].get_y() - j == 0) {
                                exit_loop = true;
                            }
                        }
                    }
                    break;
                case 4:
                    if (_guards[i].get_x() - j >= 0) {
                        if (_board[_guards[i].get_y()][_guards[i].get_x() - j] == obstacle) {
                            exit_loop = true;
                        } 
                        else {
                            //set guard los in array or something
                            if (_guards[i].get_x() - j == 0) {
                                exit_loop = true;
                            }
                        }
                    }
                    break;
            }
        }
    }
}
void Board_Screen::read_levels_from_file(const std::string& filename) {
    _guards.clear();
    _board.clear();

    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open level file: " << filename << "\n";
        return;
    }

    std::string line;
    int current_level = 0;
    bool found_level = false;
    size_t width = 0, height = 0;
    size_t row_index = 0;

    // Step 1: Read first line (save level)
    if (std::getline(file, line)) {
        std::istringstream iss(line);
        iss >> current_level;
    }

    // Step 2: Look for matching #<level>
    while (std::getline(file, line)) {
        if (line == "#" + std::to_string(current_level)) {
            found_level = true;
            break;
        }
    }

    if (!found_level) {
        std::cerr << "Level #" << current_level << " not found.\n";
        return;
    }

    // Step 3: Read dimensions
    if (!std::getline(file, line)) {
        std::cerr << "Expected dimensions line after #" << current_level << "\n";
        return;
    }

    std::istringstream dim_stream(line);
    dim_stream >> width >> height;

    _board.resize(height, std::vector<char>(width, ' '));
    row_index = height - 1;

    // Step 4: Read the actual board
    while (row_index < height && std::getline(file, line)) {
        for (size_t x = 0; x < std::min(width, line.size()); ++x) {
            char c = line[x];
            _board[row_index][x] = c;

            if (c == 'O') {
                _player.set_position(x, row_index);
            } else if (c == 'G') {
                _guards.emplace_back(x, row_index, 1); // default direction = up
            }
        }
        if (row_index == 0) break;
        --row_index;
    }

    file.close();
}

void Board_Screen::load_and_display_level(const std::string& filename) {
    read_levels_from_file(filename);
    std::cout << "\nRendering Level #" << _level << ":\n\n";
    show();
    std::cout << "\n\n";
}
