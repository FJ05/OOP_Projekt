#include <iostream>
#include <fstream>
#include <vector>
#include "assets.h"
#include "importHandler.h"
#include "schedulecalculator.h"

int main() {
    std::vector<Competitor> competitors;
    std::vector<Score> scores;
    std::vector<Sport> sports;

    ImportSystem importer;
    importer.ImportValues(&competitors, &scores, &sports);

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