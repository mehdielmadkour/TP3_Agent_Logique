#pragma once
#include <vector>
#include <iostream>
#include <random>

using namespace std;

enum Etat {
	VIDE, AGENT, INCONNU,
	MONSTRE, MONSTRE_PROBABLE,
	CREVASSE, CREVASSE_PROBABLE,
	MONSTRE_CREVASSE, MONSTRE_CREVASSE_PROBABLE,
	MONSTRE_PROCHE, CREVASSE_PROCHE, MONSTRE_CREVASSE_PROCHE,
	PORTAIL
};

class Forest {
private:
	int size;
	vector<vector<Etat>> grid;
	void setCell(int x, int y, Etat cell);
	void fillCells();
	void placePortail();
	void addOdeur(int x, int y);
	void addVent(int x, int y);
	void addMonstre(int x, int y);
	void addCrevasse(int x, int y);

public:
	Forest(int size);
	int getSize();
	Etat getCell(int x, int y);
	void print();
	void move(int x, int y);
	void shoot(int x, int y);
};