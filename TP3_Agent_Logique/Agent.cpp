#include "Agent.h"

Agent::Agent(Forest* forest, string rules) {
	
	parseRules(rules);

	score = 0;
	setNewForest(forest);

	print();
	moteurInference();
}

void Agent::setNewForest(Forest* forest) {

	forestSize = forest->getSize();
	this->sensor = new Sensor(forest);
	this->effector = new Effector(forest);

	exploredGrid.clear();

	for (int x = 0; x < forest->getSize(); x++) {
		vector<Fait> line;
		for (int y = 0; y < forest->getSize(); y++) {
			Fait fait = Fait(x, y);
			fait.etat = INCONNU;
			line.push_back(fait);
		}
		this->exploredGrid.push_back(line);
	}
	this->exploredGrid[0][0].etat = VIDE;

	cout << "Nouvelle foret de taille " << forestSize << endl;
}

void Agent::parseRules(string rules) {
	fstream fs;
	fs.open(rules, fstream::in);

	string line;
	int l = -1;

	while (getline(fs, line)) {
		
		vector<string> splitedRule = split(line, " ==> ");

		if (splitedRule.size() == 2) {

			string premisses = splitedRule[0];
			string result = splitedRule[1];

			this->rules.push_back(Rule(premisses, result));
		}
	}

}

vector<string> Agent::split(const string& str, const string& delim) {
	vector<string> tokens;
	size_t prev = 0, pos = 0;
	do
	{
		pos = str.find(delim, prev);
		if (pos == string::npos) pos = str.length();
		string token = str.substr(prev, pos - prev);
		if (!token.empty()) tokens.push_back(token);
		prev = pos + delim.length();
	} while (pos < str.length() && prev < str.length());
	return tokens;
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

Pair Agent::getSafeNeighbor(vector<Pair> neighbors) {
	
	vector<Pair> safeNeighbors;

	for (Pair neighbor : neighbors) {

		int x = neighbor.first;
		int y = neighbor.second;

		Fait fait = exploredGrid[x][y];

		if (fait.etat != MONSTRE_PROBABLE && fait.etat != CREVASSE_PROBABLE && fait.etat != MONSTRE_CREVASSE_PROBABLE) {
			safeNeighbors.push_back(neighbor);
		}
	}

	if (safeNeighbors.size() == 0) return make_pair(-1, -1);
	else {
		random_device rd;
		mt19937 mt(rd());
		uniform_real_distribution<double> dist(0.0, (double)neighbors.size());
		return safeNeighbors[dist(mt)];
	}
}

Pair Agent::getMonstreNeighbor(vector<Pair> neighbors) {

	vector<Pair> monstreNeighbors;

	for (Pair neighbor : neighbors) {

		int x = neighbor.first;
		int y = neighbor.second;

		Fait fait = exploredGrid[x][y];

		if (fait.etat == MONSTRE || fait.etat == MONSTRE_PROBABLE) {
			monstreNeighbors.push_back(neighbor);
		}
	}

	if (monstreNeighbors.size() == 0) return make_pair(-1, -1);
	else {
		random_device rd;
		mt19937 mt(rd());
		uniform_real_distribution<double> dist(0.0, (double)neighbors.size());
		return monstreNeighbors[dist(mt)];
	}
}

vector<Rule> Agent::getApplicableRules(Fait fait) {

	vector<Rule> applicableRules;

	for (Rule rule : rules)
		if (rule.isApplicable(fait, exploredGrid)) applicableRules.push_back(rule);

	return applicableRules;
}

void Agent::moteurInference() {

	while (sensor->getCell(x, y) != PORTAIL) {

		vector<Pair> neighbors = getUnexploredNeighbors();

		cout << "Deduction des cases " << endl;

		// estimation de la valeur des cases adjacentes
		for (Pair neighbor : neighbors) {
			
			int x = neighbor.first;
			int y = neighbor.second;

			Fait fait = exploredGrid[x][y];

			vector<Rule> applicableRules = getApplicableRules(fait);

			for (Rule rule : applicableRules)
				fait = rule.apply(fait, exploredGrid);

			exploredGrid[x][y] = fait;
		}

		// explore une case inconnue
		Pair safeNeighbor = getSafeNeighbor(neighbors);
		if (safeNeighbor.first == -1) {
			safeNeighbor = getMonstreNeighbor(neighbors);
			if (safeNeighbor.first == -1) {
				safeNeighbor = neighbors[0];
			}
			else {
				effector->shoot(safeNeighbor.first, safeNeighbor.second);
				score -= 10;
			}
		}

		print();

		cout << "Exploration de la case " << safeNeighbor.first << " " << safeNeighbor.second << endl;

		effector->move(safeNeighbor.first, safeNeighbor.second);
		score--;

		Fait fait = exploredGrid[safeNeighbor.first][safeNeighbor.second];
		fait.etat = sensor->getCell(safeNeighbor.first, safeNeighbor.second);
		exploredGrid[safeNeighbor.first][safeNeighbor.second] = fait;

		if (fait.etat == MONSTRE || fait.etat == CREVASSE || fait.etat == MONSTRE_CREVASSE) {
			score -= forestSize * forestSize * 10;
			effector->move(0, 0);
		}
		if (fait.etat == PORTAIL) {
			score += forestSize * forestSize * 10;
			Forest* forest = new Forest(forestSize + 1);
			setNewForest(forest);
		}


		print();

	}
}

void Agent::print() {

	cout << "grille connue				foret				score : " << score << endl;;
	for (int x = 0; x < forestSize; x++) {
		for (int y = 0; y < forestSize; y++) {

			if (exploredGrid[x][y].etat == INCONNU) cout << "  ";
			else cout << " " << exploredGrid[x][y].etat ;
		}
		cout << "         ";
		for (int y = 0; y < forestSize; y++) {

			cout << " " << sensor->getCell(x, y);
		}
		cout << endl << endl;
	}
}