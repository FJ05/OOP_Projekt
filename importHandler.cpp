#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "assets.h"

using namespace std;

class ImportSystem 
{
public:
    void ImportValues(vector<Competitor> *competitors, vector<Score> *scores)
    {
        cout << "start importvalues" << endl;
        ifstream dbFile;
        string line = "";
        dbFile.open("DB/compIndex.csv");
        getline(dbFile, line);
        int lineNumber=0;
        
        Score s;

        while(getline(dbFile, line))
        {
            stringstream ss(line);
            string substr;
            vector<string> v;
            Competitor c;

            cout << "test 1" << endl;

            while (getline(ss,substr, ',')) //splits up the line into substr and stores values in v
            {
                v.push_back(substr); 
                cout << substr << endl;
            }
            cout << "test 2" << endl;

            cout << "test 3" << endl;

            //sparar name, surname, age, sex, club
            c.name = v[0];
            c.surname = v[1];
            c.age = stoi(v[2]);
            c.sex = v[3][0];
            c.club = v[4];

            //sparar alla scores i competitor object
            for(int i = 5; i<v.size(); i++)
            {
                s.scoreStr = v[i];
                cout <<"score:" << i-4 << ": "<< v[i] << endl;

            }

            competitors->push_back(c);

            v.clear();
            cout << "name:" << c.name << endl;
            cout << "Full line:" << line << endl;
            lineNumber++;
        }
        dbFile.close();
    }
};
