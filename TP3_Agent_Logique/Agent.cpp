#include "Agent.h"

Agent::Agent(Forest* forest, vector<Rule> rules) {
	this->rules = rules;
	score = 0;
	forestSize = forest->getSize();
	setNewForest(forest);

	for (int x = 0; x < forest->getSize(); x++) {
		vector<Fait> line;
		for (int y = 0; y < forest->getSize(); y++) {
			line.push_back(Fait(x, y));
		}
		this->exploredGrid.push_back(line);
	}

	this->exploredGrid[0][0].etat = VIDE;

	moteurInference();
}

void Agent::setNewForest(Forest* forest) {
	this->sensor = new Sensor(forest);
	this->effector = new Effector(forest);
}

vector<Pair> Agent::getUnexploredNeighbors() {

	vector<Pair> unexploredNeighbors;

	for (int x = 0; x < forestSize; x++)
		for (int y = 0; y < forestSize; y++)
			if (exploredGrid[x][y].etat == INCONNU) {

				bool neighbor = false;

				if (x > 0) if (exploredGrid[x - 1][y].etat != INCONNU) neighbor = true;
				if (x < forestSize - 1) if (exploredGrid[x + 1][y].etat != INCONNU) neighbor = true;
				if (y > 0) if (exploredGrid[x][y - 1].etat != INCONNU) neighbor = true;
				if (y < forestSize - 1) if (exploredGrid[x][y + 1].etat != INCONNU) neighbor = true;
				if (x > 0 && y > 0) if (exploredGrid[x - 1][y - 1].etat != INCONNU) neighbor = true;
				if (x < forestSize - 1 && y > 0) if (exploredGrid[x + 1][y - 1].etat != INCONNU) neighbor = true;
				if (x > 0 && y < forestSize - 1) if (exploredGrid[x - 1][y + 1].etat != INCONNU) neighbor = true;
				if (x < forestSize - 1 && y < forestSize - 1) if (exploredGrid[x + 1][y + 1].etat != INCONNU) neighbor = true;

				if (neighbor) unexploredNeighbors.push_back(make_pair(x, y));
			}
	return unexploredNeighbors;
}

void Agent::moteurInference() {
}