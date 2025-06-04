#include <iostream>
#include <cstdlib>
using namespace std;

void playGame() {
    cout << "Game started! [Simulation of game here]" << endl;
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
