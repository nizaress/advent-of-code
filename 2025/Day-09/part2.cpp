#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <limits>
#include <set>


unsigned int compressed_index(const std::vector<long long> &positions, long long value){
    auto it{ std::lower_bound(positions.begin(), positions.end(), value) };
    if(it==positions.end() || *it!=value) return static_cast<unsigned int>(-1);
    return static_cast<unsigned int>(it - positions.begin());
};


bool point_on_perimeter(const std::vector<std::pair<long long,long long>> &red_tiles, long long px, long long py){
    for(unsigned int i{}; i<static_cast<unsigned int>(red_tiles.size()); ++i){
        auto [x1,y1]{ red_tiles[i] };
        auto [x2,y2]{ red_tiles[(i+1) % red_tiles.size()] };
        if(x1==x2){
            if(px==x1 && py>=std::min(y1,y2) && py<=std::max(y1,y2)) return true;
        }else if(y1==y2){
            if(py==y1 && px>=std::min(x1,x2) && px<=std::max(x1,x2)) return true;
        }
    }
    return false;
};


bool point_in_polygon(const std::vector<std::pair<long long,long long>> &red_tiles, long long point_x, long long point_y){
    int count{};
    for(unsigned int i{}; i<static_cast<unsigned int>(red_tiles.size()); ++i){
        auto [x1,y1]{ red_tiles[i] };
        auto [x2,y2]{ red_tiles[(i+1)%red_tiles.size()] };
        if((y1>point_y)!=(y2>point_y)){
            double xint{ x1 + double(point_y-y1)*double(x2-x1) / double(y2-y1) };
            if(xint>double(point_x)) ++count;
        }
    }
    return (count%2)==1;
};


unsigned long long rect_sum(const std::vector<std::vector<unsigned long long>> &prefix_2d, unsigned int x1i, unsigned int y1i, unsigned int x2i, unsigned int y2i){
    return prefix_2d[y2i+1][x2i+1] - prefix_2d[y1i][x2i+1] - prefix_2d[y2i+1][x1i] + prefix_2d[y1i][x1i];
};


unsigned long compute_largest_rectangle(const std::vector<std::pair<long long,long long>> &red_tiles){
    std::set<long long> set_x, set_y;
    long long min_x{ std::numeric_limits<long long>::max() };
    long long max_x{ std::numeric_limits<long long>::min() };
    long long min_y{ std::numeric_limits<long long>::max() };
    long long max_y{ std::numeric_limits<long long>::min() };
    for(const auto &v : red_tiles){
        min_x = std::min(min_x, v.first);
        max_x = std::max(max_x, v.first);
        min_y = std::min(min_y, v.second);
        max_y = std::max(max_y, v.second);
        set_x.insert(v.first);
        set_x.insert(v.first + 1);
        set_y.insert(v.second);
        set_y.insert(v.second + 1);
    }
    set_x.insert(min_x);
    set_x.insert(max_x + 1);
    set_y.insert(min_y);
    set_y.insert(max_y + 1);

    std::vector<long long> x_vector_set(set_x.begin(), set_x.end());
    std::vector<long long> y_vector_set(set_y.begin(), set_y.end());
    const unsigned int num_x{ static_cast<unsigned int>(x_vector_set.size())-1 };
    const unsigned int num_y{ static_cast<unsigned int>(y_vector_set.size())-1 };
    
    std::vector<std::vector<unsigned int>> allowed(num_y, std::vector<unsigned int>(num_x, 0));
    std::vector<std::vector<unsigned long long>> weights(num_y, std::vector<unsigned long long>(num_x, 0));
    for(unsigned int i{}; i<num_y; ++i){
        for(unsigned int j{}; j<num_x; ++j){
            weights[i][j] = static_cast<unsigned long long>(x_vector_set[j+1] - x_vector_set[j]) * static_cast<unsigned long long>(y_vector_set[i+1] - y_vector_set[i]);
            bool valid{ false };
            if(point_on_perimeter(red_tiles,x_vector_set[j],y_vector_set[i])) valid = true;
            else if(point_in_polygon(red_tiles,x_vector_set[j],y_vector_set[i])) valid = true;
            allowed[i][j] = valid ? 1 : 0;
        }
    }

    std::vector<std::vector<unsigned long long>> prefix_2d(num_y+1, std::vector<unsigned long long>(num_x+1,0));
    for(unsigned int i{}; i<num_y; ++i){
        unsigned long long row_sum{};
        for(unsigned int j{}; j<num_x; ++j){
            if(allowed[i][j]) row_sum += weights[i][j];
            prefix_2d[i+1][j+1] = prefix_2d[i][j+1] + row_sum;
        }
    }

    std::vector<unsigned int> compressed_grid_x(red_tiles.size()), compressed_grid_y(red_tiles.size());
    for(unsigned int i{}; i<red_tiles.size(); ++i){
        long long x{ red_tiles[i].first };
        long long y{ red_tiles[i].second };
        unsigned int new_x{ compressed_index(x_vector_set, x) };
        unsigned int new_y{ compressed_index(y_vector_set, y) };
        if(new_x==static_cast<unsigned int>(-1) || new_y==static_cast<unsigned int>(-1)) return 0;
        compressed_grid_x[i] = new_x;
        compressed_grid_y[i] = new_y;
    }

    unsigned long long best{};
    for(unsigned int i{}; i<red_tiles.size(); ++i){
        for(unsigned int j{i+1}; j<red_tiles.size(); ++j){
            unsigned int x1_index{ std::min(compressed_grid_x[i], compressed_grid_x[j]) };
            unsigned int x2_index{ std::max(compressed_grid_x[i], compressed_grid_x[j]) };
            unsigned int y1_index{ std::min(compressed_grid_y[i], compressed_grid_y[j])};
            unsigned int y2_index{ std::max(compressed_grid_y[i], compressed_grid_y[j]) };

            long long x1{ red_tiles[i].first };
            long long x2{ red_tiles[j].first };
            long long y1{ red_tiles[i].second };
            long long y2{ red_tiles[j].second };
            long long real_x1{ std::min(x1, x2) };
            long long real_x2{ std::max(x1, x2) };
            long long real_y1{ std::min(y1, y2) };
            long long real_y2{ std::max(y1, y2) };

            unsigned long long area_expected{ static_cast<unsigned long long>(real_x2-real_x1+1) * static_cast<unsigned long long>(real_y2-real_y1+1) };
            unsigned long long sum_allowed{ rect_sum(prefix_2d, x1_index, y1_index, x2_index, y2_index) };
            if(sum_allowed==area_expected && sum_allowed>best) best = sum_allowed;
        }
    }

    return best;
}


unsigned long get_data(std::vector<std::pair<long long,long long>> &red_tiles, std::string file_name){
    std::ifstream file(file_name);
    std::string line;
    if(file.is_open()){
        while(std::getline(file,line)){
            long long tile{};
            int last{};
            for(unsigned int i{1};i<static_cast<unsigned int>(line.length());++i){
                if(line[i]==','){
                    tile = std::stoll(line.substr(last, i - last));
                    last = i + 1;
                }
            }
            red_tiles.emplace_back(tile, std::stoll(line.substr(last)));
        }
        file.close();
    }
    return compute_largest_rectangle(red_tiles);
}


int main(){
    std::vector<std::pair<long long,long long>> red_tiles;
    unsigned long result{ get_data(red_tiles, "input") };

    std::cout << "The final result of the operation is: " << result << "\n";
    return 0;
}
