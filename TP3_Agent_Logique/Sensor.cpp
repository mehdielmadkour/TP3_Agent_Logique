#include "Sensor.h"

Sensor::Sensor(Forest* forest) {
	this->forest = forest;
}

Cell Sensor::getCell(int x, int y) {
	return forest->getCell(x, y);
}