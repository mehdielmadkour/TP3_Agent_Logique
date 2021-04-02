#include "Agent.h"

Agent::Agent(Forest* forest, string rules) {

	score = 0;
	parseRules(rules);
	setNewForest(forest);
	moteurInference();
}

void Agent::setNewForest(Forest* forest) {

	forestSize = forest->getSize();
	this->sensor = new Sensor(forest);
	this->effector = new Effector(forest);

	exploredGrid.clear();

	// rempli la grille des faits avec des cases inconnues
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

	// parcours le fichier des regles
	while (getline(fs, line)) {
		
		// separation des conditions et du resultat
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
		// position du delimiteur
		pos = str.find(delim, prev);
		if (pos == string::npos) pos = str.length();

		// texte compris entre le delimiteur courant et le delimiteur precedant
		string token = str.substr(prev, pos - prev);
		if (!token.empty()) tokens.push_back(token);

		// mise a jour de la position du delimiteur precedant
		prev = pos + delim.length();

	} while (pos < str.length() && prev < str.length());

	return tokens;
}

vector<Pair> Agent::getUnexploredNeighbors() {

	vector<Pair> unexploredNeighbors;

	// parcours les case inconnues
	for (int x = 0; x < forestSize; x++)
		for (int y = 0; y < forestSize; y++)
			if (exploredGrid[x][y].etat == INCONNU) {

				bool neighbor = false;

				// teste si la case est voisine à une case connue
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

	// parcours la liste des voisins
	for (Pair neighbor : neighbors) {

		int x = neighbor.first;
		int y = neighbor.second;

		Fait fait = exploredGrid[x][y];

		// selectionne les cases ne comportant ni monstre ni crevasse
		if (fait.etat != MONSTRE_PROBABLE && fait.etat != CREVASSE_PROBABLE && fait.etat != MONSTRE_CREVASSE_PROBABLE) {
			safeNeighbors.push_back(neighbor);
		}
	}

	if (safeNeighbors.size() == 0) return make_pair(-1, -1);
	else {

		// selectionne aleatoirement une des cases
		random_device rd;
		mt19937 mt(rd());
		uniform_real_distribution<double> dist(0.0, (double)neighbors.size());
		return safeNeighbors[dist(mt)];
	}
}

Pair Agent::getMonstreNeighbor(vector<Pair> neighbors) {

	vector<Pair> monstreNeighbors;

	// parcours la liste des voisins
	for (Pair neighbor : neighbors) {

		int x = neighbor.first;
		int y = neighbor.second;

		Fait fait = exploredGrid[x][y];

		// selectionne les cases ne comportant risquant de comporter un monstre
		if (fait.etat == MONSTRE || fait.etat == MONSTRE_PROBABLE) {
			monstreNeighbors.push_back(neighbor);
		}
	}

	if (monstreNeighbors.size() == 0) return make_pair(-1, -1);
	else {

		// selectionne aleatoirement une des cases
		random_device rd;
		mt19937 mt(rd());
		uniform_real_distribution<double> dist(0.0, (double)neighbors.size());
		return monstreNeighbors[dist(mt)];
	}
}

vector<Rule> Agent::getApplicableRules(Fait fait) {

	vector<Rule> applicableRules;

	// parcours la liste des regles
	for (Rule rule : rules)
		// selectionne les regles applicables
		if (rule.isApplicable(fait, exploredGrid)) applicableRules.push_back(rule);

	return applicableRules;
}

void Agent::moteurInference() {

	// tant que le but n'est pas atteint
	while (sensor->getCell(x, y) != PORTAIL) {

		vector<Pair> neighbors = getUnexploredNeighbors();

		cout << "Deduction des cases " << endl;

		// estimation de la valeur des cases adjacentes
		for (Pair neighbor : neighbors) {
			
			int x = neighbor.first;
			int y = neighbor.second;

			Fait fait = exploredGrid[x][y];

			vector<Rule> applicableRules = getApplicableRules(fait);

			// applique les regles
			for (Rule rule : applicableRules)
				fait = rule.apply(fait, exploredGrid);

			exploredGrid[x][y] = fait;
		}

		// explore une case inconnue
		Pair safeNeighbor = getSafeNeighbor(neighbors);
		if (safeNeighbor.first == -1) {

			// selectionne une case a risque si aucune case n'est sans danger
			safeNeighbor = getMonstreNeighbor(neighbors);
			if (safeNeighbor.first == -1) {

				// case par défaut si les cases adjacentes sont a risque mais ne comportent aucun monstre
				safeNeighbor = neighbors[0];
			}
			else {

				// tire sur une case comportant peut être un monstre si aucune case n'est sans danger
				effector->shoot(safeNeighbor.first, safeNeighbor.second);
				score -= 10;
			}
		}

		print();

		cout << "Exploration de la case " << safeNeighbor.first << " " << safeNeighbor.second << endl;

		// deplacement
		effector->move(safeNeighbor.first, safeNeighbor.second);
		score--;

		// explore une nouvelle case
		Fait fait = exploredGrid[safeNeighbor.first][safeNeighbor.second];
		fait.etat = sensor->getCell(safeNeighbor.first, safeNeighbor.second);
		exploredGrid[safeNeighbor.first][safeNeighbor.second] = fait;

		// case dangereuse
		if (fait.etat == MONSTRE || fait.etat == CREVASSE || fait.etat == MONSTRE_CREVASSE) {
			score -= forestSize * forestSize * 10;
			effector->move(0, 0);
		}

		// but atteint
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