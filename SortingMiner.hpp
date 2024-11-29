#pragma once

#include "MiningRobot.hpp"

using namespace std;

class SortingMiner : public MiningRobot {
public:
	SortingMiner();
	~SortingMiner();
	void printRobotDescription() const override;
	vector<int> miningAction(vector<int> gameField) override;
	char getRobotAction(Coordinates currentPosition, int fieldSum) override;

private:

};