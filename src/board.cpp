#include <fstream>
#include <algorithm>
#include <filesystem>

#include "../include/Screen.h"

Board_Screen::Board_Screen(int lvl) : _level(lvl) {
    std::string level_path = get_executable_dir() + "/levels.txt";
    read_level_from_file(level_path);
}

void Board_Screen::show(tcod::Console& console) {
    int height = static_cast<int>(_board.size());
    for (int i = 0; i < height; ++i) { // i = board y (bottom to top)
        for (size_t j = 0; j < _board[i].size(); ++j) { // j = board x (left to right)
            // Prints the board with the y axis bottom to top
            console.at({static_cast<int>(j), (height - 1) - i}).ch = _board[i][j];
            console.at({static_cast<int>(j), (height - 1) - i}).fg = tcod::ColorRGB{255, 255, 255};
            console.at({static_cast<int>(j), (height - 1) - i}).bg = tcod::ColorRGB{0, 0, 0};
        }
    }
}

bool Board_Screen::move(Movable& obj, size_t direction) {
    char new_pos;
    switch(direction) {
        case 1:
            if (obj.get_y() + 1 < _board.size()) {
                new_pos = _board[obj.get_y() + 1][obj.get_x()];
                if (new_pos == background) {
                    obj.set_y(obj.get_y() + 1);
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
            if (obj.get_y() > 0) {
                new_pos = _board[obj.get_y() - 1][obj.get_x()];
                if (new_pos == background) {
                    obj.set_y(obj.get_y() - 1);
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

/*
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
*/

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
    row_index = height - 1;

    // Read the actual board
    while (row_index < height && std::getline(file, line)) {
        for (size_t x = 0; x < std::min(width, line.size()); ++x) {
            char c = line[x];
            _board[row_index][x] = c;

            if (c == 'O') {
                _players.emplace_back(x, row_index);
            } else if (c == 'G') {
                _guards.emplace_back(x, row_index, 1); // default direction = up
            }
        }
        if (row_index == 0) break;
        --row_index;
    }

    file.close();
}

void Board_Screen::use_user_input(Screen*& current_screen, const SDL_Event& event) {
    if (event.type == SDL_EVENT_KEY_DOWN) {
        switch (event.key.key) {
            case SDLK_W:
                move(_players[0], 1); // For now, only one player
                break;
            case SDLK_D:
                move(_players[0], 2);
                break;
            case SDLK_S:
                move(_players[0], 3);
                break;
            case SDLK_A:
                move(_players[0], 4);
                break;
            case SDLK_Q:
                delete current_screen;
                current_screen = new Level_Select_Screen();
                break;
            default:
                break;
        }
    }
}
