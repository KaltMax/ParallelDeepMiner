#pragma once

#include "MiningRobot.hpp"

using namespace std;

class RandomMiner : public MiningRobot {
public:
	RandomMiner();
	~RandomMiner();
	void printRobotDescription() const override;
	vector<int> miningAction(vector<int> gameField) override;
	char getRobotAction(Coordinates currentPosition, int sumAtPosition) override;

private:
	int getRandomIndex(int size) const;

};