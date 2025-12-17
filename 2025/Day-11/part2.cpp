#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <climits>


long long gcd(long long num_a, long long num_b){
    num_a = abs(num_a);
    num_b = abs(num_b);
    while(num_b){
        long long temp{ num_b };
        num_b = num_a%num_b;
        num_a = temp;
    }
    return num_a;
}


void simplify(std::pair<long long, long long>& frac){
    if(frac.first==0){
        frac.second = 1;
        return;
    }
    long long divisor{ gcd(frac.first, frac.second) };
    frac.first /= divisor;
    frac.second /= divisor;
    if(frac.second<0){
        frac.first = -frac.first;
        frac.second = -frac.second;
    }
}


std::pair<long long, long long> add(std::pair<long long, long long> frac_a, std::pair<long long, long long> frac_b){
    std::pair<long long, long long> result ={frac_a.first*frac_b.second + frac_b.first*frac_a.second, frac_a.second*frac_b.second};
    simplify(result);
    return result;
}


std::pair<long long, long long> mul(std::pair<long long, long long> frac_a, std::pair<long long, long long> frac_b){
    std::pair<long long, long long> result ={frac_a.first*frac_b.first, frac_a.second*frac_b.second};
    simplify(result);
    return result;
}


std::pair<long long, long long> divide(std::pair<long long, long long> frac_a, std::pair<long long, long long> frac_b){
    if(frac_b.first==0) return std::make_pair(LLONG_MAX, 1LL);
    std::pair<long long, long long> result ={frac_a.first*frac_b.second, frac_a.second*frac_b.first};
    simplify(result);
    return result;
}


struct SearchContext{
    std::vector<std::vector<std::pair<long long, long long>>>& aug;
    std::vector<int>& pivot_col;
    std::vector<int>& free_vars;
    int n, m, rank;
    long long limit;
    long long& best;
};


void search_recursive(int free_idx, std::vector<long long>& free_vals, SearchContext& ctx){
    if(free_idx==(int)ctx.free_vars.size()){
        std::vector<long long> solution(ctx.m, 0);
        for(size_t i{}; i<ctx.free_vars.size(); ++i){
            solution[ctx.free_vars[i]] = free_vals[i];
        }
        
        bool is_valid{ true };
        for(int row_idx{}; row_idx<ctx.rank; ++row_idx){
            int col_idx{ ctx.pivot_col[row_idx] };
            auto current_val{ ctx.aug[row_idx][ctx.m] };
            for(int j{}; j<ctx.m; ++j){
                if(j!=col_idx){
                    auto temp_val{ mul(ctx.aug[row_idx][j],{solution[j], 1}) };
                    current_val = add(current_val,{-temp_val.first, temp_val.second});
                }
            }
            if(current_val.second!=1){
                is_valid = false;
                break;
            }
            if(current_val.first<0){
                is_valid = false;
                break;
            }
            
            solution[col_idx] = current_val.first;
        }
        
        if(is_valid){
            long long total_presses{};
            for(long long press_count : solution) total_presses += press_count;
            ctx.best = std::min(ctx.best, total_presses);
        }
        return;
    }
    
    for(long long val{}; val<=ctx.limit; ++val){
        free_vals.push_back(val);
        search_recursive(free_idx+1, free_vals, ctx);
        free_vals.pop_back();
        
        if(ctx.best<LLONG_MAX){
            long long partial_sum{};
            for(long long v : free_vals) partial_sum += v;
            if(partial_sum>=ctx.best) break;
        }
    }
}


