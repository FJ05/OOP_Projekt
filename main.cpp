#include <iostream>
#include "importHandler.cpp"
int main(){
    /*This main file will be used as the launcher to combine all the scripts*/
    ImportSystem importer;
    vector<Competitor> competitors;
    vector<Score> scores;

    importer.ImportValues(&competitors, &scores);
    std::cout << "done";
    return 0;
}