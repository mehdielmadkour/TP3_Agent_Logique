#include "Sensor.h"

Sensor::Sensor(Forest* forest) {
	this->forest = forest;
}

Etat Sensor::getCell(int x, int y) {
	return forest->getCell(x, y);
}