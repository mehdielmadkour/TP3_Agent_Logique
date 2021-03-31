// TP3_Agent_Logique.cpp : définit le point d'entrée de l'application.
//

#include "TP3_Agent_Logique.h"
#include "Forest.h"
#include "Rule.h"
#include "Agent.h"

using namespace std;

int main()
{
	Rule R1 = Rule("HAUT=MONSTRE_PROCHE OU BAS=MONSTRE_PROCHE OU GAUCHE=MONSTRE_PROCHE OU DROITE=MONSTRE_PROCHE OU HAUT_GAUCHE=MONSTRE_PROCHE OU HAUT_DROITE=MONSTRE_PROCHE OU BAS_GAUCHE=MONSTRE_PROCHE OU BAS_DROITE=MONSTRE_PROCHE", "MONSTRE_PROBABLE");
	Rule R2 = Rule("HAUT=MONSTRE_PROCHE ET BAS=MONSTRE_PROCHE ET GAUCHE=MONSTRE_PROCHE ET DROITE=MONSTRE_PROCHE ET HAUT_GAUCHE=MONSTRE_PROCHE ET HAUT_DROITE=MONSTRE_PROCHE ET BAS_GAUCHE=MONSTRE_PROCHE ET BAS_DROITE=MONSTRE_PROCHE", "MONSTRE");

	vector<Rule> rules;
	rules.push_back(R1);
	rules.push_back(R2);

	Forest forest = Forest(3);
	forest.print();

	Agent agent = Agent(&forest, rules);

	return 0;
}
