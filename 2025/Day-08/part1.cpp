#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <queue>
#include <cmath>
#include <unordered_map>


struct BoxPair{
    unsigned int box1;
    unsigned int box2;
    double distance{ 0.0 };
    unsigned int circuit{ 0 };

    bool operator<(const BoxPair& other) const{
        return distance < other.distance;
    }
};

struct BoxPairComp{
    bool operator()(const BoxPair& pair1, const BoxPair& pair2) const{
        return pair1.distance > pair2.distance;
    }
};

unsigned int next_circuit_id{ 1 };


double get_euclidean_distance(std::vector<unsigned int> pos1, std::vector<unsigned int> pos2){
    double dx = double(pos1[0]) - double(pos2[0]);
    double dy = double(pos1[1]) - double(pos2[1]);
    double dz = double(pos1[2]) - double(pos2[2]);
    return std::sqrt(dx*dx + dy*dy + dz*dz);
}


void get_data(std::vector<std::vector<unsigned int>> &junction_boxes, std::priority_queue<BoxPair, std::vector<BoxPair>, BoxPairComp> &box_pairs, std::string file_name){
    std::string line{};
    std::ifstream file(file_name);

    if(file.is_open()){
    	while(getline(file,line)){
            std::vector<unsigned int> box_position;
            int last_pos{};
            for(unsigned int i{1}; i<static_cast<unsigned int>(line.length()); ++i){
                if(line[i]==','){
                    box_position.push_back(std::stoi(line.substr(last_pos,i-last_pos)));
                    last_pos = i+1;
                }
            }
            box_position.push_back(std::stoi(line.substr(last_pos)));
            box_position.push_back(0);
            junction_boxes.push_back(box_position);
	    }
        file.close();
    }

    for(unsigned int i{}; i<static_cast<unsigned int>(junction_boxes.size()); ++i){
        for(unsigned int j{i+1}; j<static_cast<unsigned int>(junction_boxes.size()); ++j){
            BoxPair p;
            p.box1 = i;
            p.box2 = j;
            p.distance = get_euclidean_distance(junction_boxes[i], junction_boxes[j]);
            box_pairs.push(p);
        }
    }
}


void join_circuits(std::vector<std::vector<unsigned int>> &junction_boxes, std::unordered_map<unsigned int, unsigned int> &circuits, unsigned int deleted_circuit, unsigned int preserved_circuit){
    circuits[preserved_circuit] += circuits[deleted_circuit];
    circuits.erase(deleted_circuit);
    for(unsigned int i{}; i<static_cast<unsigned int>(junction_boxes.size()); ++i){
        if(junction_boxes[i][3] == deleted_circuit) junction_boxes[i][3] = preserved_circuit;
    }
}


void print_pair(std::vector<std::vector<unsigned int>> &junction_boxes, BoxPair pair){
    std::cout << "Current pair:\n";
    std::cout << "Box 1: (" << junction_boxes[pair.box1][0] << "," << junction_boxes[pair.box1][1] << "," << junction_boxes[pair.box1][2] << ") in circuit " << junction_boxes[pair.box1][3] << "\n";
    std::cout << "Box 2: (" << junction_boxes[pair.box2][0] << "," << junction_boxes[pair.box2][1] << "," << junction_boxes[pair.box2][2] << ") in circuit " << junction_boxes[pair.box2][3] << "\n\n";
}


void connect_box_circuits(std::vector<std::vector<unsigned int>> &junction_boxes, std::priority_queue<BoxPair, std::vector<BoxPair>, BoxPairComp> &box_pairs, std::unordered_map<unsigned int, unsigned int> &circuits, unsigned int num_connections){
    for(unsigned int i{}; i<num_connections; ++i){
        if(box_pairs.empty()) return;
        BoxPair pair = box_pairs.top();
        //print_pair(junction_boxes, pair);

        if(junction_boxes[pair.box1][3]==0 && junction_boxes[pair.box2][3]==0){
            junction_boxes[pair.box1][3] = junction_boxes[pair.box2][3] = next_circuit_id;
            circuits.emplace(next_circuit_id, 2);
            next_circuit_id++;
        }
        else if(junction_boxes[pair.box1][3]!=0 && junction_boxes[pair.box2][3]==0){
            junction_boxes[pair.box2][3] = junction_boxes[pair.box1][3];
            ++circuits[junction_boxes[pair.box1][3]];
        }
        else if(junction_boxes[pair.box1][3]==0 && junction_boxes[pair.box2][3]!=0){
            junction_boxes[pair.box1][3] = junction_boxes[pair.box2][3];
            ++circuits[junction_boxes[pair.box2][3]];
        }
        else{
            if(junction_boxes[pair.box1][3]==junction_boxes[pair.box2][3]){ box_pairs.pop(); continue;}
            if(circuits[junction_boxes[pair.box2][3]] < circuits[junction_boxes[pair.box1][3]]) join_circuits(junction_boxes, circuits, junction_boxes[pair.box2][3], junction_boxes[pair.box1][3]);
            else join_circuits(junction_boxes, circuits, junction_boxes[pair.box1][3], junction_boxes[pair.box2][3]);
        }
        box_pairs.pop();
    }
}


int main(){
    std::vector<std::vector<unsigned int>> junction_boxes;
    std::priority_queue<BoxPair, std::vector<BoxPair>, BoxPairComp> box_pairs;
    std::unordered_map<unsigned int, unsigned int> circuits;
    std::priority_queue<unsigned int> largest_circuits;

    get_data(junction_boxes, box_pairs, "input");
    connect_box_circuits(junction_boxes, box_pairs, circuits, 1000);

    for(auto circuit : circuits) largest_circuits.push(circuit.second);
    unsigned long result{1};
    for(unsigned int i{};i<3;++i){
        if(largest_circuits.empty()) continue;
        result *= largest_circuits.top();
        largest_circuits.pop();
    }

    std::cout << "The final result of the operation is: " << result << "\n";
    return 0;
}
