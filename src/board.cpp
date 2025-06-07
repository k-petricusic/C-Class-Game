#include "Screen.h"

void Board_Screen::show() {
    clear();
    std::ostringstream oss;

    for (size_t i = _board.size() - 1; i >= 0; --i) { // Print from bottom to top
        for (size_t j = 0; j < _board[i].size(); ++j) {
            oss << _board[i][j];
        }
        oss << "\n";
    }
    std::cout << oss.str();
}
/*
void Board_Screen::get_user_input(Screen*& current_screen) {
    // This function is currently empty, as the user input handling is not implemented.
}
*/
bool Board_Screen::move(Movable& obj, size_t direction) {
    char new_pos;
    switch(direction) {
    case 1:
        new_pos = _board[obj.get_y()+1][obj.get_x()]; // We access this a lot, so we can cache it
        if (new_pos != obstacle && obj.get_y() < _board.size() - 1) { // Check if moving down is valid
            obj.set_y(obj.get_y()+1);
        }
        break;
    case 2:
        new_pos = _board[obj.get_y()][obj.get_x()+1];
        if (new_pos != obstacle && obj.get_x() < _board[0].size() - 1) {
            obj.set_x(obj.get_x()+1);
        }
        break;
    case 3:
        new_pos = _board[obj.get_y()-1][obj.get_x()];
        if (new_pos != obstacle && obj.get_y() > 0) {
            obj.set_y(obj.get_y()-1);
        }
        break;
    case 4:
        new_pos = _board[obj.get_y()][obj.get_x()-1];
        if (new_pos != obstacle && obj.get_x() > 0) {
            obj.set_x(obj.get_x()-1);
        }
        break;
    }
}

//This one is all yours Kris
void Board_Screen::read_level_from_file(const int level) {
    // Implementation for reading the level from a file
}
