#include <iostream>
#include <fstream>
#include <vector>
#include "assets.h"
#include "importHandler.h"
#include "schedulecalculator.h"
using namespace std;
/**
 * @brief 
 * 
 * @return int 
 */
int main() {
    vector<Competitor> competitors;
    vector<Score> scores;
    vector<Sport> sports;

    ImportSystem importer;
    importer.ImportValues(&competitors, &scores, &sports);

    // Test: skriv ut resultat
    cout << "Starta print" << endl;
    cout << "Antal tävlande: " << competitors.size() << endl;
    cout << "Antal sporter: " << sports.size() << endl;
    for (int i = 0; i < sports.size(); i++) {
        const Sport& sport = sports[i];

        cout << "Sport: " << sport.name << " ( unit: " << sport.unit << " ), Arena size: " << sport.arenaSize << endl;
        cout << "  Antal tävlande: " << sport.competitorArr.size() << endl;
        cout << "Antal divisioner " << sports[i].divisionArr.size() << endl;
        cout << "  Divisioner:" << endl;

        for (int j = 0; j < sport.divisionArr.size(); j++) {
            const Division& d = sport.divisionArr[j];
            cout << "    - " << d.name << " (" << d.desc << "), Optional: " << d.optDesc  << "Gender :" << d.gender << endl;
        }
    }




    // Efter import av data
    ScheduleCalculator scheduler;
    auto schedule = scheduler.GenerateSchedule(sports);

    std::ofstream scheduleFile("schedule.csv");
    scheduleFile << "Station;Dicipline;M/F;Age;Total time;Time;Deltagare\n";
    
    for(const auto& event : schedule) {
        scheduleFile << event.station << ";"
                    << event.discipline << ";"
                    << event.gender << ";"
                    << event.ageGroup << ";"
                    << event.totalTime << ";"
                    << event.timeSlot << ";";
        
        for(size_t i = 0; i < event.competitors.size(); ++i) {
            if(i > 0) scheduleFile << ",";
            scheduleFile << event.competitors[i].name << " "
                        << event.competitors[i].surname;
        }
        scheduleFile << "\n";
    }
    
    return 0;
}