#include <iostream>
#include <fstream>
#include <vector>
#include <string>


std::pair<unsigned int, unsigned int> get_input(std::vector<std::vector<bool>> &tachyon_manifold, std::vector<std::vector<bool>> &memory, std::string file_name){
    std::string line{};
    std::ifstream file(file_name);
    std::pair<unsigned int, unsigned int> start_position{};

    if(file.is_open()){
        unsigned int j{};
    	while(getline(file,line)){
            std::vector<bool> manifold_line{};
            std::vector<bool> memory_line{};
            for(unsigned int i{}; i<static_cast<unsigned int>(line.length()); ++i){
                memory_line.push_back(false);
                switch(line[i]){
                    case '.': manifold_line.push_back(false); break;
                    case 'S': start_position.second = j; start_position.second = i; manifold_line.push_back(false); break;
                    case '^': manifold_line.push_back(true); break;
                }
            }
            memory.push_back(memory_line);
            tachyon_manifold.push_back(manifold_line);
            ++j;
	    }
        file.close();
    }
    return start_position;
}


void get_splits(std::vector<std::vector<bool>> &tachyon_manifold, std::vector<std::vector<bool>> &memory, unsigned long &num_splits, std::pair<unsigned int, unsigned int> start_position){
    for(unsigned int i{ start_position.first }; i<static_cast<unsigned int>(tachyon_manifold.size()); ++i){
        if(memory[i][start_position.second]) return;
        else memory[i][start_position.second] = true;
        if(tachyon_manifold[i][start_position.second]){
            ++num_splits;
            get_splits(tachyon_manifold,memory,num_splits,{i,start_position.second-1});
            get_splits(tachyon_manifold,memory,num_splits,{i,start_position.second+1});
            return;
        }
    }
}


int main(){
    std::vector<std::vector<bool>> tachyon_manifold{};
    std::vector<std::vector<bool>> memory{};
    unsigned long num_splits{};
    
    std::pair<unsigned int, unsigned int> start_position{ get_input(tachyon_manifold, memory, "input") };
    get_splits(tachyon_manifold, memory, num_splits, start_position);

    std::cout << "The final sum of results is: " << num_splits << "\n";
    return 0;
}
