#include "schedulecalculator.h"
#include <algorithm>
#include <map>
#include <cctype>
#include <stdexcept>

using namespace std;

/**
 * @brief 
 * 
 * @param sports 
 * @return vector<ScheduledEvent> 
 */
vector<ScheduledEvent> ScheduleCalculator::GenerateSchedule(const vector<Sport>& sports) {
    vector<ScheduledEvent> schedule;
    map<string, string> stationMap = {
        {"Long/Triple Jump – I", "Long_Jump 1"},
        {"Long/Triple Jump – II", "Long_Jump 2"},
        {"High Jump – I", "High_Jump 1"},
        {"High Jump – II", "High_Jump 2"},
        {"Pole Vault", "Pole_Vault 1"},
        {"Shot Throwing – I", "Shot_Put 1"},
        {"Shot Throwing – II", "Shot_Put 2"}
    };

    int currentDay = 1;
    string currentTime = "08:00";
    string dailyCutoff = "22:00";

    for(const auto& sport : sports) {
        for(const auto& division : sport.divisionArr) {
            // Hoppa över divisioner utan tävlande
            if(sport.competitorArr.empty()) continue;

            // Hämta aktuella tävlande
            vector<Competitor> competitors;
            for(const auto& comp : sport.competitorArr) {
                if(comp.age >= division.ageFrom && comp.age <= division.ageTo) {
                    competitors.push_back(comp);
                }
            }

            // Hoppa över om inga tävlande
            if(competitors.empty()) continue;

            // Hantera dagövergång
            while(IsTimeAfterOrEqual(currentTime, dailyCutoff)) {
                currentDay++;
                currentTime = "08:00";
            }

            // Beräkna tider
            auto stations = GetStationsForEvent(sport.name, division);
            string discipline = division.desc;
            transform(discipline.begin(), discipline.end(), discipline.begin(), ::toupper);

            int duration = CalculateDuration(sport, division, competitors.size());
            string endTime = AddTime(currentTime, duration);

            // Skapa event
            ScheduledEvent event{
                stationMap[stations[0]],
                discipline,
                division.gender,
                to_string(division.ageFrom) + "-" + to_string(division.ageTo),
                duration,
                "Day: " + to_string(currentDay) + " " + currentTime + "-" + endTime,
                competitors
            };

            schedule.push_back(event);
            currentTime = endTime;
        }
    }
    return schedule;
}

/**
 * @brief 
 * 
 * @param currentTime 
 * @param minutes 
 * @return string 
 */
string ScheduleCalculator::AddTime(const string& currentTime, int minutes) const {
    // Validera tidsformat
    if(currentTime.length() != 5 || 
       currentTime[2] != ':' ||
       !isdigit(currentTime[0]) || 
       !isdigit(currentTime[1]) ||
       !isdigit(currentTime[3]) ||
       !isdigit(currentTime[4])) {
        throw invalid_argument("Ogiltigt tidsformat: " + currentTime);
    }

    int hours = stoi(currentTime.substr(0, 2));
    int mins = stoi(currentTime.substr(3, 2));
    
    mins += minutes;
    hours += mins / 60;
    mins %= 60;
    
    // Formatera utan dagbegränsning
    char buffer[6];
    snprintf(buffer, sizeof(buffer), "%02d:%02d", hours, mins);
    return string(buffer);
}

/**
 * @brief 
 * 
 * @param time1 
 * @param time2 
 * @return true 
 * @return false 
 */
bool ScheduleCalculator::IsTimeAfterOrEqual(const string& time1, const string& time2) const {
    int h1 = stoi(time1.substr(0, 2));
    int m1 = stoi(time1.substr(3, 2));
    int h2 = stoi(time2.substr(0, 2));
    int m2 = stoi(time2.substr(3, 2));
    
    return (h1 > h2) || (h1 == h2 && m1 >= m2);
}
/**
 * @brief 
 * 
 * @param sport 
 * @param division 
 * @param numCompetitors 
 * @return int 
 */
int ScheduleCalculator::CalculateDuration(const Sport& sport, const Division& division, int numCompetitors) const {
    if(numCompetitors <= 0) return 0;
    
    if(sport.name == "running") return 10 + (numCompetitors * 2);
    if(sport.name == "jumping") {
        if(division.desc == "high") return 5 * numCompetitors;
        return 7 * numCompetitors; // längre tid för längdhopp
    }
    if(sport.name == "throwing") return 10 * numCompetitors;
    return 15; // fallback
}

/**
 * @brief 
 * 
 * @param sportType 
 * @param division 
 * @return vector<string> 
 */
vector<string> ScheduleCalculator::GetStationsForEvent(
    const string& sportType, 
    const Division& division) const 
{
    if(sportType == "running") {
        return {"Sprint Line (8 tracks)"};
    }
    if(sportType == "jumping") {
        if(division.desc == "long" || division.desc == "triple") {
            return {"Long/Triple Jump – I", "Long/Triple Jump – II"};
        }
        return {"High Jump – I", "High Jump – II"};
    }
    if(sportType == "throwing") {
        return {"Shot Throwing – I", "Shot Throwing – II"};
    }
    return {}; // fallback
}