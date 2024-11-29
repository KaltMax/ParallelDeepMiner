#include "SortingMiner.hpp"

SortingMiner::SortingMiner() {
	robotType = "Sorting Miner";
}

SortingMiner::~SortingMiner() {
}

void SortingMiner::printRobotDescription() const {
	cout << "The sorting Miner all z-values of its current x and y coordinates in descending order and is therefore able to mine the the field with the highest points.\n" << endl;
}

vector<int> SortingMiner::miningAction(vector<int> gameField) {
    if (!gameField.empty() && gameField.size() >= 10) {
        // Das Spielfeld sortieren
        sort(gameField.begin(), gameField.end(), [](int a, int b) {
            return a < b;
            });

        addPoints(gameField[9]);  // Den höchsten Wert auf dem Spielfeld abbauen
        gameField[9] = 0;  // Den Wert des höchsten Feldes auf 0 setzen
    }
    return gameField;
}

char SortingMiner::getRobotAction(Coordinates currentPosition, int sumAtPosition) {
    if (currentPosition.z < 6) {
        int randomChance = rand() % 100;  
        if (randomChance < 30) {  // 30% Chance, dass der Roboter abbaut
            return '2';  // Abbauen
        }
        else {
            return '1';  // Bewegen
        }
    }
    else {
        return '2';  // Immer Abbauen, wenn die z-Koordinate größer als 6 ist
    }
}