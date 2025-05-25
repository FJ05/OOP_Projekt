#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include <vector>
#include "assets.h"

class ImportSystem {
public:
    void ImportValues(std::vector<Competitor>* competitors, std::vector<Score>* scores, std::vector<Sport>* sports);
    
    Division* findDivision(int age, int specificSport, Sport *s);

};

#endif