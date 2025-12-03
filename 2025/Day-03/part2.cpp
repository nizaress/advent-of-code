#include <iostream>
#include <fstream>
#include <stdexcept>
#include <vector>
#include <string>
#include <cmath> // Added for part 2


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


// New recursive solution for part 2
unsigned long getJoltage(std::string bank, short missingNumbers){
    int biggestDigit{};
    for(int i=0; i < static_cast<int>(bank.length())-(missingNumbers-1); ++i){
        if(bank[i] > bank[biggestDigit]) biggestDigit = i;
    }
    
    if(--missingNumbers == 0)
        return static_cast<unsigned long>(bank[biggestDigit]-'0');
    else{
        unsigned long nextResult{ getJoltage(bank.substr(biggestDigit+1), missingNumbers) };
        unsigned long power{ static_cast<unsigned long>(std::log10(nextResult)) };
        return (static_cast<unsigned long>(bank[biggestDigit]-'0') * (std::pow(10ul, power+1ul)) + nextResult);
    }
}


int main(){

    std::vector<std::string> batteryBanks{ getBatteryBanks("input") };

    unsigned long result{};
    for(std::string bank : batteryBanks){
        result += getJoltage(bank, 12);
    }

    std::cout << "The total output joltage is: " << result << "\n";
    return 0;
}
