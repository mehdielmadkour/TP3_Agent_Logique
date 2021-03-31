#pragma once
#include <vector>
#include <iostream>
#include <random>

using namespace std;

enum Cell {
	vide, agent, inconnu,
	monstre, crevasse, monstre_crevasse,
	odeur, vent, odeur_vent,
	portail, portail_odeur, portail_vent, portail_odeur_vent
};

class Forest {
private:
	int size;
	vector<vector<Cell>> grid;
	void setCell(int x, int y, Cell cell);
	void fillCells();
	void placePortail();
	void addOdeur(int x, int y);
	void addVent(int x, int y);
	void addMonstre(int x, int y);
	void addCrevasse(int x, int y);

public:
	Forest(int size);
	int getSize();
	Cell getCell(int x, int y);
	void print();
	void move(int x, int y);
	void shoot(int x, int y);
};