#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "importHandler.h"

using namespace std;

void ImportSystem::ImportValues(vector<Competitor>* competitors,
                                vector<Score>* scores,
                                vector<Sport>* sports)
{
    cout << "start importvalues" << endl;

    ifstream dbFile("DB/compIndex.csv");
    ifstream dbSportFile("DB/sportIndex.csv");

    string line, lineSport;

    // LÄS IN SPORTDATA
    getline(dbSportFile, lineSport); // Skippa header
    while (getline(dbSportFile, lineSport)) {
        stringstream ss(lineSport);
        string cell;
        vector<string> tokens;

        while (getline(ss, cell, ',')) {
            tokens.push_back(cell);
        }

        if (tokens.size() < 5) continue;

        string sportName = tokens[0];
        string arenaSizeStr = tokens[1];
        string unit = tokens[2];
        int ageFrom = stoi(tokens[3]);
        int ageTo = stoi(tokens[4]);

        Sport* sportPtr = nullptr;
        for (auto& s : *sports) {
            if (s.name == sportName && s.unit == unit) {
                sportPtr = &s;
                break;
            }
        }

        if (!sportPtr) {
            Sport newSport;
            newSport.name = sportName;
            newSport.unit = unit;
            newSport.arenaSize = stoi(arenaSizeStr);
            sports->push_back(newSport);
            sportPtr = &sports->back();
        }

        // Divisioner börjar från index 5, varje 3 värde
        for (size_t i = 5; i + 2 < tokens.size(); i += 3) {
            string name = tokens[i];
            string desc = tokens[i + 1];
            string optDesc = tokens[i + 2];
            Division d(ageFrom, ageTo, name, desc, optDesc);
            sportPtr->divisionArr.push_back(d);
        }
    }

    // LÄS IN TÄVLANDE
    getline(dbFile, line); // Skippa header
    while (getline(dbFile, line)) {
        stringstream ss(line);
        string substr;
        vector<string> v;
        Competitor c;

        while (getline(ss, substr, ',')) {
            v.push_back(substr);
        }

        c.name = v[0];
        c.surname = v[1];
        c.age = stoi(v[2]);
        c.sex = v[3][0];
        c.club = v[4];

        for (size_t i = 5; i < v.size(); i++) {
            Score score;
            score.scoreStr = v[i];

            if (i - 5 < sports->size()) {
                score.sport = &(*sports)[i - 5];
                if (!score.sport->divisionArr.empty()) {
                    score.division = score.sport->divisionArr[0];
                }

                score.sport->competitorArr.push_back(c);
            }

            c.scoreArr.push_back(score);
            scores->push_back(score);
        }

        competitors->push_back(c);
    }

    dbFile.close();
    dbSportFile.close();
}