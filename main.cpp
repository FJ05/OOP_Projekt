#include <fstream> 
#include <iostream>
#include <vector>
#include "assets.h"
#include "importHandler.h"
#include "schedulecalculator.h"

using namespace std;

int main()
{
    vector<Competitor> competitors;
    vector<Score> scores;
    vector<Sport> sports;

    ImportSystem importer;
    importer.ImportValues(&competitors, &scores, &sports);

    // Test: skriv ut resultat
    cout << "Antal tävlande: " << competitors.size() << endl;
    for (int i = 0; i < sports.size(); i++) {
        const Sport& sport = sports[i];

        cout << "Sport: " << sport.name << " ( unit: " << sport.unit << " ), Arena size: " << sport.arenaSize << endl;
        cout << "  Antal tävlande: " << sport.competitorArr.size() << endl;
        cout << "  Divisioner:" << endl;

        for (int j = 0; j < sport.divisionArr.size(); j++) {
            const Division& d = sport.divisionArr[j];
            cout << "    - " << d.name << " (" << d.desc << "), Optional: " << d.optDesc << endl;
        }
    }




    // Efter import av data
    ScheduleCalculator scheduler;
    auto schedule = scheduler.GenerateSchedule(sports);

    // Skriv ut schemat
    std::ofstream scheduleFile("schedule.csv");
    scheduleFile << "Station;Start;Slut;Gren;Division;Deltagare\n";
    for (const auto& event : schedule) {
        scheduleFile << event.station << ";"
                    << event.startTime << ";"
                    << event.endTime << ";"
                    << event.sport.name << ";"
                    << event.division.desc << " "
                    << event.division.name << ";";
        
        for (size_t i = 0; i < event.competitors.size(); ++i) {
            if (i > 0) scheduleFile << ",";
            scheduleFile << event.competitors[i].name << " "
                        << event.competitors[i].surname;
        }
        scheduleFile << "\n";
    }
    scheduleFile.close();



    return 0;
}