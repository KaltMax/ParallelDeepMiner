#include <iostream>
#include <cstdlib>
#include <ctime>
#include "Game.hpp"
#include "VisualsAndUtility.hpp"

using namespace std;

int main() {
    bool playGame = true;
    srand(static_cast<unsigned int>(time(0)));

    VisualsAndUtility::printTitle();

    while(playGame){
        Game game;

        game.gameLoop();

        playGame = game.playAgain();
    }

    VisualsAndUtility::printGoodbye();

    return 0;
}