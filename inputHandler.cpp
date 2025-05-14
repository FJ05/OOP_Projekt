#include <iostream>
#include <cctype>
using namespace std;

int CreateSport(){
    string name;
    int arenaSize;
    string units;
    char boolian;

    cout << "--- Create a Sport ---\nEnter Sport Name: ";
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

int CreateDivision(){
    int ageFrom;
    int ageTo;
    string name;
    string desc;
    string optDec;

    cout << "--- Creating Division ---" << endl;
    cout << "Enter Age from: ";
    cin >> ageFrom;

    cout << "Enter Age to: ";
    cin >> ageTo;

    cout << "Enter Division name: ";
    cin >> name;

    cout << "Enter Division Description: ";
    cin >> desc;

    char boolian;
    cout << "Do you want to add an extra description? (y/n)";
    cin boolian;

    if(tolower(boolian) == 'y'){
	cout << "Enter Extra Division Description: ";
	cin >> optDec;
    }
    else{
	optDec == "";
    }

    return 0;
}

int CreateCompetitior(){
    string name;
    string surname;
    int age;
    char sex;
    string club;
    // Add score array

}

int CreateScore(){
    string scoreStr;
    // Add sport
    // Add divistion
}
