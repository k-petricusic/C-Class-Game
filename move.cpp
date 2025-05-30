#include <iostream>
#include <conio.h>

void move(size_t &x, size_t &y, bool &exit) {
    int input;
    int arrow_input;
    if(_kbhit()) {
        input = _getch(); 
        switch(input) {
            case 27: // esc key
                exit = true;
                break;
            case 87: // W
            case 119: // w
                ++y;
                break;
            case 65: // A
            case 97: // a
                if (x > 0) {
                    --x;
                }
                break;
            case 83: // S
            case 115: // s
                if (y > 0) {
                    --y;
                }
                break;
            case 68: // D
            case 100: // d
                ++x;
                break;
            case 224: // indicates arrow key was pressed
                arrow_input = _getch();
                switch(arrow_input) {
                    case 72: // up arrow
                        ++y;
                        break;
                    case 75: // left arrow
                        if (x > 0) {
                            --x;
                        }
                        break;
                    case 80: // down arrow
                        if (y > 0) {
                            --y;
                        }
                        break;
                    case 77: // right arrow
                        ++x;
                        break;
                }
        }

    }
}

int main() {
    bool exit = false;
    size_t x = 5;
    size_t y = 5;
    while(!exit) {
        move(x, y, exit);
        std::cout << x << " " << y << "\n";
    }
    std::cout << "game over\n";
}