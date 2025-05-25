#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <set>
#include "importHandler.h"
using namespace std;

    
Division* ImportSystem::findDivision(int age, int specificSport, Sport *s)
{
    cout << "Find specific division :Between number " << specificSport*6 << " and " << specificSport*6+5<< endl;
    //om age är rätt
    for(int j = 0; j < 6; j++)
    {
        if(s->divisionArr[specificSport*6+j].ageFrom <= age && s->divisionArr[specificSport*6+j].ageTo >= age)
        {
            cout << "FOUND IT: The answer is " << specificSport*6+j << endl;
            return &s->divisionArr[specificSport*6+j];
        }
    }
    //probably unecessary
    return nullptr;
}

void ImportSystem::ImportValues(vector<Competitor>* competitors,
                                vector<Score>* scores,
                                vector<Sport>* sports)
{
    //cout << "start importvalues" << endl;

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

        //måste läsa in detta först för att kunna få ut desc1
        string dAgeFrom1;
        string dAgeTo1;
        string dName1;
        string dDesc1;
        string dOptDesc1;

        //sparar sportNamn, arenaSize och unit i variabler
        getline(ss, sportName, ','); 
        getline(ss,temp, ',');
        //lägg till en try catch för att checka temp inte är null
        arenaSize = stoi(temp);
        getline(ss, unit, ',');
        
        Sport* sportPtr = nullptr;
        
        //kollar igenom om sporten redan är skapad (med sport menar vi running(s), running(min), jumping och throwing)
        //om den finns får sportPtr pointer värdet av den existerande sporten
        for (int i = 0; i < sports->size(); i++)
        {
            Sport &s = (*sports)[i];
            //ändra till sportName
            if (s.name == sportName && s.unit == unit) 
            {
                sportPtr = &s;
                break;
            }
        }

        //om sporten inte fanns så skapar vi den
        if (!sportPtr) 
        {
            sports->emplace_back();
            sportPtr = &sports->back();
            sportPtr->name = sportName;
            sportPtr->arenaSize = arenaSize;
            sportPtr->unit = unit;
        }

        //split up all values into different divisions
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

            //cout << "This is ageTo" << d.ageTo << endl;
            //cout << "This is name and desc and optdesc" << d.name << d.desc << d.optDesc << endl;

            sportPtr->divisionArr.push_back(d);
            
        }
    }
    dbSportFile.close();    

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

        cout << c.name << endl;

        //loopar för varje score competitor har
        for (int i = 5; i < substr.size(); i++)
        {
            //om den inte har scoret bara skippa
            if(substr[i].empty())
            {
                continue;
            }
            //current sport stands for the current subsport, like 0 is sprint 60m, 1 is sprint 200m
            int currentSportNumber = i-5;
            cout << "sportnr " << currentSportNumber << endl;
            cout << "age " << c.age << endl;
            cout << "The score: " << substr[i] << endl;



            Sport *s;
            Division *d;
            //finds subSport
            if(currentSportNumber <= 2) //18/6 because there are 3 subsports with 6 age divisions, 6*3=18 
            {
                int specificSport = currentSportNumber;
                //the sport pointer for the big sport (running(s), running(min), jumping, throwing)
                Sport *s = &(*sports)[0];

                cout << "Specific sport number " << specificSport << endl;
                cout << "sport running s, 0" << endl;
                //finds specific age group division for the subsport
                d = findDivision(c.age, specificSport, s);
                //cout << "The sport is running s and specific " << specificSport << endl;
            }
            else if(currentSportNumber <= 5) //(18+18)/6 = 6,   6-1 = 5
            {
                int specificSport = currentSportNumber - 3;
                //the sport pointer for the big sport (running(s), running(min), jumping, throwing)
                Sport *s = &(*sports)[1];
                //finds specific age group division for the subsport
                d = findDivision(c.age, specificSport, s);
                cout << "The sport is running min" << endl;
            }
            else if(currentSportNumber <= 9) //(18+18+24)/6 = 10,  10-1=9
            {
                int specificSport = currentSportNumber - 6;
                Sport *s = &(*sports)[2];
                d = findDivision(c.age, specificSport, s);
                cout << "The sport is jumping " << endl;
            }
            else //throwing scoret är else
            {
                int specificSport = currentSportNumber - 10;
                Sport *s = &(*sports)[3];
                d = findDivision(c.age, specificSport, s);
                cout << "The sport is throwing " << endl;
            }
            cout << "info bout division:" << endl;
            cout << "ageFrom:" << d->ageFrom<< endl;
            cout << "ageTo:" << d->ageTo<< endl;
            cout << "name:" << d->name<< endl;
            cout << "desc:" << d->desc<< endl;
            cout << "optdesc:" << d->optDesc<< endl;
            cout << "gender:" << d->gender<< endl;
            //ändra till emplace
            Score sc;
            sc.scoreStr = substr[i];
            
            //current sport number
           

            //if age >= ageFrom och age <= ageTo
            // add division to score
            c.scoreArr.push_back(sc);
            cout << endl;
        }  
        
        /*
        for (int i = 5; i < substr.size() && i-5 < spDvGroups.size(); i++) 
        {
            
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
        */
        // för varje score competitor kan ha
        
    }

    dbFile.close();
    cout << "importvalues done: "
         << competitors->size() << " competitors, "
         << scores->size()      << " scores, "
         << sports->size()      << " sports\n";
}