#include <iostream>
#include <fstream>
#include <vector>
#include <string>


unsigned long get_rectangle_area(std::vector<unsigned int> pos1, std::vector<unsigned int> pos2){
    unsigned long dx{ unsigned(std::abs(long(pos1[0]) - long(pos2[0]))) + 1 };
    unsigned long dy{ unsigned(std::abs(long(pos1[1]) - long(pos2[1]))) + 1 };
    return dx*dy;
}


unsigned long get_data(std::vector<std::vector<unsigned int>> &red_tiles, std::string file_name){
    std::string line{};
    std::ifstream file(file_name);

    if(file.is_open()){
    	while(getline(file,line)){
            std::vector<unsigned int> tile_position;
            int last_pos{};
            for(unsigned int i{1}; i<static_cast<unsigned int>(line.length()); ++i){
                if(line[i]==','){
                    tile_position.push_back(std::stoi(line.substr(last_pos,i-last_pos)));
                    last_pos = i+1;
                }
            }
            tile_position.push_back(std::stoi(line.substr(last_pos)));
            red_tiles.push_back(tile_position);
	    }
        file.close();
    }

    unsigned long biggest_area{};
    unsigned long current_area{};
    for(unsigned int i{}; i<static_cast<unsigned int>(red_tiles.size()); ++i){
        for(unsigned int j{i+1}; j<static_cast<unsigned int>(red_tiles.size()); ++j){
            current_area = get_rectangle_area(red_tiles[i], red_tiles[j]);
            if(current_area>biggest_area) biggest_area = current_area;
        }
    }
    return biggest_area;
}


int main(){
    std::vector<std::vector<unsigned int>> red_tiles;
    unsigned long result{ get_data(red_tiles, "input") };

    std::cout << "The final result of the operation is: " << result << "\n";
    return 0;
}
