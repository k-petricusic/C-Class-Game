#include "Screen.h"
#include <iostream>

int main() {
    // Start at the title screen
    Screen* current_screen = new Title_Screen();

    // Loop through screens until the user exits
    while (current_screen != nullptr) {
        current_screen->show();
        current_screen->get_user_input(current_screen);
    }

    std::cout << "Goodbye!\n";
    return 0;
}
