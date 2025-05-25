#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include <vector>
#include "assets.h"

class ImportSystem {
public:
    void ImportValues(std::vector<Competitor>* competitors, std::vector<Score>* scores, std::vector<Sport>* sports);
    int findDivision(Competitor *c, Sport *s);
};

#endif