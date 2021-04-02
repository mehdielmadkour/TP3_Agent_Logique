#include "Forest.h"

Forest::Forest(int size) {
	this->size = size;

	// creation d'une grille de case vide
	for (int x = 0; x < size; x++) {
		vector<Etat> line;
		for (int y = 0; y < size; y++) {
			line.push_back(VIDE);
		}
		this->grid.push_back(line);
	}

	placePortail();
	fillCells();
	this->grid[0][0] = AGENT;
}

int Forest::getSize() {
	return this->size;
}

Etat Forest::getCell(int x, int y) {
	return this->grid[x][y];
}

void Forest::setCell(int x, int y, Etat cell) {
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

	// generateur de nombre aleatoire
	random_device rd;
	mt19937 mt(rd());
	uniform_real_distribution<double> dist(1.0, 100.0);

	// parcours les cases vide
	for (int x = 0; x < size; x++) {
		for (int y = 0; y < size; y++) {
			if (this->grid[x][y] != PORTAIL) {

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

	// generateur de nombre aleatoire
	random_device rd;
	mt19937 mt(rd());
	uniform_real_distribution<double> dist(1.0, (double)size);

	// coordonnees aleatoire du portail
	int x = dist(mt);
	int y = dist(mt);

	this->grid[x][y] = PORTAIL;
}

void Forest::addOdeur(int x, int y) {

	// ajoute monstre_proche a l'etat precedant

	if (this->grid[x][y] == MONSTRE || this->grid[x][y] == CREVASSE) return;
	if (this->grid[x][y] == VIDE) this->grid[x][y] = MONSTRE_PROCHE;
	if (this->grid[x][y] == CREVASSE_PROCHE) this->grid[x][y] = MONSTRE_CREVASSE_PROCHE;
}

void Forest::addVent(int x, int y) {

	// ajoute crevasse_proche a l'etat precedant

	if (this->grid[x][y] == MONSTRE || this->grid[x][y] == CREVASSE) return;
	if (this->grid[x][y] == VIDE) this->grid[x][y] = CREVASSE_PROCHE;
	if (this->grid[x][y] == MONSTRE_PROCHE) this->grid[x][y] = MONSTRE_CREVASSE_PROCHE;
}

void Forest::addMonstre(int x, int y) {

	// ajoute monstre a l'etat precedant

	if (this->grid[x][y] == CREVASSE) this->grid[x][y] = MONSTRE_CREVASSE;
	else this->grid[x][y] = MONSTRE;

	// marque les cases adjacentes
	if (x - 1 >= 0) addOdeur(x - 1, y);
	if (x + 1 < size) addOdeur(x + 1, y);
	if (y - 1 >= 0) addOdeur(x, y - 1);
	if (y + 1 < size) addOdeur(x, y + 1);
}

void Forest::addCrevasse(int x, int y) {

	// ajoute crevasse a l'etat precedant

	if (this->grid[x][y] == MONSTRE) this->grid[x][y] = MONSTRE_CREVASSE;
	else this->grid[x][y] = CREVASSE;

	// marque les cases adjacentes
	if (x - 1 >= 0) addVent(x - 1, y);
	if (x + 1 < size) addVent(x + 1, y);
	if (y - 1 >= 0) addVent(x, y - 1);
	if (y + 1 < size) addVent(x, y + 1);
}

void Forest::move(int x, int y) {
	//this->setCell(x, y, AGENT);
}

void Forest::shoot(int x, int y) {
	if (this->grid[x][y] == MONSTRE) this->grid[x][y] = VIDE;
	if (this->grid[x][y] == MONSTRE_CREVASSE) this->grid[x][y] = CREVASSE;
}