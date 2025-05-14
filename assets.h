#ifndef ASSETS_H
#define ASSETS_H
#include <string>
#include <vector>

class Sport;  // Forward declaration

class Division {
public:
    int ageFrom{};
    int ageTo{};
    std::string name{};
    std::string desc{};
    std::string optDesc{};

    Division() = default;
    // Const for Division
    Division(int from, int to, std::string n, std::string d, std::string opt = "");
};

class Score {
public:
    Sport* sport = nullptr;
    std::string scoreStr{};
    Division division{};
};

class Competitor {
public:
    std::string name{};
    std::string surname{};
    int age{};
    char sex{};
    std::string club{};
    std::vector<Score> scoreArr{};
};

class Sport {
public:
    std::string name{};
    std::vector<Competitor> competitorArr{};
    std::vector<Division> divisionArr{};
    std::string unit{};
};

#endif