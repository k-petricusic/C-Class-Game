#include "Screen.h"

int main() {
    Screen* current_screen = new Title_Screen();

    while (current_screen != nullptr) {
        current_screen->show();
        current_screen->get_user_input(current_screen);
    }

    std::cout << "Goodbye!\n";
    return 0;
}
