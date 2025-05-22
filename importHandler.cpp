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

    struct SpDvGroup { Sport* sport; Division* div; };
    vector<SpDvGroup> spDvGroups;
    spDvGroups.reserve(eventNames.size());

    for (int i = 0; i < eventNames.size(); i++)
    {
        string& ev = eventNames[i];

        // splittar desc and name
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
        spDvGroups.push_back({sp,dv});
    }

    ifstream dbFile("DB/compIndex.csv");
    if (!dbFile.is_open()) 
    {
        cerr << "Failed to open DB/compIndex.csv\n";
        return;
    }

    string line;
    while (getline(dbFile, line)) 
    {
        stringstream ss(line);
        vector<string> substr;
        string temp;
        while (getline(ss, temp, ',')) 
        {
            substr.push_back(temp);
        }

        //ändra till emplace back sen
        Competitor c;
        c.name = substr[0];
        c.surname = substr[1];
        c.age = stoi(substr[2]);
        c.sex = substr[3][0];
        c.club = substr[4];

        // för varje score competitor kan ha
        for (int i = 5; i < substr.size() && i-5 < spDvGroups.size(); i++) 
        {
            //om competitor inte har scoret
            if (substr[i].empty())
            {
                continue; 
            }

            Sport* sp = spDvGroups[i-5].sport;
            Division* dv = spDvGroups[i-5].div;

            //om en är nullptr
            if (!sp || !dv) 
            {
                continue;
            }
            //ändra till emplace back sen istället bättre
            Score sc;
            sc.scoreStr = substr[i];
            sc.sport = sp;
            sc.division = *dv; //ändra till pointer sen

            //sparar värderna i klassobjecten
            c.scoreArr.push_back(sc);
            scores->push_back(sc);
            //ändra sen till en pointer och inte hela c
            sp->competitorArr.push_back(c);
        }

        //FIXA DETTA!!!!

        // assets använder ibland pointers och ibland inte
        //fixa så de alltid använder pointers
        //ksk fixar buggen med för många competitors i sport array saken

        competitors->push_back(c);
    }

    dbFile.close();
    cout << "importvalues done: "
         << competitors->size() << " competitors, "
         << scores->size()      << " scores, "
         << sports->size()      << " sports\n";
}