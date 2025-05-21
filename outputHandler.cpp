#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include "assets.h"
using namespace std;

class OutputSystem 
{
public:
    void SportExport(list<Sport> sportList){
        ofstream file("DB/sportIndex.csv");
        if (!file.is_open()) {
                cerr << "Failed to open file sportIndex.csv for writing. Check if the file exists in the DB folder\n";
            return;
        }
        for (const auto& sport : sportList) {
            //Enter Sport Values to "DB/sportIndex.csv"
            file << sport.name << ",";
            file << sport.arenaSize << ",";
            file << sport.unit << ",";

            for (const auto& div : sport.divisionArr){
                file << div.ageFrom << ",";
                file << div.ageTo << ",";
                file << div.name << ",";
                file << div.desc << ",";
                file << div.optDesc << ",";
            }
        }
        file.close();
    }
    void CompExport(list<Sport> sportList){
        ofstream file("DB/compIndex.csv");
        if (!file.is_open()) {
                cerr << "Failed to open file compIndex.csv for writing. Check if the file exists in the DB folder\n";
            return;
        }
        for (const auto& sport : sportList){
            for (const auto& comp : sport.competitorArr){
                file << comp.name << ",";
                file << comp.surname << ",";
                file << comp.age << ",";
                file << comp.club << ",";

                // Add scores
            }
        }
    }
};