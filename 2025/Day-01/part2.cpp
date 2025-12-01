#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <stdexcept>
#include <cmath>


int rotate(int currentState, int rotation, int &countOfZeros){

    int newState{ currentState + rotation };
    while(newState < 0) newState += 100;
    newState %= 100;

    // Newly added logic for part 2
    countOfZeros += static_cast<int>(std::abs(rotation)/100);
    if     (currentState!=0 && rotation<0 && newState>currentState) ++countOfZeros;
    else if(currentState!=0 && rotation>0 && newState<currentState) ++countOfZeros;
    else if(currentState>0 && newState==0) ++countOfZeros;

    return newState;
}


int parseToInt(std::string input){

    int finalRotation{};
    
    if(input[0] == 'L') finalRotation = -1;
    else if(input[0] == 'R') finalRotation = 1;
    else throw std::invalid_argument("Incorrect line format detected.");

    int value = std::stoi(input.substr(1));
    finalRotation *= value;
    return finalRotation;
}


std::vector<int> getRotationsFromFile(std::string fileName){

    std::vector<int> rotations{};
    
    std::ifstream file;
    std::string line;

    file.open(fileName);
    if(file.is_open()){
        while(getline(file, line)){
            rotations.push_back(parseToInt(line));
        }
        file.close();
    }
    else throw std::invalid_argument("Could not read the file.");

    return rotations;
}


int main(){

    // Get the rotations from the input file
    std::vector<int> rotations = getRotationsFromFile("input");

    int currentState{ 50 }; // State of the dial
    int countOfZeros{ 0 };  // Return password
    
    // Perform all rotations
    for(int rotation : rotations){
        currentState = rotate(currentState, rotation, countOfZeros);
    }
    
    std::cout << "The final password is: " << countOfZeros << "\n";
    return 0;
}