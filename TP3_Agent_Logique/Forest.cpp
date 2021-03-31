#include "Forest.h"

Forest::Forest(int size) {
	this->size = size;

	for (int x = 0; x < size; x++) {
		vector<Cell> line;
		for (int y = 0; y < size; y++) {
			line.push_back(vide);
		}
		this->grid.push_back(line);
	}
	placePortail();
	fillCells();
	this->grid[0][0] = agent;
}

int Forest::getSize() {
	return this->size;
}

Cell Forest::getCell(int x, int y) {
	return this->grid[x][y];
}

void Forest::setCell(int x, int y, Cell cell) {
	this->grid[x][y] = cell;
}

void Forest::print() {
	for (int x = 0; x < size; x++) {
		for (int y = 0; y < size; y++) {
			cout << " " << getCell(x, y);
		}
		cout << endl << endl;
	}
}

void Forest::fillCells() {
	random_device rd;
	mt19937 mt(rd());
	uniform_real_distribution<double> dist(1.0, 100.0);

	for (int x = 0; x < size; x++) {
		for (int y = 0; y < size; y++) {
			if (this->grid[x][y] != portail) {

				if (x != 0 && y != 0) {
					// monstre
					if (dist(mt) < 10) addMonstre(x, y);

					// crevasse
					if (dist(mt) < 10) addCrevasse(x, y);
				}
			}
		}
	}
}

void Forest::placePortail() {
	random_device rd;
	mt19937 mt(rd());
	uniform_real_distribution<double> dist(0.0, (double)size);

	int x = dist(mt);
	int y = dist(mt);

	this->grid[x][y] = portail;
}

void Forest::addOdeur(int x, int y) {
	if (this->grid[x][y] == monstre || this->grid[x][y] == crevasse) return;
	if (this->grid[x][y] == vide) this->grid[x][y] = odeur;
	if (this->grid[x][y] == portail) this->grid[x][y] = portail_odeur;
	if (this->grid[x][y] == portail_vent) this->grid[x][y] = portail_odeur_vent;
	if (this->grid[x][y] == vent) this->grid[x][y] = odeur_vent;
}

void Forest::addVent(int x, int y) {
	if (this->grid[x][y] == monstre || this->grid[x][y] == crevasse) return;
	if (this->grid[x][y] == vide) this->grid[x][y] = vent;
	if (this->grid[x][y] == portail) this->grid[x][y] = portail_vent;
	if (this->grid[x][y] == portail_odeur) this->grid[x][y] = portail_odeur_vent;
	if (this->grid[x][y] == odeur) this->grid[x][y] = odeur_vent;
}

void Forest::addMonstre(int x, int y) {
	if (this->grid[x][y] == vide) this->grid[x][y] = monstre;
	if (this->grid[x][y] == crevasse) this->grid[x][y] = monstre_crevasse;

	// diagonales
	if (x - 1 >= 0 && y - 1 >= 0) addOdeur(x - 1, y - 1);
	if (x + 1 < size && y - 1 >= 0) addOdeur(x + 1, y - 1);
	if (x - 1 >= 0 && y + 1 < size) addOdeur(x - 1, y + 1);
	if (x + 1 < size && y + 1 < size) addOdeur(x + 1, y + 1);

	// cotes
	if (x - 1 >= 0) addOdeur(x - 1, y);
	if (x + 1 < size) addOdeur(x + 1, y);
	if (y - 1 >= 0) addOdeur(x, y - 1);
	if (y + 1 < size) addOdeur(x, y + 1);
}

void Forest::addCrevasse(int x, int y) {
	if (this->grid[x][y] == vide) this->grid[x][y] = crevasse;
	if (this->grid[x][y] == monstre) this->grid[x][y] = monstre_crevasse;

	// diagonales
	if (x - 1 >= 0 && y - 1 >= 0) addVent(x - 1, y - 1);
	if (x + 1 < size && y - 1 >= 0) addVent(x + 1, y - 1);
	if (x - 1 >= 0 && y + 1 > size) addVent(x - 1, y + 1);
	if (x + 1 < size && y + 1 > size) addVent(x + 1, y + 1);

	// cotes
	if (x - 1 >= 0) addVent(x - 1, y);
	if (x + 1 < size) addVent(x + 1, y);
	if (y - 1 >= 0) addVent(x, y - 1);
	if (y + 1 < size) addVent(x, y + 1);
}

void Forest::move(int x, int y) {
	this->setCell(x, y, agent);
}

void Forest::shoot(int x, int y) {
	if (this->grid[x][y] == monstre) this->grid[x][y] = vide;
	if (this->grid[x][y] == monstre_crevasse) this->grid[x][y] = crevasse;
}