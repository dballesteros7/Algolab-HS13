#include <iostream>
#include <algorithm>
#include <climits>
#include <utility>
#include <vector>

using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    int t;
    int N;
    int i, j;
    // (ring position, length), sorted by ring position
    vector< pair<int,int> > boats;
    int count;
    cin >> t;
    for(i = 0; i < t; i++){
        cin >> N;
        count = 0;
        boats.resize(N);
        for(j = 0; j < N; j++){
            cin >> boats[j].second;
            cin >> boats[j].first;
        }
        sort(boats.begin(), boats.end());
        // current_start -> end of the last boat fixed so far
        // best_deadline -> earliest end of a boat placed after current_start
        long long current_start = LLONG_MIN/2;
        long long best_deadline = LLONG_MAX;
        for(j = 0; j < N; j++){
            long long p = boats[j].first;
            long long l = boats[j].second;
            if(p >= best_deadline){
                count++;
                current_start = best_deadline;
                best_deadline = LLONG_MAX;
            }
            long long possible_deadline = max(current_start, p - l) + l;
            if(possible_deadline <= best_deadline){
                best_deadline = possible_deadline;
            }
        }
        if(N > 0){
            count++;
        }

        cout << count << "\n";
    }
    return 0;
}
