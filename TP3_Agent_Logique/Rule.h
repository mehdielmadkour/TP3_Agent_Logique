#pragma once
#include "Forest.h"
#include <string>
#include "Fait.h"

enum Direction { POSITION, HAUT, BAS, GAUCHE, DROITE, HAUT_GAUCHE, HAUT_DROITE, BAS_GAUCHE, BAS_DROITE };
enum Operation { ET, OU, ET_NON };
class Rule {
private:
	Etat resultat;
	vector<Direction> premisses;
	vector<pair<Direction, Etat>> conditions;
	vector<Operation> operations;
	vector<string> split(const string& str, const string& delim);
public:
	Rule(string premisses, string resultat);
	bool isApplicable(Fait fait, vector<vector<Fait>> exploredGrid);
	Fait apply(Fait fait, vector<vector<Fait>> exploredGrid);
};