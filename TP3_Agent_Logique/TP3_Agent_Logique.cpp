// TP3_Agent_Logique.cpp : définit le point d'entrée de l'application.
//

#include "TP3_Agent_Logique.h"
#include "Forest.h"
#include "Rule.h"
#include "Agent.h"

using namespace std;

int main()
{

	string rulesFile;

	cout << "fichier regle : ";
	cin >> rulesFile;

	Forest forest = Forest(3);

	Agent agent = Agent(&forest, rulesFile);

	return 0;
}
