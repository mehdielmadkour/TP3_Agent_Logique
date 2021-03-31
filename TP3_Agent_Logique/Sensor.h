#pragma once
#include "Forest.h"

using namespace std;
class Sensor {
private:
	Forest* forest;

public:
	Sensor(Forest* foret);
	Etat getCell(int x, int y);
};