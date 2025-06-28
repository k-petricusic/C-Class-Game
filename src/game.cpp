#include <iostream>
#include <cstdlib>
#include "../include/Guard.h"
#include "../include/Movable.h"
#include "../include/Player.h"
#include "../include/Screen.h"
#include "board.cpp"
#include "menu_screens.cpp"
#include "move.cpp"

using namespace std;

void playGame() {
    cout << "Game started! [Simulation of game here]" << endl;
    while (true) {
        Screen* current_screen = new Title_Screen();
        current_screen->show();
        current_screen->get_user_input(current_screen);
    }
}

int main() {
    int choice;
    cout << "=== Welcome to the Game ===" << endl;
    cout << "1. Play Game\n2. Exit\n";
    cout << "Enter your choice: ";
    cin >> choice;

    if (choice == 1) {
        playGame();
    } else {
        cout << "Exiting..." << endl;
    }

    return 0;
}
