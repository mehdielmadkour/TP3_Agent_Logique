#include "Rule.h"

Rule::Rule(string premisses, string resultat) {

    if (resultat == "PORTAIL") this->resultat = PORTAIL;
    if (resultat == "VIDE") this->resultat = VIDE;
    if (resultat == "INCONNU") this->resultat = INCONNU;
    if (resultat == "MONSTRE") this->resultat = MONSTRE;
    if (resultat == "MONSTRE_PROCHE") this->resultat = MONSTRE_PROCHE;
    if (resultat == "MONSTRE_PROBABLE") this->resultat = MONSTRE_PROBABLE;
    if (resultat == "CREVASSE") this->resultat = CREVASSE;
    if (resultat == "CREVASSE_PROCHE") this->resultat = CREVASSE_PROCHE;
    if (resultat == "CREVASSE_PROBABLE") this->resultat = CREVASSE_PROBABLE;

    // separation des premisses
    string delimiter = " ";
    vector<string> regle = split(premisses, " ");

    bool isPremisse = true;

    for (string str : regle) {

        if (isPremisse) {

            // separation des directions et des etats
            vector<string> splitedConditions = split(str, "=");

            Direction direction;
            if (splitedConditions[0] == "HAUT") direction = HAUT;
            if (splitedConditions[0] == "BAS") direction = BAS;
            if (splitedConditions[0] == "GAUCHE") direction = GAUCHE;
            if (splitedConditions[0] == "DROITE") direction = DROITE;
            if (splitedConditions[0] == "HAUT_GAUCHE") direction = HAUT_GAUCHE;
            if (splitedConditions[0] == "HAUT_DROITE") direction = HAUT_DROITE;
            if (splitedConditions[0] == "BAS_GAUCHE") direction = BAS_GAUCHE;
            if (splitedConditions[0] == "BAS_DROITE") direction = BAS_DROITE;
            if (splitedConditions[0] == "POSITION") direction = POSITION;

            Etat etat;
            if (splitedConditions[1] == "PORTAIL") etat = PORTAIL;
            if (splitedConditions[1] == "VIDE") etat = VIDE;
            if (splitedConditions[1] == "INCONNU") etat = INCONNU;
            if (splitedConditions[1] == "MONSTRE") etat = MONSTRE;
            if (splitedConditions[1] == "MONSTRE_PROCHE") etat = MONSTRE_PROCHE;
            if (splitedConditions[1] == "MONSTRE_PROBABLE") etat = MONSTRE_PROBABLE;
            if (splitedConditions[1] == "CREVASSE") etat = CREVASSE;
            if (splitedConditions[1] == "CREVASSE_PROCHE") etat = CREVASSE_PROCHE;
            if (splitedConditions[1] == "CREVASSE_PROBABLE") etat = CREVASSE_PROBABLE;

            this->premisses.push_back(direction);
            conditions.push_back(make_pair(direction, etat));
        }
        else {
            if (str == "OU") operations.push_back(OU);
            if (str == "ET") operations.push_back(ET);
            if (str == "ET_NON") operations.push_back(ET_NON);
        }

        isPremisse = !isPremisse;
    }
}

vector<string> Agent::split(const string& str, const string& delim) {

    vector<string> tokens;
    size_t prev = 0, pos = 0;

    do
    {
        // position du delimiteur
        pos = str.find(delim, prev);
        if (pos == string::npos) pos = str.length();

        // texte compris entre le delimiteur courant et le delimiteur precedant
        string token = str.substr(prev, pos - prev);
        if (!token.empty()) tokens.push_back(token);

        // mise a jour de la position du delimiteur precedant
        prev = pos + delim.length();

    } while (pos < str.length() && prev < str.length());

    return tokens;
}

