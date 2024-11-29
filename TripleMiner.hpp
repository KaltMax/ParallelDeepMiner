#pragma once

#include "MiningRobot.hpp"

using namespace std;

class TripleMiner : public MiningRobot {
public:
	TripleMiner();
	~TripleMiner();
	void printRobotDescription() const override;
	vector<int> miningAction(vector<int> gameField) override;
	char getRobotAction(Coordinates currentPosition, int sumAtPosition) override;

private:
};