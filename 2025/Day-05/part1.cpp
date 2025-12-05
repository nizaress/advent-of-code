#include <iostream>
#include <fstream>
#include <vector>
#include <string>


void get_input(std::vector<std::vector<unsigned long>> &id_ranges, std::vector<unsigned long> &ingredient_ids, std::string file_name){
    std::string line{};
    std::ifstream file(file_name);
    if(file.is_open()){
    	while(getline(file,line) && line!=""){
            std::vector<unsigned long> range{};
            for(int i=0; i<static_cast<int>(line.length()); ++i){

                if(line[i]=='-'){
                    range.push_back(std::stoul(line.substr(0,i)));
                    range.push_back(std::stoul(line.substr(i+1)));
                    break;
                }
            }
            id_ranges.push_back(range);
	    }

        while(getline(file,line)){
            ingredient_ids.push_back(std::stoul(line));
	    }
        file.close();
    }
}


bool check_in_range(unsigned long id, std::vector<std::vector<unsigned long>> &id_ranges){
    for(std::vector<unsigned long> range : id_ranges){
        if(id>=range[0] && id<=range[1]) return true;
    }
    return false;
}


int main(){
    std::vector<std::vector<unsigned long>> id_ranges{};
    std::vector<unsigned long> ingredient_ids{};
    unsigned int fresh_ingredient_count{};
    
    get_input(id_ranges, ingredient_ids, "input");

    for(unsigned long id : ingredient_ids){
        if(check_in_range(id, id_ranges)) ++fresh_ingredient_count;
    }

    std::cout << "The final count of fresh ingredients is: " << fresh_ingredient_count << "\n";
    return 0;
}
