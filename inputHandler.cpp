#include <iostream>
#include <cctype>
using namespace std;

int CreateSport(){
    string name;
    int arenaSize;
    string units;
    char boolian;

    cout << "Create a Sport\nEnter Sport Name: ";
    cin >> name;

    cout << "\nHow many people does the arena host?: ";
    cin >> arenaSize;

    cout << "\nWhat unit will be used for the sport? ex (s, m, min, feet, inches, cm)";
    cin >> units;

    cout << "\nDo you want " << name << " as the name, " << arenaSize << " As the max host of the arena and \n" << units << " as the units of the sport? (y/n)";
    cin >> boolian;

    if(tolower(boolian) == 'y'){
        // Save the name

        // Add Division

        // Add Competitor
    }
    else{
        // Exit to main
        return 0;
    }
}