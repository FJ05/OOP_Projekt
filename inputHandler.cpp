#include <iostream>
#include <cctype>
using namespace std;

int CreateSport(){
    string sportName;
    char boolian;

    cout << "Create a Sport\nEnter Sport Name: ";
    cin >> sportName;

    cout << "\nDo you want " << sportName << " as the name of the sport?\n(y/n): ";
    cin >> boolian;

    if(tolower(boolian) == 'y'){
        // Save the name
    }
    else{
        // Exit to main
    }
}