#pragma once

enum Etat { MONSTRE, MONSTRE_PROCHE, MONSTRE_PROBABLE, CREVASSE, CREVASSE_PROCHE, CREVASSE_PROBABLE, VIDE, PORTAIL, INCONNU };
class Fait {
public:
	Fait(int x, int y);
	int x, y;
	Etat etat;
};