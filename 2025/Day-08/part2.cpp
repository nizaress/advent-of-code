#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <queue>
#include <cmath>

struct BoxPair{
    unsigned int box1{};
    unsigned int box2{};
    double distance{ 0.0 };

    bool operator<(const BoxPair& other) const{
        return distance > other.distance;
    }
};

struct BoxPairComp{
    bool operator()(const BoxPair& pair1, const BoxPair& pair2) const{
        return pair1.distance > pair2.distance;
    }
};

unsigned long long result{}; // Part 2 output


double get_euclidean_distance(const std::vector<unsigned int>& pos1, const std::vector<unsigned int>& pos2){
    double dx = double(pos1[0]) - double(pos2[0]);
    double dy = double(pos1[1]) - double(pos2[1]);
    double dz = double(pos1[2]) - double(pos2[2]);
    return std::sqrt(dx*dx + dy*dy + dz*dz);
}


void get_data(std::vector<std::vector<unsigned int>> &junction_boxes, std::priority_queue<BoxPair, std::vector<BoxPair>, BoxPairComp> &box_pairs, std::string file_name){
    std::string line{};
    std::ifstream file(file_name);

    if(file.is_open()){
        while(getline(file, line)){
            std::vector<unsigned int> box_position;
            int last_pos = 0;
            for(unsigned int i = 1; i < line.length(); ++i){
                if(line[i] == ','){
                    box_position.push_back(std::stoi(line.substr(last_pos, i - last_pos)));
                    last_pos = i + 1;
                }
            }
            box_position.push_back(std::stoi(line.substr(last_pos)));
            junction_boxes.push_back(box_position);
        }
        file.close();
    }
    for(unsigned int i{}; i<junction_boxes.size(); ++i){
        for(unsigned int j{i+1}; j<junction_boxes.size(); ++j){
            BoxPair p;
            p.box1 = i;
            p.box2 = j;
            p.distance = get_euclidean_distance(junction_boxes[i], junction_boxes[j]);
            box_pairs.push(p);
        }
    }
}


unsigned int find_set(unsigned int v, std::vector<unsigned int>& parent){
    if(v == parent[v]) return v;
    return parent[v] = find_set(parent[v], parent);
}


bool union_sets(unsigned int a, unsigned int b, std::vector<unsigned int>& parent, std::vector<unsigned int>& size, unsigned int& components){
    a = find_set(a, parent);
    b = find_set(b, parent);

    if(a == b)
        return false;

    if(size[a] < size[b])
        std::swap(a, b);

    parent[b] = a;
    size[a] += size[b];
    components--;

    return true;
}


// New logic for part 2
void connect_box_circuits(std::vector<std::vector<unsigned int>>& junction_boxes, std::priority_queue<BoxPair, std::vector<BoxPair>, BoxPairComp>& box_pairs){
    unsigned int n = junction_boxes.size();
    std::vector<unsigned int> parent(n);
    std::vector<unsigned int> size(n, 1);
    for(unsigned int i{}; i<n; ++i)
        parent[i] = i;
    unsigned int components = n;

    while(!box_pairs.empty()){
        BoxPair p = box_pairs.top();
        box_pairs.pop();
        if(union_sets(p.box1, p.box2, parent, size, components)){
            if(components == 1){
                result = (unsigned long long)junction_boxes[p.box1][0] * junction_boxes[p.box2][0];
                return;
            }
        }
    }
}


int main(){
    std::vector<std::vector<unsigned int>> junction_boxes;
    std::priority_queue<BoxPair, std::vector<BoxPair>, BoxPairComp> box_pairs;

    get_data(junction_boxes, box_pairs, "input");
    connect_box_circuits(junction_boxes, box_pairs);

    std::cout << "The final result of the operation is: " << result << "\n";
    return 0;
}
