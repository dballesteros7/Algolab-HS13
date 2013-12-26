#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>

using namespace std;

typedef pair<int, pair<int,int> > Triplet;

struct compare_strict{
    bool operator()(const Triplet& a, const Triplet& b) const{
        if(a.first > b.first){
            return true;
        } else if (a.first == b.first){
            if(a.second.first < b.second.first){
                return true;
            } else if(a.second.first == b.second.first){
                if(a.second.second < b.second.second){
                    return true;
                }
            }
        }
        return false;
    }
};

int main(){
    int n;
    cin >> n;
    for(int i = 0; i < n; ++i){
        int p;
        cin >> p;
        vector<int> parasols(p);
        for(int j = 0; j < p; ++j){
            cin >> parasols[j];
        }
        sort(parasols.begin(), parasols.end());
        bool stop = false;
        int left_parasol, right_parasol;
        int bar_loc;
        vector< Triplet > bar_locations;
        left_parasol = 0;
        right_parasol = 0;
        bar_loc = 0;
        while(!stop){
            bool record = false;
            while(!record){
                right_parasol++;
                if(right_parasol == p || parasols[right_parasol] - parasols[left_parasol] > 200){
                    record = true;
                    right_parasol--;
                }
            }
            if((parasols[left_parasol] + parasols[right_parasol]) % 2 == 0){
                bar_loc = (parasols[left_parasol] + parasols[right_parasol])/2;
                int locations = right_parasol - left_parasol + 1;
                int max_distance = max(bar_loc - parasols[left_parasol], parasols[right_parasol] - bar_loc);
                bar_locations.push_back(Triplet(locations, pair<int,int>(max_distance, bar_loc)));

            } else {
                bar_loc = (parasols[left_parasol] + parasols[right_parasol] + 1)/2;
                int locations = right_parasol - left_parasol + 1;
                int max_distance = max(bar_loc - parasols[left_parasol], parasols[right_parasol] - bar_loc);
                bar_locations.push_back(Triplet(locations, pair<int,int>(max_distance, bar_loc)));
                bar_loc = (parasols[left_parasol] + parasols[right_parasol] - 1)/2;
                locations = right_parasol - left_parasol + 1;
                max_distance = max(bar_loc - parasols[left_parasol], parasols[right_parasol] - bar_loc);
                bar_locations.push_back(Triplet(locations, pair<int,int>(max_distance, bar_loc)));
            }
            if(right_parasol == p - 1){
                stop = true;
            }
            left_parasol++;
        }
        sort(bar_locations.begin(), bar_locations.end(), compare_strict());
        int max_locations = bar_locations[0].first;
        int min_distance = bar_locations[0].second.first;
        cout << max_locations << " " << min_distance << "\n";
        cout << bar_locations[0].second.second;
        for(int j = 1; j < bar_locations.size(); ++j){
            if(bar_locations[j].first < max_locations || bar_locations[j].second.first > min_distance){
                break;
            }
            cout << " " << bar_locations[j].second.second;
        }
        cout << "\n";
    }
}
