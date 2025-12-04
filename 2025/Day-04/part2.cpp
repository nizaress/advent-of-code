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


// New logic for part 2
void checkRemovePropagate(unsigned int row, unsigned int col, std::vector<std::vector<bool>> &paperRollGrid, unsigned int &finalCount, unsigned int gridRows, unsigned int gridCols){
    
    if(!paperRollGrid[row][col]) return;

    // Check if we can remove
    unsigned short aroundCount{};
    if(row>0 && paperRollGrid[row-1][col]) ++aroundCount;            //Up
    if(col>0 && paperRollGrid[row][col-1]) ++aroundCount;            //Left
    if(row>0 && col>0 && paperRollGrid[row-1][col-1]) ++aroundCount; //Up-Left
    if(row<gridRows-1 && paperRollGrid[row+1][col]) ++aroundCount;   //Down
    if(col<gridCols-1 && paperRollGrid[row][col+1]) ++aroundCount;   //Right
    if(row<gridRows-1 && col<gridCols-1 && paperRollGrid[row+1][col+1]) ++aroundCount; //Down-Right
    if(row<gridRows-1 && col>0 && paperRollGrid[row+1][col-1]) ++aroundCount; //Down-Left
    if(row>0 && col<gridCols-1 && paperRollGrid[row-1][col+1]) ++aroundCount; //Up-Right

    if(aroundCount<4){
        ++finalCount;
        // We remove the paper roll from this position
        paperRollGrid[row][col] = false;

        // Now we propagate the change
        if(row>0 && paperRollGrid[row-1][col]) checkRemovePropagate(row-1, col, paperRollGrid, finalCount, gridRows, gridCols);              //Up
        if(col>0 && paperRollGrid[row][col-1]) checkRemovePropagate(row, col-1, paperRollGrid, finalCount, gridRows, gridCols);              //Left
        if(row>0 && col>0 && paperRollGrid[row-1][col-1]) checkRemovePropagate(row-1, col-1, paperRollGrid, finalCount, gridRows, gridCols); //Up-Left
        if(row<gridRows-1 && paperRollGrid[row+1][col]) checkRemovePropagate(row+1, col, paperRollGrid, finalCount, gridRows, gridCols);     //Down
        if(col<gridCols-1 && paperRollGrid[row][col+1]) checkRemovePropagate(row, col+1, paperRollGrid, finalCount, gridRows, gridCols);     //Right
        if(row<gridRows-1 && col<gridCols-1 && paperRollGrid[row+1][col+1]) checkRemovePropagate(row+1, col+1, paperRollGrid, finalCount, gridRows, gridCols); //Down-Right
        if(row<gridRows-1 && col>0 && paperRollGrid[row+1][col-1]) checkRemovePropagate(row+1, col-1, paperRollGrid, finalCount, gridRows, gridCols); //Down-Left
        if(row>0 && col<gridCols-1 && paperRollGrid[row-1][col+1]) checkRemovePropagate(row-1, col+1, paperRollGrid, finalCount, gridRows, gridCols); //Up-Right
    }
}


void countAccessibleRolls(std::vector<std::vector<bool>> &paperRollGrid, unsigned int &finalCount){

    unsigned int gridRows{ static_cast<unsigned int>(paperRollGrid.size()) };
    unsigned int gridCols{ static_cast<unsigned int>(paperRollGrid[0].size()) };

    for(unsigned int i=0; i<gridRows; ++i){
        for(unsigned int j=0; j<gridCols; ++j){
            checkRemovePropagate(i, j, paperRollGrid, finalCount, gridRows, gridCols);
        }
    }
}


int main(){

    std::vector<std::vector<bool>> paperRollGrid{};
    getGrid("input", paperRollGrid);

    unsigned int finalResult{};
    countAccessibleRolls(paperRollGrid, finalResult);

    std::cout << "The final count of accessible paper rolls is: " << finalResult << "\n";
    return 0;
}