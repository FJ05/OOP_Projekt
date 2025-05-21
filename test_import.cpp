// test_import.cpp
// Simple test for ImportSystem: reads existing DB/compIndex.csv and DB/sportIndex.csv,
// imports data into memory, and prints out competitors and their scores.

#include <iostream>
#include <vector>
#include "assets.h"
#include "importHandler.h"

int main() {
    // Containers for imported data
    std::vector<Competitor> competitors;
    std::vector<Score> scores;
    std::vector<Sport> sports;

    // Perform import
    ImportSystem importer;
    importer.ImportValues(&competitors, &scores, &sports);

    // Print summary
    std::cout << "Imported " << competitors.size() << " competitors, "
              << scores.size() << " scores, "
              << sports.size() << " sports.\n\n";

    // Print each competitor with their scores
    for (const auto& comp : competitors) {
        std::cout << comp.name << " " << comp.surname
                  << ", age " << comp.age
                  << ", sex " << comp.sex
                  << ", club " << comp.club << "\n";
        if (comp.scoreArr.empty()) {
            std::cout << "  (no scores)\n";
        } else {
            std::cout << "  Scores:\n";
            for (const auto& sc : comp.scoreArr) {
                std::cout << "    "
                          << (sc.sport ? sc.sport->name : "<none>")
                          << ": " << sc.division.desc;
                if (!sc.division.name.empty())
                    std::cout << " " << sc.division.name;
                std::cout << " = " << sc.scoreStr << "\n";
            }
        }
        std::cout << "\n";
    }

    return 0;
}