long long get_min_pushes(const std::vector<long long>& goal, const std::vector<std::vector<int>>& buttons){
    int num_counters{ static_cast<int>(goal.size()) };
    int num_buttons{ static_cast<int>(buttons.size()) };
    
    if(num_buttons==0) return -1;
    
    std::vector<std::vector<std::pair<long long, long long>>> augmented_matrix(num_counters, std::vector<std::pair<long long, long long>>(num_buttons+1));
    
    for(int counter_idx{}; counter_idx<num_counters; ++counter_idx){
        for(int button_idx{}; button_idx<num_buttons; ++button_idx){
            augmented_matrix[counter_idx][button_idx] ={0, 1};
        }
        augmented_matrix[counter_idx][num_buttons] ={goal[counter_idx], 1};
    }
    
    for(int btn_idx{}; btn_idx<num_buttons; ++btn_idx){
        for(int light : buttons[btn_idx]){
            augmented_matrix[light][btn_idx] ={1, 1};
        }
    }
    
    std::vector<int> pivot_col(num_counters, -1);
    int rank{};
    
    for(int col{}; col<num_buttons && rank<num_counters; ++col){
        int pivot_row{ -1 };
        for(int row{rank}; row<num_counters; ++row){
            if(augmented_matrix[row][col].first!=0){
                pivot_row = row;
                break;
            }
        }
        
        if(pivot_row==-1) continue;
        
        std::swap(augmented_matrix[rank], augmented_matrix[pivot_row]);
        pivot_col[rank] = col;
        
        auto pivot_val{ augmented_matrix[rank][col] };
        for(int c{}; c<=num_buttons; ++c){
            augmented_matrix[rank][c] = divide(augmented_matrix[rank][c], pivot_val);
        }
        
        for(int row{}; row<num_counters; ++row){
            if(row==rank) continue;
            if(augmented_matrix[row][col].first==0) continue;
            
            auto factor{ augmented_matrix[row][col] };
            for(int c{}; c<=num_buttons; ++c){
                auto temp{ mul(augmented_matrix[rank][c], factor) };
                augmented_matrix[row][c] = add(augmented_matrix[row][c],{-temp.first, temp.second});
            }
        }
        
        ++rank;
    }
    
    for(int row{rank}; row<num_counters; ++row){
        if(augmented_matrix[row][num_buttons].first!=0){
            return -1;
        }
    }
    
    std::vector<bool> is_basic(num_buttons, false);
    for(int row{}; row<rank; ++row){
        if(pivot_col[row]!=-1){
            is_basic[pivot_col[row]] = true;
        }
    }
    
    std::vector<int> free_vars;
    for(int col{}; col<num_buttons; ++col){
        if(!is_basic[col]){
            free_vars.push_back(col);
        }
    }
    
    if(free_vars.empty()){
        std::vector<long long> solution(num_buttons, 0);
        for(int row{}; row<rank; ++row){
            int col{ pivot_col[row] };
            auto x_val{ augmented_matrix[row][num_buttons] };
            if(x_val.second!=1 || x_val.first<0){
                return -1;
            }
            solution[col] = x_val.first;
        }
        long long total{};
        for(long long v : solution) total += v;
        return total;
    }
    
    long long best_result{ LLONG_MAX };
    
    long long upper_limit{};
    for(long long g : goal) upper_limit += g;
    
    SearchContext ctx ={augmented_matrix, pivot_col, free_vars, num_counters, num_buttons, rank, upper_limit, best_result};
    std::vector<long long> free_vals;
    search_recursive(0, free_vals, ctx);
    
    return (ctx.best==LLONG_MAX) ? -1 : ctx.best;
}


void get_data(std::vector<std::vector<std::vector<int>>>& buttons, std::vector<std::vector<long long>>& joltage_levels, std::string file_name){
    std::string line;
    std::ifstream file(file_name);

    if(file.is_open()){
        while(std::getline(file, line)){
            int i{};
            while(line[i]!=']') ++i;
            ++i;
            std::vector<std::vector<int>> button_list;
            while(line[i]!='{'){
                if(line[i]=='('){
                    ++i;
                    std::vector<int> affected;
                    int num{};
                    while(line[i]!=')'){
                        if(isdigit(line[i])){
                            num = num*10 + (line[i]-'0');
                        }else if(line[i]==','){
                            affected.push_back(num);
                            num = 0;
                        }
                        ++i;
                    }
                    affected.push_back(num);
                    button_list.push_back(affected);
                }
                ++i;
            }
            buttons.push_back(button_list);
            ++i;
            std::vector<long long> joltage;
            long long num{};
            while(line[i]!='}'){
                if(isdigit(line[i])){
                    num = num*10 + (line[i]-'0');
                }else if(line[i]==','){
                    joltage.push_back(num);
                    num = 0;
                }
                ++i;
            }
            joltage.push_back(num);
            joltage_levels.push_back(joltage);
        }
        file.close();
    }
}


int main(){
    std::vector<std::vector<std::vector<int>>> buttons;
    std::vector<std::vector<long long>> joltage_levels;
    get_data(buttons, joltage_levels, "input");

    long long result{}, op_result{};
    for(size_t i{}; i<joltage_levels.size(); ++i){
        result += op_result = get_min_pushes(joltage_levels[i], buttons[i]);
        std::cout << "Completed line " << i+1 << " out of " << joltage_levels.size() << " with result: " << op_result << "\n";
    }

    std::cout << "The final result is: " << result << "\n";
    return 0;
}
