/*
This file is used for exporting competitors to the compIndex.csv
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include "assets.h"
using namespace std;

/**
 * @brief 
 * 
 */
class OutputSystem 
{
public:
void CompExport(list<Sport> sportList) {
    ofstream file("DB/compIndex.csv");
    if (!file.is_open()) {
        cerr << "Failed to open file compIndex.csv for writing. Check if the file exists in the DB folder\n";
        return;
    }

    // Step 1: Gather all unique competitors and unique event names
    vector<Competitor> uniqueCompetitors;
    vector<string> eventNames;

    for (const auto& sport : sportList) {
        for (const auto& comp : sport.competitorArr) {
            // Check for uniqueness
            bool exists = false;
            for (const auto& uc : uniqueCompetitors) {
                if (uc.name == comp.name && uc.surname == comp.surname && uc.age == comp.age && uc.club == comp.club)
                    exists = true;
            }
            if (!exists)
                uniqueCompetitors.push_back(comp);

            // Get unique event names from scoreArr
            for (const auto& score : comp.scoreArr) {
                if (score.sport) {
                    std::string eventLabel = score.division.desc + " " + score.division.name;
                    bool found = false;
                    for (const auto& existing : eventNames) {
                        if (existing == eventLabel) {
                            found = true;
                            break;
                        }
                    }
                    if (!found)
                        eventNames.push_back(eventLabel);
                }
            }
        }
    }

    // Step 2: Write header
    file << "Name,Surname,Age,Sex,Club";
    for (const auto& event : eventNames) {
        file << "," << event;
    }
    file << "\n";

    // Step 3: Write data
    for (const auto& comp : uniqueCompetitors) {
        file << comp.name << "," << comp.surname << "," << comp.age << "," << comp.sex << "," << comp.club;

        for (const auto& event : eventNames) {
            bool hasScore = false;
            for (const auto& score : comp.scoreArr) {
                if (score.sport) {
                    std::string eventLabel = score.division.desc + " " + score.division.name;
                    if (eventLabel == event) {
                        file << "," << score.scoreStr;
                        hasScore = true;
                        break;
                    }
                }
            }
            if (!hasScore) {
                file << ",";
            }
        }
        file << "\n";
    }

    file.close();
}

};