#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <set>
#include "importHandler.h"
using namespace std;

void ImportSystem::ImportValues(vector<Competitor>* competitors,
                                vector<Score>* scores,
                                vector<Sport>* sports)
{
    cout << "start importvalues" << endl;

    ifstream dbSportFile("DB/sportIndex.csv");
    
    if (!dbSportFile.is_open()) {
        cerr << "Cant open DB/sportIndex.csv\n";
        return;
    }

    vector<string> eventNames;
    set<string> seenEvents;
    string lineSport;

    while (getline(dbSportFile, lineSport)) 
    {
        stringstream ss(lineSport);
        string sportName;
        int arenaSize;
        string unit;
        
        string temp;

        //sparar sportNamn, arenaSize och unit i variabler
        getline(ss, sportName, ','); 
        getline(ss,temp, ',');
        //lägg till en try catch för att checka temp inte är null
        arenaSize = stoi(temp);
        getline(ss, unit, ',');
        
        Sport* sportPtr = nullptr;
        
        //kollar igenom om det redan finns en sport med det namnet
        //om den finns får sportPtr pointer värdet av den existerande sporten
        
        // PROBLEM: kan finnas flera objekt med samma sport och unit, ksk fixa sportNamnet och desc innan o jämföra
        for (int i = 0; i < sports->size(); i++)
        {
            Sport &s = (*sports)[i];
            if (s.name == sportName && s.unit == unit) 
            {
                sportPtr = &s;
                break;
            }
        }

        //om sportPtr är tom ska vi skapa sporten
        if (!sportPtr) 
        {
            sports->emplace_back();
            sportPtr = &sports->back();
            sportPtr->name = sportName;
            sportPtr->arenaSize = arenaSize;
            sportPtr->unit = unit;
        }

        // split up all values into different divisions
        while (true) {
            Division d;
            //if there is no more values
            if (!(ss >> d.ageFrom)) 
            {
                break;
            }
            ss.ignore(1);
            ss >> d.ageTo;
            ss.ignore(1);
            getline(ss, d.name, ',');
            getline(ss, d.desc, ',');
            getline(ss, d.optDesc, ',');

            cout << "This is ageTo" << d.ageTo << endl;
            cout << "This is name and desc and optdesc" << d.name << d.desc << d.optDesc << endl;

            // add division in current sport
            sportPtr->divisionArr.push_back(d);

            //checks so that there are no doubles of divisions, only one "60m sprint"
            string ev = d.desc + " " + d.name;
            if (seenEvents.insert(ev).second) 
            {
                cout << "Denna kom in " << ev << endl;
                eventNames.push_back(ev);
            }
            
        }
    }
    dbSportFile.close();

    cout << "This is the end of while" << endl;

    struct ColMap { Sport* sport; Division* div; };
    vector<ColMap> colMaps;
    colMaps.reserve(eventNames.size());

    for (int i = 0; i < eventNames.size(); i++)
    {
        string& ev = eventNames[i];

        // split desc and name
        int pos = ev.find(' ');
        string desc = ev.substr(0,pos);
        string name = ev.substr(pos+1);

        //hittar sporten och divisionen
        Sport* sp  = nullptr;
        Division* dv = nullptr;
        for (int i = 0; i < sports->size();i++) 
        {
            Sport& s = (*sports)[i];
            for (int j = 0; j < s.divisionArr.size(); j++) {
                Division& d = s.divisionArr[j];
                if (d.desc == desc && d.name == name) {
                    sp = &s;
                    dv = &d;
                    break;
                }
            }
            if (sp)
            {
                break;
            }
        }
        colMaps.push_back({sp,dv});
    }

    ifstream dbFile("DB/compIndex.csv");
    if (!dbFile.is_open()) {
        cerr << "Failed to open DB/compIndex.csv\n";
        return;
    }

    string line;
    while (getline(dbFile, line)) {
        stringstream ss(line);
        vector<string> fields;
        string fld;
        while (getline(ss, fld, ',')) {
            fields.push_back(fld);
        }
        // need at least 5 base fields + event columns
        if (fields.size() < 5) continue;

        // build competitor
        Competitor c;
        c.name = fields[0];
        c.surname = fields[1];
        c.age = stoi(fields[2]);
        c.sex = fields[3].empty() ? 'U' : fields[3][0];
        c.club = fields[4];

        // for each event column
        for (size_t i = 5; i < fields.size() && i-5 < colMaps.size(); ++i) {
            if (fields[i].empty()) continue;  // no score

            Sport*     sp = colMaps[i-5].sport;
            Division*  dv = colMaps[i-5].div;
            if (!sp || !dv) continue;        // unknown mapping

            Score sc;
            sc.scoreStr = fields[i];
            sc.sport    = sp;
            sc.division = *dv;

            // attach score to competitor and sport
            c.scoreArr.push_back(sc);
            scores->push_back(sc);
            sp->competitorArr.push_back(c);
        }

        competitors->push_back(c);
    }

    dbFile.close();
    cout << "importvalues done: "
         << competitors->size() << " competitors, "
         << scores->size()      << " scores, "
         << sports->size()      << " sports\n";
}