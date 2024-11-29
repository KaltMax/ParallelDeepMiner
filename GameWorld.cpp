#include "GameWorld.hpp"

GameWorld::GameWorld() {
    worldHeight = 10; // Die Höhe der Welt ist immer 10
    chooseWorldSize();
    createWorld();
    // Initialize mutexes
    fieldMutexes = new mutex * [worldWidth];
    for (int x = 0; x < worldWidth; x++) {
        fieldMutexes[x] = new mutex[worldLength];
    }
}

GameWorld::~GameWorld() {
    // Speicher des Mutex-Array freigeben
    for (int x = 0; x < worldWidth; x++) {
        delete[] fieldMutexes[x];
    }
    delete[] fieldMutexes;
}

void GameWorld::createWorld() {
    // 3D-Vector mit Zufallszahlen füllen
    world.resize(worldWidth, vector<vector<int>>(worldLength, vector<int>(worldHeight)));

    for (int x = 0; x < worldWidth; x++) {
        for (int y = 0; y < worldLength; y++) {
            for (int z = 0; z < worldHeight; z++) {
                world[x][y][z] = 1 + rand() % 9; // Assign random number between 1 and 9
            }
        }
    }

    sumBeforeMining = getSumAllFields(); // Summe der Welt berechnen
}

void GameWorld::chooseWorldSize() {
    int inputLength = 0;
    int inputWidth = 0;   

    while (true) {
        cout << "Please enter the length of the world (between 5 and 50): ";
        cin >> inputLength;  
        if (inputLength >= 5 && inputLength <= 50) {
            worldLength = inputLength;  
            break;  
        }
        cout << "Invalid input. Please enter a number between 5 and 50." << endl;
        cin.clear();  // Clear error flag
        cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Discard bad input
    }

    while (true) {
        cout << "Please enter the width of the world (between 5 and 50): ";
        cin >> inputWidth;  
        if (inputWidth >= 5 && inputWidth <= 50) {
            worldWidth = inputWidth; 
            break;
        }
        cout << "Invalid input. Please enter a number between 5 and 50." << endl;
        cin.clear();  // Clear error flag
        cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Discard bad input
    }
}

void GameWorld::printWorld() const {
    cout << "\nGame World:\n" << endl;
    for (int y = 0; y < worldLength; y++) { // Iteriere über die Y-Achse
        cout << setw(4) << right << y << " | ";
        for (int x = 0; x < worldWidth; x++) {
            int sum = 0;
            for (int z = 0; z < worldHeight; z++) { // Und über die Z-Achse
                sum += world[x][y][z];
            }
            cout << setw(4) << left << sum; // Verwende immer 2 Stellen für die Ausgabe
        }
        cout << endl; // Eine neue Zeile nach jeder Reihe
    }
    cout << endl;
}

void GameWorld::placeRobotsOnWorld(int numberOfRobots) {
    Coordinates robotCoordinates = { 0, 0, 0 };

    for (int robotCount = 1; robotCount <= numberOfRobots; robotCount++) {
        robotCoordinates.x = rand() % worldWidth;
        robotCoordinates.y = rand() % worldLength;
        robotCoordinates = updateRobotZPosition(robotCoordinates);
        robotPositions.push_back(robotCoordinates);
    }
}

Coordinates GameWorld::getPositionRobot(int robotCount) const {
    return robotPositions[robotCount - 1];
}

bool GameWorld::attemptMoveRobot(int robotCount, char direction) {
    Coordinates currentPosition = getPositionRobot(robotCount);
    Coordinates newPosition = currentPosition;

    // Die neue Position berechnen
    switch (direction) {
    case 'w': if (currentPosition.y > 0) newPosition.y--; break;
    case 'a': if (currentPosition.x > 0) newPosition.x--; break;
    case 's': if (currentPosition.y < worldLength - 1) newPosition.y++; break;
    case 'd': if (currentPosition.x < worldWidth - 1) newPosition.x++; break;
    }

    // Überprüfen, ob sich die Position geändert hat
    if (newPosition.x == currentPosition.x && newPosition.y == currentPosition.y) {
        return false; // Keine Bewegung
    }

    // Üperprüfen, ob die neue Position gültig ist
    if (newPosition.x < 0 || newPosition.y < 0 ||
        newPosition.x >= worldWidth || newPosition.y >= worldLength) {
        return false; // Invalid Move - außerhalb des Spielfelds
    }

    lock_guard<mutex> lock(getFieldMutex(newPosition.x, newPosition.y)); // Das Zielfeld sperren

    return updatePosition(robotCount, newPosition); // Die Position des Roboters aktualisieren
}

bool GameWorld::updatePosition(int robotCount, Coordinates newPosition) {
    robotPositions[robotCount - 1] = updateRobotZPosition(newPosition);
    return true;
}

Coordinates GameWorld::updateRobotZPosition(Coordinates& positionRobot) {
    int countNonZero = 0;
    for (int z = 0; z < worldHeight; z++) {
        if (world[positionRobot.x][positionRobot.y][z] != 0) {
            countNonZero++;
        }
    }
    positionRobot.z = countNonZero;
    return positionRobot;
}

vector<int> GameWorld::getVectorAtPosition(int robotCount) {
    Coordinates position = robotPositions[robotCount - 1];

    return world[position.x][position.y];
}

void GameWorld::updateVectorAtPosition(int robotCount, vector<int> vector) {
    if (vector.size() != worldHeight) {
        cout << "The vector must have the same size as the height of the world." << endl;
        return;
    }

    Coordinates position = robotPositions[robotCount - 1];

    world[position.x][position.y] = vector;

    robotPositions[robotCount - 1] = updateRobotZPosition(position);
}

int GameWorld::getSumAtPosition(int robotCount) {
    Coordinates position = robotPositions[robotCount - 1];
    int sum = 0;

    for (int z = 0; z < worldHeight; z++) {
        sum += world[position.x][position.y][z];
    }

    return sum;
}

int GameWorld::getSumAllFields() const{
    int sum = 0;
    for (int x = 0; x < worldWidth; x++) {
        for (int y = 0; y < worldLength; y++) {
            for (int z = 0; z < worldHeight; z++) {
				sum += world[x][y][z];
			}
		}
	}
	return sum;
}

int GameWorld::getSumBeforeMining() const {
    return sumBeforeMining;
}

mutex& GameWorld::getFieldMutex(int x, int y) const {
    return fieldMutexes[x][y];
}