#include <iostream>
#include <fstream>
#include <vector>
#include <string>


void get_input(std::vector<std::vector<unsigned long>> &id_ranges, std::string file_name){
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
        file.close();
    }
}


// New logic for part 2
unsigned long get_num_ids(std::vector<std::vector<unsigned long>> &id_ranges){
    unsigned long sum_result{};
    for(std::vector<unsigned long> range : id_ranges) sum_result += range[1] - range[0] + 1;
    return sum_result;
}


// New logic for part 2
void merge_overlapping_ranges(std::vector<std::vector<unsigned long>> &id_ranges){
    bool totally_reduced{ false };
    while(!totally_reduced){
        totally_reduced = true;
        for(unsigned int i=0; i<static_cast<unsigned int>(id_ranges.size()); ++i){
            for(unsigned int j=0; j<static_cast<unsigned int>(id_ranges.size()); ++j){
                if(i==j) continue;
                if(id_ranges[i][0] >= id_ranges[j][0] && id_ranges[i][0] <= id_ranges[j][1]){
                    totally_reduced = false;
                    if(id_ranges[i][1] >= id_ranges[j][0] && id_ranges[i][1] <= id_ranges[j][1]){
                        id_ranges[i] = id_ranges.back();
                        id_ranges.pop_back();
                    }
                    else{
                        id_ranges[j][1] = id_ranges[i][1];
                        id_ranges[i] = id_ranges.back();
                        id_ranges.pop_back();
                    }
                }
                else if(id_ranges[i][1] >= id_ranges[j][0] && id_ranges[i][1] <= id_ranges[j][1]){
                    totally_reduced = false;
                    id_ranges[i][1] = id_ranges[j][1];
                    id_ranges[j] = id_ranges.back();
                    id_ranges.pop_back();
                }
                if(!totally_reduced) break;
            }
            if(!totally_reduced) break;
        }
    }
}


int main(){
    std::vector<std::vector<unsigned long>> id_ranges{};
    
    get_input(id_ranges, "input");
    merge_overlapping_ranges(id_ranges);

    std::cout << "The final count of fresh ingredients is: " << get_num_ids(id_ranges) << "\n";
    return 0;
}
