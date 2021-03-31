#include "Effector.h"

Effector::Effector(Forest* forest) {
	this->forest = forest;
}

void Effector::move(int x, int y) {
	if (forest->getCell(x, y) == MONSTRE || forest->getCell(x, y) == CREVASSE || forest->getCell(x, y) == MONSTRE_CREVASSE)
		cout << "died" << endl;
	else if (forest->getCell(x, y) == PORTAIL)
		cout << "leave" << endl;
	else
		forest->move(x, y);
}

void Effector::shoot(int x, int y) {
	forest->shoot(x, y);
}