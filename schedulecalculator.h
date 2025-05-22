#ifndef SCHEDULECALCULATOR_H
#define SCHEDULECALCULATOR_H

#include "assets.h"
#include <vector>
#include <string>

struct ScheduledEvent {
    std::string station;
    std::string discipline;
    char gender;
    std::string ageGroup;
    int totalTime;
    std::string timeSlot;
    std::vector<Competitor> competitors;
};

class ScheduleCalculator {
public:
    std::vector<ScheduledEvent> GenerateSchedule(const std::vector<Sport>& sports);
    
private:
    std::string AddTime(const std::string& currentTime, int minutes) const;
    bool IsTimeAfterOrEqual(const std::string& time1, const std::string& time2) const;
    int CalculateDuration(const Sport& sport, const Division& division, int numCompetitors) const;
    std::vector<std::string> GetStationsForEvent(const std::string& sportType, const Division& division) const;
};
#endif