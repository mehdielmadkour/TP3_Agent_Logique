#pragma once
#include "Sensor.h"
#include "Effector.h"
#include "Fait.h"
#include "Rule.h"
#include <list>

using namespace std;
typedef pair<int, int> Pair;
class Agent {
private:
	int forestSize;
	vector<vector<Fait>> exploredGrid;
	int score;
	int x = 0;
	int y = 0;
	Sensor* sensor;
	Effector* effector;
	void moteurInference();
	vector<Pair> getUnexploredNeighbors();
	list<Fait> pile;
	vector<Rule> rules;
	void print();
public:
	Agent(Forest* forest, vector<Rule> rules);
	void setNewForest(Forest* forest);

};