#include <iostream>
#include <vector>
#include "assets.h"
#include "importHandler.h" // Du måste ev. lägga ImportSystem-klassen här
                          // eller inkludera den via separat headerfil

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
        cout << "Sport: " << sport.name << " (" << sport.unit << ")" << endl;
        cout << "  Antal tävlande: " << sport.competitorArr.size() << endl;
        cout << "  Divisioner:" << endl;
        for (const auto& d : sport.divisionArr) {
            cout << "    - " << d.name << " (" << d.desc << ")" << endl;
        }
    }

    return 0;
}