#include <iostream>
#include <vector>
#include <map>

using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    for(int i = 0; i < t; ++i){
        int n;
        cin >> n;
        int *lengths = new int[n];
        for(int j = 0; j < n; ++j){
            cin >> lengths[j];
        }
        int **chips = new int*[n];
        for(int j = 0; j < n; ++j){
            chips[j] = new int[lengths[j]];
            for(int k = lengths[j] - 1; k >= 0; --k){
                cin >> chips[j][k];
            }
        }

        // Iterate over all chips possibilities of size n
        int *pointers = new int[n];
        for(int j = 0; j < n; ++j){
            pointers[j] = -1;
        }
        map<vector<int>, int> scores;
        bool condition = true;
        while(condition){
            int max = 0;
            for(int j = 1; j < (1 << n); ++j){
                vector<int> new_state_positions;
                for(int k = 0; k < n; ++k){
                    if((j & (1 << k)) > 0){
                        new_state_positions.push_back(k);
                    }
                }
                if(new_state_positions.size() == 0){
                    cout << "PANIC\n";
                }
                bool dispose_subset = false;
                for(int k = 0; k < new_state_positions.size(); ++k){
                    if(pointers[new_state_positions[k]] == -1){
                        dispose_subset = true;
                        break;
                    }
                }
                if(dispose_subset){
                    continue;
                }
                int color = chips[new_state_positions[0]][pointers[new_state_positions[0]]];
                for(int k = 1; k < new_state_positions.size(); ++k){
                    if(chips[new_state_positions[k]][pointers[new_state_positions[k]]] != color){
                        dispose_subset = true;
                        break;
                    }
                }
                if(dispose_subset){
                    continue;
                }
                for(int k = 0; k < new_state_positions.size(); ++k){
                    //cout << new_state_positions[k] << " ";
                }
                //cout << "\n";
                vector<int> new_state(n);
                for(int k = 0; k < n; ++k){
                    new_state[k] = pointers[k];
                }
                for(int k = 0; k < new_state_positions.size(); ++k){
                    new_state[new_state_positions[k]]--;
                }
                int value;
                if(new_state_positions.size() > 1){
                    value = scores[new_state] + (1 << (new_state_positions.size() - 2));
                } else {
                    value = scores[new_state];
                }
                if(value > max){
                    max = value;
                }
            }
            vector<int> current_state(n);
            for(int j = 0; j < n; ++j){
                current_state[j] = pointers[j];
            }
            scores.insert(pair<vector<int>, int>(current_state, max));
            for(int j = n - 1; j >= 0; --j){
                pointers[j]++;
                if(pointers[j] == lengths[j]){
                    pointers[j] = -1;
                } else {
                    break;
                }
            }
            bool sum = true;
            for(int j = 0; j < n; ++j){
                sum &= (pointers[j] == -1);
            }
            if(sum){
                condition = false;
            }
        }
        vector<int> final_state(n);
        for(int j = 0; j < n; ++j){
            final_state[j] = lengths[j] - 1;
        }
        cout << scores[final_state] << "\n";
    }

}
