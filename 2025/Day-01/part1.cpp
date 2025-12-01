#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <stdexcept>


int rotate(int currentState, int rotation){

    int newState{ currentState + rotation };
    while(newState < 0) newState += 100;
    newState %= 100;
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
        currentState = rotate(currentState, rotation);
        if(currentState == 0){
            ++countOfZeros;
        }
    }
    
    std::cout << "The final password is: " << countOfZeros << "\n";
    return 0;
}