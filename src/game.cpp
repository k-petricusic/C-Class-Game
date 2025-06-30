#include <libtcod.hpp>
#include "Screen.h"
#include <iostream>

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
