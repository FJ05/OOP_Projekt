#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

class ImportSystem 
{
public:
    void ImportValues()
    {
        cout << "start importvalues" << endl;
        ifstream dbFile;
        string line = "";
        dbFile.open("db.csv");
        getline(dbFile, line);

        while(getline(dbFile, line))
        {
            stringstream ss(line);
            
            string substr;
            vector<string> v;

            while (getline(ss,substr, ',')) //splits up the line into substr and stores values in v
            {
                v.push_back(substr); 
                //cout << substr << endl;

            }

            //save name = v[0];
            //save surname = v[1];
            //etc... in order of the csv file
            v.clear();

            cout << "Full line:" << line << endl;
        }
        dbFile.close();
    }
};