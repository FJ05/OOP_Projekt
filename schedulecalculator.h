#ifndef SCHEDULECALCULATOR_H
#define SCHEDULECALCULATOR_H

#include <vector>
#include <string>
#include "assets.h"

struct ScheduledEvent {
    std::string station;
    std::string startTime;
    std::string endTime;
    Sport sport;
    Division division;
    std::vector<Competitor> competitors;
};

class ScheduleCalculator {
public:
    std::vector<ScheduledEvent> GenerateSchedule(const std::vector<Sport>& sports);
    
private:
    std::string AddTime(const std::string& currentTime, int minutes) const;
    int CompareTime(const std::string& t1, const std::string& t2) const;
    std::vector<std::string> GetStationsForEvent(const std::string& sportType, const Division& division) const;
    int CalculateDuration(const Sport& sport, const Division& division, int numCompetitors) const;
};

#endif