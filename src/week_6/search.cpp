#include <iostream>
#include <utility>
#include <vector>
#include <algorithm>
#include <map>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    for(int i = 0; i < t; ++i){
        int n;
        cin >> n;
        int *occurrences = new int[n];
        int *current_occurrences = new int[n];
        bool *exists = new bool[n];
        int total = 0;
        for(int j = 0; j < n; ++j){
            cin >> occurrences[j];
            total += occurrences[j];
            exists[j] = false;
            current_occurrences[j] = 0;
        }
        vector<pair<int,int> > positions(total, pair<int,int>());
        int w = 0;
        int sum = 0;
        for(int j = 0; j < total; ++j){
            if(j - sum >= occurrences[w]){
                sum += occurrences[w];
                ++w;
            }
            int p;
            cin >> p;
            pair<int,int> current(p, w);
            positions[j] = current;
        }
        sort(&positions[0], &positions[total]);
        int opt_start_pos = positions[0].first;
        int opt_start_pos_j = 0;
        int opt_end_pos = positions[1].first;
        int opt_end_pos_j = 1;
        current_occurrences[positions[0].second]++;
        current_occurrences[positions[1].second]++;
        exists[positions[0].second] = true;
        exists[positions[1].second] = true;
        for(int j = 2; j < total; ++j){
            pair<int, int> current = positions[j];
            if(!exists[current.second]){
                exists[current.second] = true;
                opt_end_pos = current.first;
                opt_end_pos_j = j;
                current_occurrences[current.second]++;
            } else {
                //cout << current.second << "\n";
                current_occurrences[current.second]++;
                int new_start = opt_start_pos_j;
                vector<int> restore;
                for(int k = opt_start_pos_j; k < j; ++k){
                    if(current_occurrences[positions[k].second] > 1){
                        current_occurrences[positions[k].second]--;
                        restore.push_back(positions[k].second);
                    } else {
                        new_start = k;
                        break;
                    }
                }
                int distance_1 = current.first - positions[new_start].first + 1;
                int distance_2 = opt_end_pos - opt_start_pos + 1;
                //cout << distance_1 << " " << distance_2 << "\n";
                if(distance_1 < distance_2){
                    //cout << distance_1 << " " << distance_2 << "\n";
                    //cout << positions[new_start].first << " " << current.first << "\n";
                    opt_start_pos_j = new_start;
                    opt_start_pos = positions[opt_start_pos_j].first;
                    opt_end_pos = current.first;
                    opt_end_pos_j = j;
                } else {
                    for(int k = 0; k < restore.size(); k++){
                        current_occurrences[restore[k]]++;
                    }
                }
            }
        }
        cout << opt_end_pos - opt_start_pos + 1 << "\n";
        //cout << opt_end_pos << " " << opt_start_pos << "\n";
    }
}
