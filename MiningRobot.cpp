#include "MiningRobot.hpp"

MiningRobot::MiningRobot() {
}

MiningRobot::~MiningRobot() {
}

int MiningRobot::getPoints() const {
	return points;
}

void MiningRobot::addPoints(int value) {
	lock_guard<mutex> lock(robotMutex);
	points += value;
}

void MiningRobot::addStolenPoints(int value) {
	lock_guard<mutex> lock(robotMutex);
	points += value;
	stolenPoints += value;
}

int MiningRobot::stealPoints() {
	lock_guard<mutex> lock(robotMutex);
	// Mit einer Wahrscheinlichkeit von 10% werden 5% der Punkte gestohlen
	if (rand() % 100 < 10) {
		int stolenPoints = points / 20;
		points -= stolenPoints;
		return stolenPoints;
	}
	else {
		return 0;
	}
}

void MiningRobot::decreaseLifePoints() {
	lock_guard<mutex> lock(robotMutex);
	int value = 1;

	lifePoints -= value;

	if (rand() % 100 < 40) { // 50% Chance einen weiteren Lebenspunkt zu verlieren
		lifePoints -= value;
	}
	if (lifePoints < 0) {
		lifePoints = 0;
	}
	if (lifePoints == 0) {
		isAlive = false;
	}
}

string MiningRobot::getRobotType() const{
	return robotType;
}

void MiningRobot::setRobotCount(int robotCount) {
	if (robotCount < 0) {
		cout << "Invalid robot count. Please enter 1 or 2." << endl;
	}
	else {
		this->robotCount = robotCount;
	}
}

int MiningRobot::getRobotCount() const {
	return robotCount;
}

int MiningRobot::getLifePoints() const {
	return lifePoints;
}

bool MiningRobot::getIsAlive() {
	lock_guard<mutex> lock(robotMutex);
	return isAlive;
}

void MiningRobot::printStats() const {
	cout << "Robot Type: " << robotType << endl;
	cout << "Points Total: " << points << endl;
	cout << "Points stolen: " << stolenPoints << endl;
	cout << "Life Points: " << lifePoints <<  endl;
}