bool Rule::isApplicable(Fait fait, vector<vector<Fait>> exploredGrid) {

    int x = fait.x;
    int y = fait.y;

    vector<bool> results;

    // parcours la liste des directions nécessaire pour appliquer la regle
    for (Direction d : this->premisses) {

        bool applicable = false;

        // teste si les etats des cases sont connus
        if (d == HAUT) {
            if (x - 1 >= 0) {
                if (exploredGrid[x - 1][y].etat != INCONNU) applicable = true;
            }
        }
        if (d == BAS) {
            if (x + 1 < exploredGrid.size()) {
                if (exploredGrid[x + 1][y].etat != INCONNU) applicable = true;
            }
        }
        if (d == GAUCHE) {
            if (y - 1 >= 0) {
                if (exploredGrid[x][y - 1].etat != INCONNU) applicable = true;
            }
        }
        if (d == DROITE) {
            if (y + 1 < exploredGrid.size()) {
                if (exploredGrid[x][y + 1].etat != INCONNU) applicable = true;
            }
        }
        if (d == HAUT_GAUCHE) {
            if (x - 1 >= 0 && y - 1 >= 0) {
                if (exploredGrid[x - 1][y - 1].etat != INCONNU) applicable = true;
            }
        }
        if (d == HAUT_DROITE) {
            if (x - 1 >= 0 && y + 1 < exploredGrid.size()) {
                if (exploredGrid[x - 1][y + 1].etat != INCONNU) applicable = true;
            }
        }
        if (d == BAS_GAUCHE) {
            if (x + 1 < exploredGrid.size() && y - 1 >= 0) {
                if (exploredGrid[x + 1][y - 1].etat != INCONNU) applicable = true;
            }
        }
        if (d == BAS_DROITE) {
            if (x + 1 < exploredGrid.size() && y + 1 < exploredGrid.size()) {
                if (exploredGrid[x + 1][y + 1].etat != INCONNU) applicable = true;
            }
        }
        if (d == POSITION) {
            applicable = true;
        }

        results.push_back(applicable);
    }

    bool result = results[0];

    // applique les operateurs logique
    for (int i = 0; i < operations.size(); i++) {
        if (operations[i] == ET) result = result && results[i + 1];
        if (operations[i] == OU) result = result || results[i + 1];
        if (operations[i] == ET_NON) result = result || !results[i + 1];
    }

    return result;
}

Fait Rule::apply(Fait fait, vector<vector<Fait>> exploredGrid) {

    vector<bool> results;
    int x = fait.x;
    int y = fait.y;

    // parcours la liste des conditions
    for (pair<Direction, Etat> condition : conditions) {

        Direction d = condition.first;
        Etat e = condition.second;

        bool result = false;

        // teste si les etats des cases sont valide
        if (d == HAUT) {
            if (x - 1 >= 0) {
                if (exploredGrid[x - 1][y].etat == e) result = true;
            }
        }
        if (d == BAS) {
            if (x + 1 < exploredGrid.size()) {
                if (exploredGrid[x + 1][y].etat == e) result = true;
            }
        }
        if (d == GAUCHE) {
            if (y - 1 >= 0) {
                if (exploredGrid[x][y - 1].etat == e) result = true;
            }
        }
        if (d == DROITE) {
            if (y + 1 < exploredGrid.size()) {
                if (exploredGrid[x][y + 1].etat == e) result = true;
            }
        }
        if (d == HAUT_GAUCHE) {
            if (x - 1 >= 0 && y - 1 >= 0) {
                if (exploredGrid[x - 1][y - 1].etat == e) result = true;
            }
        }
        if (d == HAUT_DROITE) {
            if (x - 1 >= 0 && y + 1 < exploredGrid.size()) {
                if (exploredGrid[x - 1][y + 1].etat == e) result = true;
            }
        }
        if (d == BAS_GAUCHE) {
            if (x + 1 < exploredGrid.size() && y - 1 >= 0) {
                if (exploredGrid[x + 1][y - 1].etat == e) result = true;
            }
        }
        if (d == BAS_DROITE) {
            if (x + 1 < exploredGrid.size() && y + 1 < exploredGrid.size()) {
                if (exploredGrid[x + 1][y + 1].etat == e) result = true;
            }
        }
        if (d == POSITION) {
            if (exploredGrid[x][y].etat == e) result = true;
        }

        results.push_back(result);
    }

    bool result = results[0];

    // applique les operateurs logique
    for (int i = 0; i < operations.size(); i++) {
        if (operations[i] == ET) result = result && results[i + 1];
        if (operations[i] == OU) result = result || results[i + 1];
        if (operations[i] == ET_NON) result = result || !results[i + 1];
    }

    // applique le resultat si la regle est valide
    if (result) fait.etat = resultat;

    return fait;
}