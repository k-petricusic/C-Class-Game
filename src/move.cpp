#include <iostream>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

// Replacement for _getch()
int getch() {
    struct termios oldt, newt;
    int ch;
    tcgetattr(STDIN_FILENO, &oldt);           // Save old settings
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);         // Disable canonical mode and echo
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);  // Apply new settings
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);  // Restore old settings
    return ch;
}

// Replacement for _kbhit()
bool kbhit() {
    struct termios oldt, newt;
    int oldf;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
    int ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);
    if (ch != EOF) {
        ungetc(ch, stdin);
        return true;
    }
    return false;
}

// Movement function
void move(size_t &x, size_t &y, bool &exit) {
    int input;
    int arrow_input;
    if (kbhit()) {
        input = getch();
        switch(input) {
            case 27: // esc key
                exit = true;
                break;
            case 'w': case 'W':
                ++y;
                break;
            case 'a': case 'A':
                if (x > 0) --x;
                break;
            case 's': case 'S':
                if (y > 0) --y;
                break;
            case 'd': case 'D':
                ++x;
                break;
            // No 224 in Linux, but you can extend later with escape sequences
        }
    }
}

