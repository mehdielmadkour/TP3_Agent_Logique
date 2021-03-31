#pragma once
#include "Forest.h"

using namespace std;
class Effector {
private:
	Forest* forest;

public:
	Effector(Forest* forest);
	void move(int x, int y);
	void shoot(int x, int y);
};