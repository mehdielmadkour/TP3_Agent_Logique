#pragma once
#include "Forest.h"

using namespace std;
class Sensor {
private:
	Forest* forest;

public:
	Sensor(Forest* foret);
	Cell getCell(int x, int y);
};