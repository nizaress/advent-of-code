#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <functional>


void get_input(std::vector<std::vector<char>> &char_grid, std::vector<char> &operations, std::string file_name){
    std::string line{};
    std::ifstream file(file_name);

    if(file.is_open()){
    	while(getline(file,line) && line[0]!='*' && line[0]!='+'){
            std::vector<char> number_line{};
            for(int i=0; i<static_cast<int>(line.length()); ++i)
                number_line.push_back(line[i]);
            char_grid.push_back(number_line);
	    }
        for(auto c : line){
            if(c!=' ')
                operations.push_back(c);
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


void compute_columns(std::vector<std::vector<char>> &operation_nums, std::vector<char> &operations, unsigned long &final_result){
    unsigned int operation_count{};
    unsigned long current_result{};
    std::function<unsigned long(unsigned long, unsigned long)> func_var;
    (operations[0]=='+') ? func_var = add : func_var = multiply;

    for(unsigned int i{}; i<static_cast<unsigned int>(operation_nums[0].size()); ++i){
        std::string current_number{};
        for(unsigned int j{}; j<static_cast<unsigned int>(operation_nums.size()); ++j){
            if(operation_nums[j][i]!=' ')
                current_number.push_back(operation_nums[j][i]);
        }
        if(current_number==""){
            switch(operations[++operation_count]){
                case '+': func_var = add; break;
                case '*': func_var = multiply; break;
            }
            final_result += current_result;
            current_result = 0;
            continue;
        }
        else{
            if(current_result==0) current_result = func_var(std::stoul(current_number), (operations[operation_count]=='+')?0:1);
            else current_result = func_var(current_result, std::stoul(current_number));
        }
    }
    final_result += current_result;
}


int main(){
    std::vector<std::vector<char>> char_grid{};
    std::vector<char> operations{};
    unsigned long final_result{};
    
    get_input(char_grid, operations, "input");
    compute_columns(char_grid, operations, final_result);

    std::cout << "The final sum of results is: " << final_result << "\n";
    return 0;
}
