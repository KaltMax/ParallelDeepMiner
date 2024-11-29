#include "Game.hpp"
#include "GameWorld.hpp"
#include "VisualsAndUtility.hpp"
#include "MiningRobot.hpp"
#include "RandomMiner.hpp"
#include "TripleMiner.hpp"
#include "SortingMiner.hpp"

using namespace std;

Game::Game() {
}

Game::~Game() {
    for (MiningRobot* robot : robots) {
        delete robot;
    }
}

void Game::gameLoop() {
    atomic<bool> gameFinished(false);

    chooseNumberOfRobots();
    chooseRobot();
    gameWorld.placeRobotsOnWorld(numberOfRobots);

    cout << "The game has started!\n" << endl;
    gameWorld.printWorld();
    printRobotStats();
    cout << "\nSum of all points of all fields before mining: " << gameWorld.getSumBeforeMining() << endl;

    auto programStart = chrono::steady_clock::now();

    vector<long long> executionTimes(robots.size()); // Vektor für zum Speichern der Execution Times
    vector<thread> threads;  // Vektor für die einzelnen Threads

    for (int i = 0; i < robots.size(); i++) {
        threads.emplace_back([this, i, &gameFinished, &executionTimes]() {
            auto startTime = chrono::steady_clock::now();

            while (!gameFinished && robots[i]->getIsAlive()) {
                robotAction(robots[i]);

                if (gameWorld.getSumAllFields() == 0) {
                    gameFinished = true;
                    break;
                }
            }

            auto endTime = chrono::steady_clock::now();
            executionTimes[i] = chrono::duration_cast<chrono::milliseconds>(endTime - startTime).count();
            });
    }

    // Auf die fertige Ausführung aller Threads warten
    for (auto& thread : threads) {
        thread.join();
    }

    auto programEnd = chrono::steady_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(programEnd - programStart).count();

    cout << "\n\nThe game has ended! All fields are depleted!\n" << endl;
    gameWorld.printWorld();
    printRobotStats();
    printProgramStats(executionTimes, duration);
}

