#pragma once
#include "Sensor.h"
#include "Effector.h"
#include "Fait.h"
#include "Rule.h"
#include <list>
#include <string>
#include <fstream>

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
	vector<Rule> getApplicableRules(Fait fait);
	Pair getSafeNeighbor(vector<Pair> neighbors);
	Pair getMonstreNeighbor(vector<Pair> neighbors);
	void parseRules(string rules);
	vector<string> split(const string& str, const string& delim);
	float CalculateChance(int x, int y);
	Pair ChooseLessRiskyMove(vector<Pair> neighbors);
	vector<Pair> getDirectNeighbors(int x, int y);
	vector<Etat> getDirectNeighborsState(int x, int y);
public:
	Agent(Forest* forest, string rules);
	void setNewForest(Forest* forest);

};