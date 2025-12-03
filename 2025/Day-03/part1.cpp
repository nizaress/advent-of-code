#include <iostream>
#include <fstream>
#include <stdexcept>
#include <vector>
#include <string>


std::vector<std::string> getBatteryBanks(std::string fileName){
    std::vector<std::string> banks;
    std::ifstream file(fileName);

    std::string singleBank{};
    if(file.is_open()){
        while(getline(file, singleBank))
            banks.push_back(singleBank);
        file.close();
    }
    else throw std::invalid_argument("Could not open the input file.");

    return banks;
}


int getJoltage(std::string bank){
    int biggestDigit{};
    for(int i=0; i < static_cast<int>(bank.length())-1; ++i){
        if(bank[i] > bank[biggestDigit]) biggestDigit = i;
    }

    int nextBiggest{biggestDigit+1};
    for(int i=biggestDigit+2; i < static_cast<int>(bank.length()); ++i){
        if(bank[i] > bank[nextBiggest]) nextBiggest = i;
    }

    return ((bank[biggestDigit]-'0')*10 + (bank[nextBiggest]-'0'));
}


int main(){

    std::vector<std::string> batteryBanks{ getBatteryBanks("input") };

    int result{};
    for(std::string bank : batteryBanks){
        result += getJoltage(bank);
    }

    std::cout << "The total output joltage is: " << result << "\n";
    return 0;
}