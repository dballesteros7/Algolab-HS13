#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(0);
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
        // Sliding window over the sorted parasols: for every left parasol take
        // the largest window of width <= 200 and place the bar in its middle.
        // Both window ends only move right, so the candidate locations are
        // generated in increasing order and no sorting of candidates is needed.
        int max_locations = 0;
        int min_distance = 0;
        vector<int> bar_locations;
        int right_parasol = 0;
        for(int left_parasol = 0; left_parasol < p; ++left_parasol){
            while(right_parasol + 1 < p && parasols[right_parasol + 1] - parasols[left_parasol] <= 200){
                right_parasol++;
            }
            int locations = right_parasol - left_parasol + 1;
            int width = parasols[right_parasol] - parasols[left_parasol];
            int max_distance = (width + 1)/2;
            if(locations > max_locations || (locations == max_locations && max_distance < min_distance)){
                max_locations = locations;
                min_distance = max_distance;
                bar_locations.clear();
            }
            if(locations == max_locations && max_distance == min_distance){
                int sum = parasols[left_parasol] + parasols[right_parasol];
                if(width % 2 == 0){
                    bar_locations.push_back(sum/2);
                } else {
                    // floor and ceil of the odd midpoint
                    bar_locations.push_back((sum - 1)/2);
                    bar_locations.push_back((sum + 1)/2);
                }
            }
            if(right_parasol == p - 1){
                break;
            }
        }
        cout << max_locations << " " << min_distance << "\n";
        cout << bar_locations[0];
        for(size_t j = 1; j < bar_locations.size(); ++j){
            cout << " " << bar_locations[j];
        }
        cout << "\n";
    }
}
