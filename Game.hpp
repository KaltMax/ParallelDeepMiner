#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <thread>
#include <mutex>
#include <future>
#include "GameWorld.hpp"
#include "MiningRobot.hpp"
#include "RandomMiner.hpp"
#include "TripleMiner.hpp"
#include "SortingMiner.hpp"

using namespace std;

class Game {
	public:
		Game();
		~Game();
		void gameLoop();
		bool playAgain();
		int getNumberOfRobots() const;

	private:
		void chooseRobot();
		void chooseNumberOfRobots();
		void printRobotStats();
		void printProgramStats(vector<long long> executionTimes, long long duration);
		void robotAction(MiningRobot* robot);
		char getAction(int robotCount);
		void moveRobot(MiningRobot* robot);
		int getSumRobotPoints() const;
		void printExecutionTimes(vector<long long>& executionTimes) const;
		void robotAttack(MiningRobot* robot);

		GameWorld gameWorld;
		vector<MiningRobot*> robots; // Vektor für die Spieler
		int numberOfRobots = 0;
};