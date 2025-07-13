#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <tuple>
#include "importHandler.h"
using namespace std;

//används för att hitta rätt id (om id är mellan 0 och 11 så adderar man funktionens return för att få rätt id)
int ImportSystem::findDivision(Competitor *c, Sport *s)
{
    for(int j = 0; j < 12; j++)
    {
        //om det är rätt ålder och rätt kön betyder det att det är rätt division, och returnar j
        if(s->divisionArr[j].ageFrom <= c->age && s->divisionArr[j].ageTo >= c->age && s->divisionArr[j].gender == c->sex)
        {
            return j;
        }
    }
}

void ImportSystem::ImportValues(vector<Competitor>* competitors, vector<Score>* scores, vector<Sport>* sports)
{
    ifstream dbSportFile("DB/sportIndex.csv");
    
    if (!dbSportFile.is_open()) {
        cerr << "Cant open DB/sportIndex.csv\n";
        return;
    }

    string lineSport;

    //int divId = 0;
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
            d.gender = 'M';
            sportPtr->divisionArr.push_back(d);
            //lägger till en female division också
            d.gender = 'F';
            sportPtr->divisionArr.push_back(d);
        }
    }
    dbSportFile.close();    

    /*
    for(int i = 0; i < sports->size(); i++)
    {
        Sport &s = (*sports)[i];
        cout << "Sport namn: " << s.name << " unit: " << s.unit << endl;
        for(int j = 0; j < s.divisionArr.size(); j++)
        {
            Division &d = s.divisionArr[j];
                cout << "info bout division:" << endl;
                //cout << "ID:" << d.id << endl;
                cout << "ageFrom:" << d.ageFrom<< endl;
                cout << "ageTo:" << d.ageTo<< endl;
                cout << "name:" << d.name<< endl;
                cout << "desc:" << d.desc<< endl;
                cout << "optdesc:" << d.optDesc<< endl;
                cout << "gender:" << d.gender<< endl << endl; 
        }
        cout << endl << endl << endl;
    }
    */
    
    
    ifstream dbFile("DB/compIndex.csv");
    if (!dbFile.is_open())
    {
        cerr << "Failed to open DB/compIndex.csv\n";
        return;
    }

    //innehåller startId and sportIndex som används för att hitta rätt division senare
    vector<pair<int, int>> searchValues = {
        {0, 0},
        {12, 0},
        {0, 1},
        {12, 1},
        {24, 1},
        {36, 1},
        {24, 0},
        {0, 2},
        {12, 2},
        {24, 2},
        {0, 3}
    };

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

        //lägger till de fasta och simpla värderna i competitor
        Competitor c;
        c.name = substr[0];
        c.surname = substr[1];
        c.age = stoi(substr[2]);
        c.sex = substr[3][0];
        c.club = substr[4];

        int ageGenderDivDiff = findDivision(&c,&(*sports)[0]);

        //loopar alla scores competitor har
        for (int i = 5; i < substr.size(); i++)
        {
            //om den inte har scoret bara skippa
            if(substr[i].empty())
            {
                continue;
            }
            
            //current sport står för den nuvariga delsporten, like 0 är sprint 60m, 1 är sprint 200m
            int currentSportNumber = i-5;
            
            //cout << "sportnr " << currentSportNumber << endl;
            //cout << "age " << c.age << endl;
            //cout << "The score: " << substr[i] << endl;

            int from = std::get<0>(searchValues[currentSportNumber]);
            int sportId = std::get<1>(searchValues[currentSportNumber]);
            
            //sportId används för att hitta rätt Id
            Sport &s = (*sports)[sportId];
            //from+ageGenderDivDiff ger rätt id för division 
            //(vi vet att id är mellan t.ex mellan 12 och 23, då tar man 12 och adderar skillnaden (svårt att förklara))    
            Division *d = &s.divisionArr[from+ageGenderDivDiff];
            
            /*
            error fixing cout
            if(d != nullptr)
            {
            cout << "info bout division:" << endl;
            cout << "ageFrom:" << d->ageFrom<< endl;
            cout << "ageTo:" << d->ageTo<< endl;
            cout << "name:" << d->name<< endl;
            cout << "desc:" << d->desc<< endl;
            cout << "optdesc:" << d->optDesc<< endl;
            cout << "gender:" << d->gender<< endl;
            }
            */
            
            //lägger till score i competitor
            c.scoreArr.emplace_back();
            Score *scorePtr = &c.scoreArr.back();
            scorePtr->scoreStr = substr[i];
            scorePtr->division = *d;
            scorePtr->sport = &s;

            s.competitorArr.push_back(c);
            //cout << endl;
        }
        competitors->push_back(c);        
    }
    dbFile.close();
}