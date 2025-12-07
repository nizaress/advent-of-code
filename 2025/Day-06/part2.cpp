#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <functional>


void get_input(std::vector<std::vector<std::string>> &char_grid, std::string file_name){
    std::string line{};
    std::ifstream file(file_name);

    if(file.is_open()){
    	while(getline(file,line)){
            std::vector<std::string> number_line{};
            for(int i=0; i<static_cast<int>(line.length()); ++i)
                number_line.push_back({line[i]});
            char_grid.push_back(number_line);
	    }
        file.close();
    }
}


unsigned long add(unsigned long num1=0, unsigned long num2=0){
    return num1 + num2;
}


unsigned long multiply(unsigned long num1=1, unsigned long num2=1){
    return num1 * num2;
}


void compute_columns(std::vector<std::vector<std::string>> &operation_nums){
    //std::vector<> 
    std::function<unsigned long(unsigned long, unsigned long)> func_var;
    for(unsigned int i{}; i<static_cast<unsigned int>(operation_nums[0].size()); ++i){
        func_var
        for(unsigned int j{}; j<static_cast<unsigned int>(operation_nums.size())-1; ++j){
            operation_nums[j][i]
        }
        if(i!=(static_cast<unsigned int>(operation_nums[0].size())-1) && )
    }
}


int main(){
    std::vector<std::vector<std::string>> char_grid{};
    std::vector<unsigned long> operated_columns{};
    unsigned long final_result{};
    
    get_input(char_grid, "test");
    compute_columns(char_grid);

    std::cout << "The final sum of results is: " << final_result << "\n";
    return 0;
}
