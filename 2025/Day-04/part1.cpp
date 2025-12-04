#include <iostream>
#include <stdexcept>
#include <vector>
#include <string>
#include <fstream>


void getGrid(std::string fileName, std::vector<std::vector<bool>> &paperRollGrid){
    
    std::ifstream file(fileName);
    std::string line{};

    if(file.is_open()){
        while(getline(file,line)){
            std::vector<bool> row{};
            for(auto c : line){
                if(c == '.') row.push_back(false);
                else if(c == '@') row.push_back(true);
            }
            paperRollGrid.push_back(row);
        }
        file.close();
    }
    else throw std::invalid_argument("Could not open the input file.");
}


unsigned int countAccessibleRolls(std::vector<std::vector<bool>> paperRollGrid){

    unsigned int finalCount{};
    unsigned int gridRows{ static_cast<unsigned int>(paperRollGrid.size()) };
    unsigned int gridCols{ static_cast<unsigned int>(paperRollGrid[0].size()) };

    for(unsigned int i=0; i<gridRows; ++i){
        for(unsigned int j=0; j<gridCols; ++j){
            if(!paperRollGrid[i][j]) continue;

            unsigned short aroundCount{};
            if(i>0 && paperRollGrid[i-1][j]) ++aroundCount;          //Up
            if(j>0 && paperRollGrid[i][j-1]) ++aroundCount;          //Left
            if(i>0 && j>0 && paperRollGrid[i-1][j-1]) ++aroundCount; //Up-Left
            if(i<gridRows-1 && paperRollGrid[i+1][j]) ++aroundCount; //Down
            if(j<gridCols-1 && paperRollGrid[i][j+1]) ++aroundCount; //Right
            if(i<gridRows-1 && j<gridCols-1 && paperRollGrid[i+1][j+1]) ++aroundCount; //Down-Right
            if(i<gridRows-1 && j>0 && paperRollGrid[i+1][j-1]) ++aroundCount; //Down-Left
            if(i>0 && j<gridCols-1 && paperRollGrid[i-1][j+1]) ++aroundCount; //Up-Right

            if(aroundCount<4) ++finalCount;
        }
    }
    return finalCount;
}


int main(){

    std::vector<std::vector<bool>> paperRollGrid{};
    getGrid("input", paperRollGrid);

    unsigned int finalResult{ countAccessibleRolls(paperRollGrid) };

    std::cout << "The final count of accessible paper rolls is: " << finalResult << "\n";
    return 0;
}