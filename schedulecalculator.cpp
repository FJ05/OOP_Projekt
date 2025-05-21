#include "schedulecalculator.h"
#include <map>
#include <algorithm>

std::vector<ScheduledEvent> ScheduleCalculator::GenerateSchedule(const std::vector<Sport>& sports) {
    std::vector<ScheduledEvent> schedule;
    std::map<std::string, std::string> stationAvailability;
    std::map<std::string, std::string> competitorAvailability;

    // Initialize stations
    const std::vector<std::string> allStations = {
        "Running Circle (6 tracks)",
        "Sprint Line (8 tracks)",
        "Long/Triple Jump – I",
        "Long/Triple Jump – II",
        "High Jump – I",
        "High Jump – II",
        "Pole Vault",
        "Shot Throwing – I",
        "Shot Throwing – II"
    };
    
    for (const auto& station : allStations) {
        stationAvailability[station] = "09:00";
    }

    for (const auto& sport : sports) {
        for (const auto& division : sport.divisionArr) {
            // Hämta aktuella tävlande
            std::vector<Competitor> competitors;
            for (const auto& comp : sport.competitorArr) {
                if (comp.age >= division.ageFrom && comp.age <= division.ageTo) {
                    competitors.push_back(comp);
                }
            }

            // Hämta stationer för denna gren
            auto stations = GetStationsForEvent(sport.name, division);
            if (stations.empty()) continue;

            // Beräkna tävlingstid
            int duration = CalculateDuration(sport, division, competitors.size());
            int maxCompetitors = 8;  // Standardvärde
            
            if (stations[0].find("Running Circle") != std::string::npos) maxCompetitors = 6;
            else if (stations[0].find("Long/Triple") != std::string::npos) maxCompetitors = 1;

            // Dela upp i heat
            int numHeats = (competitors.size() + maxCompetitors - 1) / maxCompetitors;
            
            for (int i = 0; i < numHeats; ++i) {
                // Välj station med tidigast tillgänglig tid
                std::string chosenStation;
                std::string earliestTime = "23:59";
                for (const auto& station : stations) {
                    if (CompareTime(stationAvailability[station], earliestTime) < 0) {
                        earliestTime = stationAvailability[station];
                        chosenStation = station;
                    }
                }

                // Skapa tidsintervall
                std::string startTime = earliestTime;
                std::string endTime = AddTime(startTime, duration);

                // Skapa heat
                auto startIt = competitors.begin() + i * maxCompetitors;
                auto endIt = (i+1)*maxCompetitors > competitors.size() ? 
                    competitors.end() : startIt + maxCompetitors;
                std::vector<Competitor> heatCompetitors(startIt, endIt);

                // Skapa event
                ScheduledEvent event{
                    chosenStation,
                    startTime,
                    endTime,
                    sport,
                    division,
                    heatCompetitors
                };

                // Uppdatera tillgängliga tider
                stationAvailability[chosenStation] = endTime;
                for (const auto& comp : heatCompetitors) {
                    std::string key = comp.name + "_" + comp.surname;
                    if (CompareTime(competitorAvailability[key], endTime) < 0) {
                        competitorAvailability[key] = endTime;
                    }
                }

                schedule.push_back(event);
            }
        }
    }

    return schedule;
}

// Hjälpfunktioner här...


// Lägg till tidsberäkningsfunktioner
std::string ScheduleCalculator::AddTime(const std::string& currentTime, int minutes) const {
    int hours = std::stoi(currentTime.substr(0, 2));
    int mins = std::stoi(currentTime.substr(3, 2));
    
    mins += minutes;
    hours += mins / 60;
    mins %= 60;
    
    char buffer[6];
    snprintf(buffer, sizeof(buffer), "%02d:%02d", hours, mins);
    return std::string(buffer);
}

int ScheduleCalculator::CompareTime(const std::string& t1, const std::string& t2) const {
    int h1 = std::stoi(t1.substr(0, 2));
    int m1 = std::stoi(t1.substr(3, 2));
    int h2 = std::stoi(t2.substr(0, 2));
    int m2 = std::stoi(t2.substr(3, 2));
    
    if (h1 != h2) return h1 - h2;
    return m1 - m2;
}


// Implementera stationsmappning
std::vector<std::string> ScheduleCalculator::GetStationsForEvent(
    const std::string& sportType, 
    const Division& division) const 
{
    if (sportType == "running") {
        if (division.desc.find("sprint") != std::string::npos) {
            return {"Sprint Line (8 tracks)"};
        }
        return {"Running Circle (6 tracks)"};
    }
    else if (sportType == "jumping") {
        if (division.desc == "long" || division.desc == "triple") {
            return {"Long/Triple Jump – I", "Long/Triple Jump – II"};
        }
        else if (division.desc == "high") {
            return {"High Jump – I", "High Jump – II"};
        }
        return {"Pole Vault"};
    }
    else if (sportType == "throwing") {
        return {"Shot Throwing – I", "Shot Throwing – II"};
    }
    return {};
}


// Lägg till logik för beräkning av tävlingstid
int ScheduleCalculator::CalculateDuration(const Sport& sport, const Division& division, int numCompetitors) const {
    if (sport.name == "running") {
        return 10;  // 10 minuter per heat
    }
    else if (sport.name == "jumping") {
        return numCompetitors * 5;  // 5 min per deltagare
    }
    else if (sport.name == "throwing") {
        return numCompetitors * 10; // 10 min per deltagare
    }
    return 15; // fallback
}