void Game::chooseNumberOfRobots() {
    int numberOfRobots = 0;

    while (true) {
        cout << "How many robots should be in the game?" << endl;
        cout << "Choose a number between 5 and 10: ";
        cin >> numberOfRobots;

        if (numberOfRobots >= 5 && numberOfRobots <= 10) {
            this->numberOfRobots = numberOfRobots;
            break;
        }
        else {
            cout << "Invalid input. Please enter a number between 5 and 10." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
    VisualsAndUtility::clearScreen();
}

void Game::chooseRobot() {
    int robotChoice = 0;
    int robotCount = 1;

    for (int i = 0; i < numberOfRobots; i++) {
        // Zufälligen Mining Robot auswählen
        robotChoice = 1 + rand() % 3;

        if (robotChoice == 1) {
			robots.push_back(new RandomMiner());
		}
        else if (robotChoice == 2) {
			robots.push_back(new TripleMiner());
		}
        else if (robotChoice == 3) {
			robots.push_back(new SortingMiner());
		}
        robots[i]->setRobotCount(robotCount);
        robotCount++;
	}
}           

void Game::printRobotStats() {
    for (int i = 0; i < robots.size(); i++) {
        Coordinates positionRobot = gameWorld.getPositionRobot(robots[i]->getRobotCount());

        cout << "Robot " << (i + 1) << ": " << endl;
        robots[i]->printStats();
        cout << "Position: " << positionRobot.x << " / " << positionRobot.y << " / " << positionRobot.z << "\n" << endl;
    }
}

void Game::printProgramStats(vector<long long> executionTimes, long long duration) {
    cout << "\nSum of all points of all fields before mining: " << gameWorld.getSumBeforeMining() << endl;
    cout << "\nSum of all points of all robots: " << getSumRobotPoints() << "\n" << endl;
    printExecutionTimes(executionTimes); // Ausgabe der Zeit, die jeder Roboter für die Ausführung benötigt hat
    cout << "Total execution time of the Game Loop: " << duration << " ms" << endl;
}

void Game::robotAction(MiningRobot* robot) {
    Coordinates currentPosition = gameWorld.getPositionRobot(robot->getRobotCount());

    char action = getAction(robot->getRobotCount());

    if (action == '1') {  // Move
        moveRobot(robot);
    }
    else if (action == '2') { // Mine
        lock_guard<mutex> lock(gameWorld.getFieldMutex(currentPosition.x, currentPosition.y));
        vector<int> gameField = gameWorld.getVectorAtPosition(robot->getRobotCount());
        gameField = robot->miningAction(gameField);
        gameWorld.updateVectorAtPosition(robot->getRobotCount(), gameField);
    }

    robotAttack(robot);
}

char Game::getAction(int robotCount) {
    int sumAtPosition = gameWorld.getSumAtPosition(robotCount); // Summe der Punkte an der Position des Roboters
    Coordinates currentPosition = gameWorld.getPositionRobot(robotCount); // Position des Roboters

    return robots[robotCount - 1]->getRobotAction(currentPosition, sumAtPosition);
}

void Game::moveRobot(MiningRobot* robot) {
    const char directions[] = { 'w', 'a', 's', 'd' };
    int robotCount = robot->getRobotCount();
    Coordinates currentPosition = gameWorld.getPositionRobot(robotCount);
    int attempts = 0;

    while (attempts < 100) {  // 100 Versuche, um eine gültige Bewegung zu finden
        char direction = directions[rand() % 4];  // Zufällige Richtung auswählen

        // Überprüfen, ob der Roboter sich bewegen kann
        if (gameWorld.attemptMoveRobot(robotCount, direction)) {
            return; 
        }
        attempts++;
    }
}

void Game::robotAttack(MiningRobot* activeRobot) {
    Coordinates currentPosition = gameWorld.getPositionRobot(activeRobot->getRobotCount());

    lock_guard<mutex> lock(gameWorld.getFieldMutex(currentPosition.x, currentPosition.y)); // Das Feld sperren

    // Iteriere über alle robots und greife jeden an, der sich auf demselben Feld befindet
    for (MiningRobot* otherRobot : robots) {
        if (otherRobot != activeRobot) {
            Coordinates otherPosition = gameWorld.getPositionRobot(otherRobot->getRobotCount());
            if (otherPosition.x == currentPosition.x && otherPosition.y == currentPosition.y) {
                otherRobot->decreaseLifePoints(); // Lebenspunkte des anderen Roboters verringern
                activeRobot->addStolenPoints(otherRobot->stealPoints()); // Punkte stehlen und die gestohlenen Punkte dem activeRobot hinzufügen
            }
        }
    }
}

int Game::getSumRobotPoints() const{
    int sum = 0;
    for (int i = 0; i < robots.size(); i++) {
        sum += robots[i]->getPoints();
	}
	return sum;
}

void Game::printExecutionTimes(vector<long long>& executionTimes) const {
    	cout << "Execution times of the robots:" << endl;
        for (int i = 0; i < executionTimes.size(); i++) {
		cout << "Robot " << (i + 1) << ": " << executionTimes[i] << " ms" << endl;
	}
	cout << endl;
}

bool Game::playAgain() {
    string input;
    char playAgainChoice = '\0';

    VisualsAndUtility::enterToContinue();

    while (true) {
        cout << "Do you want to play again? (y/n): ";
        getline(cin, input);

        if (!input.empty()) {
            playAgainChoice = input[0];
        }
        else {
            cout << "No input detected. Please enter 'y' or 'n'." << endl;
            continue;
        }

        if (playAgainChoice == 'y') {
            VisualsAndUtility::clearScreen();
            return true;
        }
        else if (playAgainChoice == 'n') {
            VisualsAndUtility::clearScreen();
            return false;
        }
        else {
            cout << "Invalid input. Please enter 'y' or 'n'." << endl;
        }
    }
    VisualsAndUtility::clearScreen();
}

int Game::getNumberOfRobots() const{
    return numberOfRobots;
}