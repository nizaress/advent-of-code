#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <set>
#include <algorithm>


unsigned long long search_iterations{};
const unsigned long long MAX_ITERATIONS = 1000;


void get_input_data(std::vector<std::vector<std::vector<bool>>> &present_shapes,
                    std::vector<std::tuple<unsigned int, unsigned int, std::vector<unsigned int>>> &tree_regions, const std::string &file_name){
    std::ifstream file(file_name);
    if(file.is_open()){
        std::string line;
        std::vector<std::vector<bool>> present;
        while(getline(file,line)){
            if(line.size()==0 || line[1] == ':'){
                if(present.size()>0){
                    present_shapes.push_back(present);
                    present.clear();
                }
                continue;
            }
            if(line[0]=='#' or line[0]=='.'){
                std::vector<bool> present_line;
                for(auto i{0u}; i<line.size(); ++i){
                    switch(line[i]){
                        case '#': present_line.push_back(true); break;
                        case '.': present_line.push_back(false); break;
                    }
                }
                present.push_back(present_line);
            }
            else{
                std::tuple<unsigned int, unsigned int, std::vector<unsigned int>> region;
                unsigned int last_pos{};
                unsigned int i{};
                for(; i<line.size(); ++i){
                    if(line[i]=='x'){
                        std::get<0>(region) = std::stoi(line.substr(last_pos, i-last_pos));
                        last_pos = i+1;
                        ++i;
                        break;
                    }
                }
                for(; i<line.size(); ++i){
                    if(line[i]==':'){
                        std::get<1>(region) = std::stoi(line.substr(last_pos, i-last_pos));
                        last_pos = i+2;
                        i = last_pos;
                        break;
                    }
                }
                for(; i<line.size(); ++i){
                    if(line[i]==' '){
                        if(i > last_pos) std::get<2>(region).push_back(std::stoi(line.substr(last_pos, i-last_pos)));
                        last_pos = i+1;
                    }
                }
                if(last_pos<line.size()) std::get<2>(region).push_back(std::stoi(line.substr(last_pos, line.size()-last_pos)));
                tree_regions.push_back(region);
            }
        }
        file.close();
    }
}


std::vector<std::pair<int,int>> normalize_shape_coords(std::vector<std::pair<int,int>> coords){
    if(coords.empty()) return coords;
    int min_r = coords[0].first, min_c = coords[0].second;
    for(auto &p : coords){
        min_r = std::min(min_r, p.first);
        min_c = std::min(min_c, p.second);
    }
    for(auto &p : coords){
        p.first -= min_r;
        p.second -= min_c;
    }
    std::sort(coords.begin(), coords.end());
    return coords;
}


std::vector<std::pair<int,int>> normalize_shape(const std::vector<std::vector<bool>> &shape){
    std::vector<std::pair<int,int>> coords;
    for(unsigned int r{}; r<shape.size(); ++r){
        for(unsigned int c{}; c<shape[r].size(); ++c){
            if(shape[r][c]){
                coords.push_back({static_cast<int>(r), static_cast<int>(c)});
            }
        }
    }
    if(coords.empty()) return coords;

    int min_r = coords[0].first, min_c = coords[0].second;
    for(auto &p : coords){
        min_r = std::min(min_r, p.first);
        min_c = std::min(min_c, p.second);
    }
    for(auto &p : coords){
        p.first -= min_r;
        p.second -= min_c;
    }
    std::sort(coords.begin(), coords.end());
    return coords;
}


std::vector<std::vector<std::pair<int,int>>> get_all_orientations(const std::vector<std::vector<bool>> &shape){
    std::set<std::vector<std::pair<int,int>>> unique_orientations;
    auto coords = normalize_shape(shape);
    for(int flip{}; flip<2; ++flip){
        for(int rot{}; rot<4; ++rot){
            std::vector<std::pair<int,int>> oriented = coords;
            for(int i{}; i<rot; ++i){
                for(auto &p : oriented){
                    int temp = p.first;
                    p.first = -p.second;
                    p.second = temp;
                }
            }
            if(flip){
                for(auto &p : oriented)
                    p.second = -p.second;
            }
            unique_orientations.insert(normalize_shape_coords(oriented));
        }
    }
    return std::vector<std::vector<std::pair<int,int>>>(unique_orientations.begin(), unique_orientations.end());
}


bool can_place(const std::vector<std::vector<bool>> &grid, const std::vector<std::pair<int,int>> &shape, int r, int c){
    for(auto &offset : shape){
        int nr = r+offset.first;
        int nc = c+offset.second;
        if(nr<0 || static_cast<unsigned int>(nr) >= grid.size() || 
           nc<0 || static_cast<unsigned int>(nc) >= grid[0].size() || 
           grid[nr][nc]){
            return false;
        }
    }
    return true;
}


void toggle_shape(std::vector<std::vector<bool>> &grid, const std::vector<std::pair<int,int>> &shape, int r, int c, bool place){
    for(auto &offset : shape)
        grid[r+offset.first][c+offset.second] = place;
}


bool fit_presents(std::vector<std::vector<bool>> &grid, const std::vector<std::vector<std::vector<std::pair<int,int>>>> &all_orientations, std::vector<int> &remaining){
    if(++search_iterations > MAX_ITERATIONS) return false;
    int present_idx = -1;
    for(unsigned int i{}; i<remaining.size(); ++i){
        if(remaining[i] > 0){
            present_idx = static_cast<int>(i);
            break;
        }
    }
    if(present_idx == -1) return true;
    
    int height = static_cast<int>(grid.size());
    int width = static_cast<int>(grid[0].size());
    for(int r{}; r<height; ++r){
        for(int c{}; c<width; ++c){
            for(auto &orientation : all_orientations[present_idx]){
                if(can_place(grid, orientation, r, c)){
                    toggle_shape(grid, orientation, r, c, true);
                    --remaining[present_idx];
                    if(fit_presents(grid, all_orientations, remaining)) return true;
                    toggle_shape(grid, orientation, r, c, false);
                    ++remaining[present_idx];
                }
            }
        }
    }
    return false;
}


unsigned short presents_fit(const std::vector<std::vector<std::vector<bool>>> &present_shapes, unsigned int width, unsigned int height, const std::vector<unsigned int> &needed_presents){
    std::vector<std::vector<bool>> grid(height, std::vector<bool>(width, false));
    std::vector<std::vector<std::vector<std::pair<int,int>>>> all_orientations;
    for(auto &shape : present_shapes)
        all_orientations.push_back(get_all_orientations(shape));
    std::vector<int> remaining(needed_presents.begin(), needed_presents.end());

    search_iterations = 0;
    bool result = fit_presents(grid, all_orientations, remaining);
    return result ? 1 : 0;
}


int main(){
    std::vector<std::vector<std::vector<bool>>> present_shapes;
    std::vector<std::tuple<unsigned int, unsigned int, std::vector<unsigned int>>> tree_regions;
    get_input_data(present_shapes, tree_regions, "input");
    
    unsigned int result{};
    for(unsigned int i{}; i<tree_regions.size(); ++i){
        auto fits = presents_fit(present_shapes, std::get<0>(tree_regions[i]), std::get<1>(tree_regions[i]), std::get<2>(tree_regions[i]));
        std::cout << "Region " << i << " (" << std::get<0>(tree_regions[i]) << "x" << std::get<1>(tree_regions[i]) << "): " << (fits ? "FITS" : "DOES NOT FIT") << std::endl;
        result += fits;
    }
    
    std::cout << "The number of valid regions is: " << result << std::endl;
    return 0;
}