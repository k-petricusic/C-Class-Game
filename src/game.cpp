#include <libtcod.hpp>
#include "Screen.h"
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

/*
void playGame() {
    cout << "Game started! [Simulation of game here]" << endl;
    while (true) {
        Screen* current_screen = new Title_Screen();
        current_screen->show();
        current_screen->get_user_input(current_screen);
    }
}
*/

int main() {
    TCODConsole::initRoot(80, 50, "My Game", false, TCOD_RENDERER_SDL2);
    TCODConsole::root->setDefaultBackground(TCODColor::black);
    TCODConsole::root->setDefaultForeground(TCODColor::white);

    while (!TCODConsole::isWindowClosed()) {
        TCODConsole::root->clear();
        TCODConsole::root->print(1, 1, "Hello World");
        TCODConsole::flush();
    }
    TCODConsole::deinit();
    return 0;
}
