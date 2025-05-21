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

    // --- 1) Read sportIndex.csv → build sports + divisions + eventNames in export order ---
    ifstream dbSportFile("DB/sportIndex.csv");
    if (!dbSportFile.is_open()) {
        cerr << "Failed to open DB/sportIndex.csv\n";
        return;
    }

    vector<string> eventNames;
    set<string> seenEvents;
    string lineSport;
    while (getline(dbSportFile, lineSport)) {
        stringstream ss(lineSport);
        string sportName;
        int arenaSize;
        string unit;

        // read the first three fields
        getline(ss, sportName, ',');
        ss >> arenaSize;      // arenaSize
        ss.ignore(1);         // comma
        getline(ss, unit, ',');

        // find or create Sport
        Sport* sportPtr = nullptr;
        for (auto& s : *sports) {
            if (s.name == sportName) {
                sportPtr = &s;
                break;
            }
        }
        if (!sportPtr) {
            sports->emplace_back();
            sportPtr = &sports->back();
            sportPtr->name       = sportName;
            sportPtr->arenaSize  = arenaSize;
            sportPtr->unit       = unit;
        }

        // parse all division‐blocks in this line
        while (true) {
            Division d;
            if (!(ss >> d.ageFrom)) break;  // no more blocks
            ss.ignore(1);
            ss >> d.ageTo;
            ss.ignore(1);
            getline(ss, d.desc, ',');
            getline(ss, d.name, ',');

            // add division
            sportPtr->divisionArr.push_back(d);

            // record eventName = "desc name"
            string ev = d.desc + (d.name.empty() ? "" : " " + d.name);
            if (seenEvents.insert(ev).second) {
                eventNames.push_back(ev);
            }

            if (ss.peek() == EOF) break;
        }
    }
    dbSportFile.close();

    // --- 2) Build a mapping vector so index i in compIndex → Sport* + Division* ---
    struct ColMap { Sport* sport; Division* div; };
    vector<ColMap> colMaps;
    colMaps.reserve(eventNames.size());

    for (auto& ev : eventNames) {
        // split into desc + name
        auto pos = ev.find(' ');
        string desc = (pos==string::npos ? ev : ev.substr(0,pos));
        string name = (pos==string::npos ? "" : ev.substr(pos+1));

        Sport*  sp  = nullptr;
        Division* dv = nullptr;
        for (auto& s : *sports) {
            for (auto& d : s.divisionArr) {
                if (d.desc == desc && d.name == name) {
                    sp = &s;
                    dv = &d;
                    break;
                }
            }
            if (sp) break;
        }
        colMaps.push_back({sp,dv});
    }

    // --- 3) Read compIndex.csv (no header!) ---
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
        c.name    = fields[0];
        c.surname = fields[1];
        c.age     = stoi(fields[2]);
        c.sex     = fields[3].empty() ? 'U' : fields[3][0];
        c.club    = fields[4];

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
