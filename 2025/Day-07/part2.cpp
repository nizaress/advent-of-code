#include <iostream>
#include <fstream>
#include <vector>
#include <string>


// New memory for the possible paths problem
std::vector<std::vector<long long>> memo;


std::pair<unsigned int, unsigned int> get_input(std::vector<std::vector<bool>> &tachyon_manifold, std::string file_name){
    std::string line{};
    std::ifstream file(file_name);
    std::pair<unsigned int, unsigned int> start_position{};

    if(file.is_open()){
        unsigned int j{};
        while(getline(file,line)){
            std::vector<bool> manifold_line{};
            for(unsigned int i{}; i < static_cast<unsigned int>(line.length()); ++i){
                switch(line[i]){
                    case '.': manifold_line.push_back(false); break;
                    case 'S':
                        start_position.first = j;
                        start_position.second = i;
                        manifold_line.push_back(false);
                        break;
                    case '^': manifold_line.push_back(true); break;
                }
            }
            tachyon_manifold.push_back(manifold_line);
            ++j;
        }
        file.close();
    }
    return start_position;
}


// New part 2 logic
unsigned long long count_quantum_paths(const std::vector<std::vector<bool>> &tachyon_manifold, unsigned int r, unsigned int c){
    if(memo[r][c]!=-1) return static_cast<unsigned long long>(memo[r][c]);
    for(unsigned int i = r; i < static_cast<unsigned int>(tachyon_manifold.size()); ++i){
        if(tachyon_manifold[i][c]){
            memo[r][c] = static_cast<long long>(count_quantum_paths(tachyon_manifold,i,c-1) + count_quantum_paths(tachyon_manifold,i,c+1));
            return memo[r][c];
        }
    }
    memo[r][c] = 1;
    return 1;
}


int main(){
    std::vector<std::vector<bool>> tachyon_manifold{};
    std::pair<unsigned int, unsigned int> start_position{ get_input(tachyon_manifold,"input") };
    memo.assign(static_cast<unsigned int>(tachyon_manifold.size()), std::vector<long long>(static_cast<unsigned int>(tachyon_manifold[0].size()), -1));

    std::cout << "The final number of quantum paths is: " << count_quantum_paths(tachyon_manifold, start_position.first, start_position.second) << "\n";
    return 0;
}
