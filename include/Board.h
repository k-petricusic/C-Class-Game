#pragma once

#include <string>
#include <vector>
#include <iostream>

class Board {
    private:
        char background = ' ';
        char obstacle = 'X';
        std::vector<std::vector<char>> board;
        std::vector<Guard> guards;
    public:
        Board(size_t x_size = 50, size_t y_size = 50, size_t num_of_guards) {
            board.resize(y_size);
            for (size_t i = 0; i < y_size; ++i) {
                board[i].resize(x_size);
            }
            guards.resize(num_of_guards);
        }
        void print(Movable &player) {
            std::string output;
            size_t y_index = 0;
            size_t x_index = 0;
            for (size_t i = 0; i < board.size(); ++i) {
                y_index = board.size()-i-1;
                for (size_t j = 0; j < board[i].size(); ++j) {
                    x_index = j;
                    if (y_index == player.get_y() && x_index = player.get_x()) {
                        output += 'O';
                    }
                    else if (has_guard(x_index, y_index)) {
                        output += 'G';
                    }
                    else {
                        output += board[y_index][x_index];
                    }
                }
                output += "\n";
            }
            std::cout << output;
        }

        bool has_guard(size_t x, size_t y) {
            for (size_t i = 0; i < guards.size(); ++i) {
                if (guards[i].get_x() == x && guards[i].get_y() == y) {
                    return true;
                }
            }
            return false;
        }
        void move(Movable &object, int dir) {
            switch(dir) {
                case 1:
                    if (object.get_y()+1 != "-") {
                        object.set_y(object.get_y()+1);
                    }
                    break;
                case 2:
                    if (object.get_x()+1 != "|") {
                         object.set_x(object.get_x()+1);
                    }
                    break;
                case 3:
                    if (object.get_y()-1 != "-") {
                        object.set_y(object.get_y()-1);
                    }
                    break;
                case 4:
                    if (object.get_x()-1 != "|") {
                        object.set_x(object.get_x()-1);
                    }
                    break;
            }
        }
        void make_wall(size_t x, size_t y) {
            board[y][x] = obstacle;
        }
        void clear() {
            for (size_t i = 0; i < board.size(); ++i) {
                for (size_t j = 0; j < board[i].size(); ++j) {
                    board[i][j] = background;
                }
            }
        }

}