#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include <vector>
#include "assets.h"

// ImportSystem klass som hanterar import av data
class ImportSystem {
public:
    void ImportValues(std::vector<Competitor>* competitors,
                      std::vector<Score>* scores,
                      std::vector<Sport>* sports);
};

#endif // INPUTHANDLER_H