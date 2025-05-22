#ifndef ASSETS_H
#define ASSETS_H

#include <string>
#include <vector>

class Sport;

class Division {
public:
    int ageFrom;
    int ageTo;
    std::string name;
    std::string desc;
    std::string optDesc;
    char gender;

    Division() = default;
    Division(int from, int to, std::string n, std::string d, char g, std::string opt = "");
};

class Score {
public:
    Sport* sport;
    std::string scoreStr;
    Division division;
};

class Competitor {
public:
    std::string name;
    std::string surname;
    int age;
    char sex;
    std::string club;
    std::vector<Score> scoreArr;
};

class Sport {
public:
    std::string name;
    std::vector<Competitor> competitorArr;
    std::vector<Division> divisionArr;
    std::string unit;
    int arenaSize;
};

#endif