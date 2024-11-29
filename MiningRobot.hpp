#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <mutex>
#include "Coordinates.hpp"


using namespace std;

class MiningRobot {
	public:
		MiningRobot();
		virtual ~MiningRobot();
		void printStats() const;
		int getPoints() const;
		void addPoints(int value);
		void addStolenPoints(int value);
		int stealPoints();
		void decreaseLifePoints();
		string getRobotType() const;
		void setRobotCount(int robotCount);
		int getRobotCount() const;
		int getLifePoints() const;
		bool getIsAlive();
		virtual void printRobotDescription() const = 0;
		virtual char getRobotAction(Coordinates currentPosition, int sumAtPosition) = 0;
		virtual vector<int> miningAction(vector<int> gameField) = 0;
		

	protected:
		int points = 0;
		int stolenPoints = 0;
		string robotType = "";
		int robotCount = 0;
		int lifePoints = 100;
		bool isAlive = true;
		mutex robotMutex;
};