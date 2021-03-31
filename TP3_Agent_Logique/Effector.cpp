#include "Effector.h"

Effector::Effector(Forest* forest) {
	this->forest = forest;
}

void Effector::move(int x, int y) {
	if (forest->getCell(x, y) == monstre || forest->getCell(x, y) == crevasse)
		cout << "died" << endl;
	else if (forest->getCell(x, y) == portail)
		cout << "leave" << endl;
	else
		forest->move(x, y);
}

void Effector::shoot(int x, int y) {
	forest->shoot(x, y);
}