#include "assets.h"

/**
 * @brief Construct a new Division:: Division object
 * 
 * @param from 
 * @param to 
 * @param n 
 * @param d 
 * @param g 
 * @param opt 
 */
Division::Division(int from, int to, std::string n, std::string d, char g, std::string opt)
    : ageFrom(from), ageTo(to), name(n), desc(d), optDesc(opt), gender(g) {}