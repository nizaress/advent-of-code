#include <iostream>
#include <fstream>
#include <stdexcept>
#include <vector>
#include <string>


std::vector<std::vector<long>> getIdRanges(std::string fileName){
    std::vector<std::vector<long>> idRanges{};
    std::ifstream file(fileName);
    std::string inputString{};

    if(file.is_open()){
        getline(file, inputString);
        file.close();
    }
    else throw std::invalid_argument("Could not read the file.");

    std::vector<long> currentRange{};
    int numStart{};

    for(int i=0; i < static_cast<int>(inputString.length()); ++i){
        if(inputString[i] == ',' || inputString[i] == '-'){
            currentRange.push_back(std::stol(inputString.substr(numStart, i-numStart)));
            numStart = i+1;

            if(inputString[i] == ','){
                idRanges.push_back(currentRange);
                currentRange.clear();
            }
        }
        if(i == static_cast<int>(inputString.length()) - 1){
            currentRange.push_back(std::stol(inputString.substr(numStart, i+1-numStart)));
            idRanges.push_back(currentRange);
        }
    }

    return idRanges;
}


bool checkInvalidPattern(long id){
    std::string number{ std::to_string(id) };
    std::string pattern{};

    // Changed logic for part 2
    for(int i=1; i < static_cast<int>(number.length()); ++i){
        if(pattern != ""){
            if(number[i] != pattern[i % static_cast<int>(pattern.length())])
                pattern = "";
        }
        if(pattern == "" && number[i]==number[0] && (static_cast<int>(number.length()) % i == 0)) pattern = number.substr(0, i);
    }

    if(pattern != "") return true;
    return false;
}


long sumInvalidIDs(std::vector<std::vector<long>> idRanges){
    long long result{};
    for(std::vector range : idRanges){
        for(long i=range[0]; i<=range[1]; ++i){
            if(checkInvalidPattern(i)) result += i;
        }
    }
    return result;
}


int main(){
    std::vector<std::vector<long>> idRanges{ getIdRanges("input")};
    long long result{ sumInvalidIDs(idRanges) };

    std::cout << "The resulting sum of invalid IDs is: " << result << "\n";
    return 0;
}