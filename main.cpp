#include <iostream>
#include "importHandler.cpp"
int main(){
    /*This main file will be used as the launcher to combine all the scripts*/
    ImportSystem importer;
    importer.ImportValues();
    std::cout << "done";
    return 0;
}