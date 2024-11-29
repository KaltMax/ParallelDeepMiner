#include "RandomMiner.hpp"

RandomMiner::RandomMiner() {
	robotType = "Random Miner";
}

RandomMiner::~RandomMiner() {
}

void RandomMiner::printRobotDescription() const {
	cout << "The random Miner can mine 3 randomly chosen z-values.\n" << endl;
}

vector<int> RandomMiner::miningAction(vector<int> gameField) {
    int points = 0;

    // Wählt 4 zufällige Felder aus und addiert die Punkte
    for (int i = 0; i < 3; ++i) {
        int randomIndex = getRandomIndex(gameField.size());
        points += gameField[randomIndex];
        gameField[randomIndex] = 0;
    }

    addPoints(points);

    return gameField;
}

int RandomMiner::getRandomIndex(int size) const {
    return rand() % size;
}

char RandomMiner::getRobotAction(Coordinates currentPosition, int sumAtPosition) {
    int random = rand() % 2 + 1;

    if (random == 1) {
        return '1';
    }
    else {
        return '2';
    }
}