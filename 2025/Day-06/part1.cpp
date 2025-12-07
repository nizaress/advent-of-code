#include <iostream>
#include <fstream>
#include <vector>
#include <string>


void get_input(std::vector<std::vector<unsigned long>> &operation_nums, std::vector<char> &operations, std::string file_name){
    std::string line{};
    std::ifstream file(file_name);

    if(file.is_open()){
    	while(getline(file,line) && line[0]!='*' && line[0]!='+'){
            std::vector<unsigned long> number_line{};
            int num_start{};
            for(int i=0; i<static_cast<int>(line.length()); ++i){
                if(line[num_start]==' ' && line[i]==' '){ ++num_start; continue; }
                if(line[i]==' '){
                    if(num_start<i)
                        number_line.push_back(std::stoul(line.substr(num_start,(i-num_start))));
                    num_start = i+1;
                }
                if(i==static_cast<int>(line.length()-1)){
                    if(num_start<i)
                        number_line.push_back(std::stoul(line.substr(num_start,(i+1-num_start))));
                }
            }
            operation_nums.push_back(number_line);
	    }

        for(auto c : line){
            if(c!=' ')
                operations.push_back(c);
        }
        file.close();
    }
}


void compute_columns(std::vector<std::vector<unsigned long>> &operation_nums, std::vector<char> &operations){
    for(unsigned int i{1}; i<static_cast<unsigned int>(operation_nums.size()); ++i){
        for(unsigned int j{}; j<static_cast<unsigned int>(operation_nums[0].size()); ++j){
            if(operations[j]=='*')
                operation_nums[0][j] *= operation_nums[i][j];
            else if(operations[j]=='+')
                operation_nums[0][j] += operation_nums[i][j];
        }
    }
}


int main(){
    std::vector<std::vector<unsigned long>> operation_nums{};
    std::vector<char> operations{};
    unsigned long final_result{};
    
    get_input(operation_nums, operations, "input");
    compute_columns(operation_nums, operations);

    for(unsigned long num : operation_nums[0]){
        final_result += num;
}
    std::cout << "The final sum of results is: " << final_result << "\n";
    return 0;
}
