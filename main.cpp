#include <iostream>
#include <vector>
#include "assets.h"
#include "importHandler.h"

using namespace std;

int main()
{
    vector<Competitor> competitors;
    vector<Score> scores;
    vector<Sport> sports;

    ImportSystem importer;
    importer.ImportValues(&competitors, &scores, &sports);

    // Test: skriv ut resultat
    cout << "Antal tävlande: " << competitors.size() << endl;
    for (const auto& sport : sports) {
        cout << "Sport: " << sport.name << " (" << sport.unit << "), Arena size: " << sport.arenaSize << endl;
        cout << "  Antal tävlande: " << sport.competitorArr.size() << endl;
        cout << "  Divisioner:" << endl;
        for (const auto& d : sport.divisionArr) {
            cout << "    - " << d.name << " (" << d.desc << "), Optional: " << d.optDesc << endl;
        }
    }

    return 0;
}