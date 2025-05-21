#include <iostream>
#include <fstream>
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
    char boolian;
    int ageFrom;
    int ageTo;
    string name;
    string desc;
    string optDec;

    cout << "--- Creating Division ---" << endl;
    cout << "Enter Age from: ";
    cin >> ageFrom;

    cout << "\nEnter Age to: ";
    cin >> ageTo;

    cout << "\nEnter Division name: ";
    cin >> name;

    cout << "\nEnter Division Description: ";
    cin >> desc;

    cout << "\nDo you want to add an extra description? (y/n)";
    cin >> boolian;

    if(tolower(boolian) == 'y'){
        cout << "\nEnter Extra Division Description: ";
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

    cout << "\n--- Creating Competitor ---" << endl;
    cout << "Enter name : ";
    cin >> name;

    cout << "\nEnter surname: ";
    cin >> surname;

    cout << "\nEnter age: ";
    cin >> age;

    cout << "\nEnter sex: ";
    cin >> sex;

    cout << "\nEnter club name: ";
    cin >> club;

    // Add score array


    ///// NYTT FRÅN DAVID /////////////////////////////////////////
    ofstream outFile("DB/compIndex.csv", ios::app); // öppna i "append"-läge
    if (!outFile) {
        cerr << "Kunde inte öppna filen.\n";
        return 1;
    }

    outFile << name << "," << surname << "," << age << "," << sex << "," << club; // ADD SCORE


    outFile << "\n";
    outFile.close();
///////////////////////////////////////////////////////////////////
    

}

int CreateScore(){
    string scoreStr;
    // Add sport
    // Add divistion

    cout << "\n--- Create Score ---";
    cout << "\n Enter score: ";
    cin >> scoreStr;
}

/*
TODO

Add error handlig and try statements with the inputs
*/