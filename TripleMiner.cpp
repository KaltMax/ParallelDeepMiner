#include "TripleMiner.hpp"

TripleMiner::TripleMiner() {
	robotType = "Triple Miner";
}

TripleMiner::~TripleMiner() {
}

void TripleMiner::printRobotDescription() const {
	cout << "The Triple Miner is a robot that takes the value of its coordinates and the next two z-values.\n" << endl;
}

vector<int> TripleMiner::miningAction(vector<int> gameField) {
    int points = 0;
   
    if (gameField.size() > 9) { // berprüfen, ob das Spielfeld mindestens 10 Elemente hat, um sicher auf den Index 9 zugreifen zu können
        int count = 0;
        for (int i = 9; i >= 0 && count < 3; --i) { // Iteriere rückwärts durch das Spielfeld
            if (gameField[i] > 0) { // Überprüfen, ob der aktuelle Index einen Wert größer als 0 hat
                points += gameField[i]; 
                gameField[i] = 0; 
                ++count;  
            }
        }
    }
    addPoints(points);
	return gameField;
}

char TripleMiner::getRobotAction(Coordinates currentPosition, int sumAtPosition) {
    if (sumAtPosition < 20) {
        int randomChance = rand() % 100;  // Generate a random number between 0 and 99
        if (randomChance < 30) {  // 30% Chance sich zu bewegen
            return '1';  // Bewegen
        }
        else {
            return '2';  // Abbauen
        }
    }
    else {
        return '2';  // Wenn die Summe an der Position größer als 20 ist, dann abbauen
    }
}