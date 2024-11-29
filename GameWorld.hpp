#pragma once
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <string>
#include <algorithm>
#include <mutex>
#include "Coordinates.hpp"
#include "VisualsAndUtility.hpp"


using namespace std;

class GameWorld {
public:
    GameWorld();
    ~GameWorld();
    void printWorld() const;
    void placeRobotsOnWorld(int numberOfRobots);
    struct Coordinates getPositionRobot(int robotCount) const;
    bool attemptMoveRobot(int robotCount, char direction);
    bool updatePosition(int robotCount, Coordinates newPosition);
    vector<int> getVectorAtPosition(int robotCount);
    void updateVectorAtPosition(int robotCount, vector<int> vector);
    int getSumAtPosition(int robotCount);
    int getSumAllFields() const;
    int getSumBeforeMining() const;
    mutex& getFieldMutex(int x, int y) const;

private:
    int worldLength;
    int worldWidth;
    int worldHeight;
    int sumBeforeMining;
    vector<vector<vector<int>>> world;
    vector<Coordinates> robotPositions;

    mutex** fieldMutexes;

    Coordinates updateRobotZPosition(Coordinates& positionRobot);
    void chooseWorldSize();
    void createWorld();

};