#include <iostream>
#include <fstream>
#include <sstream>
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
        string sportName, divName, desc, optDesc, unit;

        getline(ss, sportName, ',');
        getline(ss, divName, ',');
        getline(ss, desc, ',');
        getline(ss, optDesc, ',');
        getline(ss, unit, ',');

        Sport* sportPtr = nullptr;
        for (auto& s : *sports) {
            if (s.name == sportName) {
                sportPtr = &s;
                break;
            }
        }

        if (!sportPtr) {
            Sport newSport;
            newSport.name = sportName;
            newSport.unit = unit;
            sports->push_back(newSport);
            sportPtr = &sports->back();
        }

        Division d(0, 0, divName, desc, optDesc);
        sportPtr->divisionArr.push_back(d);
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
