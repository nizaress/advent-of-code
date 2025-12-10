#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <limits>

using State = std::string;


unsigned long get_min_pushes_rec(State state, unsigned long pushes, const State &goal, const std::vector<std::vector<unsigned short>> &buttons, std::unordered_map<State, unsigned long> &memory){
    if(state==goal) return pushes;

    auto prev_res = memory.find(state);
    if(prev_res!=memory.end() && prev_res->second<=pushes)
        return std::numeric_limits<unsigned long>::max();
    memory[state] = pushes;

    unsigned long min_pushes = std::numeric_limits<unsigned long>::max();
    for(const auto &button : buttons){
        for(unsigned short light_index : button) state[light_index] ^= 1;

        unsigned long result = get_min_pushes_rec(state, pushes + 1, goal, buttons, memory);
        if(result<min_pushes) min_pushes = result;

        for(unsigned short light_index : button) state[light_index] ^= 1;
    }
    return min_pushes;
}


unsigned long get_min_pushes(const State &goal, const std::vector<std::vector<unsigned short>> &buttons){
    std::unordered_map<State, unsigned long> memory;
    State init_state(goal.size(), (char)0);
    return get_min_pushes_rec(init_state, 0, goal, buttons, memory);
}


void get_data(std::vector<State> &indicator_lights, std::vector<std::vector<std::vector<unsigned short>>> &buttons, std::string file_name){
    std::string line{};
    std::ifstream file(file_name);

    if(file.is_open()){
    	while(getline(file,line)){
            unsigned int i{1};
            State indicator;
            for(; line[i]!=']'; ++i){
                if(line[i]=='.') indicator.push_back(0);
                else indicator.push_back(1);
            }
            indicator_lights.push_back(indicator);

            std::vector<std::vector<unsigned short>> button;
            std::vector<unsigned short> affected_lights;
            unsigned int last_pos{};
            for(; line[i]!='{'; ++i){
                if(line[i]=='(') last_pos = i+1;
                if(line[i]==','){
                    affected_lights.push_back(std::stoi(line.substr(last_pos,i-last_pos)));
                    last_pos = i+1;
                }
                if(line[i]==')'){
                    affected_lights.push_back(std::stoi(line.substr(last_pos,i-last_pos)));
                    button.push_back(affected_lights);
                    affected_lights.clear();
                }
            }
            buttons.push_back(button);
	    }
        file.close();
    }
}


int main(){
    std::vector<State> indicator_lights;
    std::vector<std::vector<std::vector<unsigned short>>> buttons;
    get_data(indicator_lights, buttons, "input");

    unsigned long result{};
    for(auto i{0u}; i<indicator_lights.size(); ++i){
        result += get_min_pushes(indicator_lights[i], buttons[i]);
    }

    std::cout << "The final result of the operation is: " << result << "\n";
    return 0;
}
