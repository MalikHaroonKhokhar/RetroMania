#include <iostream>
#include <chrono>
#include <thread>

void clearScreen() {
    std::cout << "\033[2J\033[1;1H";
}

void moveCursor(int x, int y) {
    std::cout << "\033[" << y << ";" << x << "H";
}
int main() {
     
    //pL for cursor location
    char input;
    int startPoint = 1;
    std::cout << "enter x to clear the screen" << std::endl;


    std::cin >> input;
    while (input != 'x' && input != 'X') {


        if (input == 'w' || input == 'W') {
            moveCursor(startPoint, 1);
            startPoint+=5;
        }

        std::cin >> input;


    }
    if (input == 'x' || input =='X') {
        clearScreen();
    }
    return 0;

}