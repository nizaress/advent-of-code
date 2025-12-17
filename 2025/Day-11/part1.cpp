#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <limits>


void get_data(std::vector<std::vector<unsigned int>> &connections, std::string file_name){
    std::string line{};
    std::ifstream file(file_name);

    unsigned long next_rack_id{ 2 };
    std::unordered_map<std::string, unsigned int> rack_ids;

    if(file.is_open()){
    	while(getline(file,line)){
            unsigned int current_id{};
            unsigned int last_pos{};

            for(unsigned int i{}; i<line.size(); ++i){
                std::cout << line[i] << std::flush;
                if(last_pos > i) continue;

                if(line[i]==':'){
                    if(line.substr(0,i) == "you") current_id = 0;
                    else current_id = next_rack_id++;
                    rack_ids.emplace(line.substr(0,i), current_id);
                    last_pos = i + 2;
                }

                if(i==line.size()-1){
                    if(rack_ids.find(line.substr(last_pos, (i-last_pos))) != rack_ids.end()){
                        connections[current_id].push_back(rack_ids[line.substr(last_pos, (i-last_pos))]);
                    }
                    else{
                        if(line.substr(last_pos, (i-last_pos)) == "you"){
                            rack_ids.emplace(line.substr(last_pos, (i-last_pos)), 0);
                            connections[current_id].push_back(0);
                        }
                        else if(line.substr(last_pos, (i-last_pos)) == "out"){
                            rack_ids.emplace(line.substr(last_pos, (i-last_pos)), 1);
                            connections[current_id].push_back(1);
                        }
                        else{
                            rack_ids.emplace(line.substr(last_pos, (i-last_pos)), next_rack_id);
                            connections[current_id].push_back(next_rack_id++);
                        }
                    }
                    break;
                }

                if(line[i]==' '){
                    if(rack_ids.find(line.substr(last_pos, (i-last_pos-1))) != rack_ids.end()){
                        connections[current_id].push_back(rack_ids[line.substr(last_pos, (i-last_pos-1))]);
                    }
                    else{
                        if(line.substr(last_pos, (i-last_pos-1)) == "you"){
                            rack_ids.emplace(line.substr(last_pos, (i-last_pos-1)), 0);
                            connections[current_id].push_back(0);
                        }
                        else if(line.substr(last_pos, (i-last_pos-1)) == "out"){
                            rack_ids.emplace(line.substr(last_pos, (i-last_pos-1)), 1);
                            connections[current_id].push_back(1);
                        }
                        else{
                            rack_ids.emplace(line.substr(last_pos, (i-last_pos-1)), next_rack_id);
                            connections[current_id].push_back(next_rack_id++);
                        }
                    }
                    last_pos = i + 1;
                }
            }
            std::cout << "\n";
	    }
        file.close();
    }
}


int main(){
    std::vector<std::vector<unsigned int>> connections;
    get_data(connections, "test");

    for(auto i{0u};i<connections.size();++i){
        std::cout << "In " << i << " there are:\n";
        for(auto j{0u}; j<connections[i].size(); ++j){
            std::cout << connections[i][j] << " ";
        }
        std::cout << "\n\n";
    }

    unsigned int result{};
    std::cout << "The final result of the operation is: " << result << "\n";
    return 0;
}
