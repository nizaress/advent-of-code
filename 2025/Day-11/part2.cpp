#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <tuple>


// Memory for the search
std::unordered_map<unsigned int, long long> memory_paths;


void get_data(std::vector<std::vector<unsigned int>> &connections, std::string file_name){
    std::string line{};
    std::ifstream file(file_name);

    unsigned long next_rack_id{ 4 };
    std::unordered_map<std::string, unsigned int> rack_ids;
    rack_ids.emplace("svr", 0);
    rack_ids.emplace("out", 1);
    rack_ids.emplace("fft", 2);
    rack_ids.emplace("dac", 3);

    if(file.is_open()){
    	while(getline(file,line)){
            unsigned int current_id{};
            unsigned int last_pos{};

            for(unsigned int i{}; i<line.size(); ++i){
                if(last_pos > i) continue;

                if(line[i]==':'){
                    if(rack_ids.find(line.substr(0,i)) != rack_ids.end())
                        current_id = rack_ids[line.substr(0,i)];
                    else{
                        rack_ids.emplace(line.substr(0,i), next_rack_id);
                        current_id = next_rack_id++;
                        connections.push_back(std::vector<unsigned int>());
                    }
                    last_pos = i + 2;
                }

                if(i==line.size()-1){
                    if(rack_ids.find(line.substr(last_pos, (i+1-last_pos))) != rack_ids.end()){
                        connections[current_id].push_back(rack_ids[line.substr(last_pos, (i+1-last_pos))]);
                    }
                    else{
                        if(line.substr(last_pos, (i+1-last_pos)) == "svr")
                            connections[current_id].push_back(0);
                        else if(line.substr(last_pos, (i+1-last_pos)) == "out")
                            connections[current_id].push_back(1);
                        else if(line.substr(last_pos, (i+1-last_pos)) == "fft")
                            connections[current_id].push_back(2);
                        else if(line.substr(last_pos, (i+1-last_pos)) == "dac")
                            connections[current_id].push_back(3);
                        else{
                            rack_ids.emplace(line.substr(last_pos, (i+1-last_pos)), next_rack_id);
                            connections[current_id].push_back(next_rack_id++);
                            connections.push_back(std::vector<unsigned int>());
                        }
                    }
                    break;
                }

                if(line[i]==' '){
                    if(rack_ids.find(line.substr(last_pos, (i-last_pos))) != rack_ids.end()){
                        connections[current_id].push_back(rack_ids[line.substr(last_pos, (i-last_pos))]);
                    }
                    else{
                        if(line.substr(last_pos, (i-last_pos)) == "svr")
                            connections[current_id].push_back(0);
                        else if(line.substr(last_pos, (i-last_pos)) == "out")
                            connections[current_id].push_back(1);
                        else if(line.substr(last_pos, (i-last_pos)) == "fft")
                            connections[current_id].push_back(2);
                        else if(line.substr(last_pos, (i-last_pos)) == "dac")
                            connections[current_id].push_back(3);
                        else{
                            rack_ids.emplace(line.substr(last_pos, (i-last_pos)), next_rack_id);
                            connections[current_id].push_back(next_rack_id++);
                            connections.push_back(std::vector<unsigned int>());
                        }
                    }
                    last_pos = i + 1;
                }
            }
	    }
        file.close();
    }
}


// New logic for part 2
long long get_every_path(unsigned int current_rack, bool fft, bool dac, const std::vector<std::vector<unsigned int>> &connections){

    if(current_rack==1){
        if(fft && dac) return 1;
        return 0;
    }
    //if(memory_paths[current_rack]!=-1) return memory_paths[current_rack];

    if(current_rack==2) fft = true;
    if(current_rack==3) dac = true;

    long long num_paths{};
    for(auto rack : connections[current_rack])
        num_paths += get_every_path(rack, fft, dac, connections);
    
    //memory_paths[current_rack] = num_paths;
    return num_paths;
}


int main(){
    std::vector<std::vector<unsigned int>> connections;
    connections.push_back(std::vector<unsigned int>());
    connections.push_back(std::vector<unsigned int>());
    connections.push_back(std::vector<unsigned int>());
    connections.push_back(std::vector<unsigned int>());
    get_data(connections, "test");

    for(unsigned int i{}; i<connections.size(); ++i)
        memory_paths.emplace(i,-1);

    long long result{ get_every_path(0, false, false, connections) };
    std::cout << "The final result of the operation is: " << result << "\n";
    return 0;
}
