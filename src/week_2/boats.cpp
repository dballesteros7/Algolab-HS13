#include <iostream>
#include <set>
#include <vector>

using namespace std;

struct compare_strict{
    bool operator()(const vector<int>& a, const vector<int>& b) const{
        return a[0] < b[0];
    }
};

int main(){
    int t;
    int N;
    int i, j;
    set< vector<int>, compare_strict > ordered_intervals;
    int count;
    cin >> t;
    for(i = 0; i < t; i++){
        cin >> N;
        count = 0;
        ordered_intervals = set< vector<int>, compare_strict>();
        for(j = 0; j < N; j++){
            int l, p;
            cin >> l;
            cin >> p;
            vector<int> tmp = vector<int>();
            tmp.push_back(p);
            tmp.push_back(l);
            ordered_intervals.insert(tmp);
        }
        int current_start = -2000000;
        set< vector<int>, compare_strict >::iterator it = ordered_intervals.begin();
        int best_deadline = 3000000;
        while(it != ordered_intervals.end()){
            int l, p;
            p = (*it)[0];
            l = (*it)[1];
            if(p >= best_deadline){
                count++;
                current_start = best_deadline;
                best_deadline = 3000000;
            }
            int possible_deadline = max(current_start, p - l) + l;
            if(possible_deadline <= best_deadline){
                best_deadline = possible_deadline;
                it++;
            } else {
                it++;
            }
        }
        count++;
        
        cout << count << "\n";
    }
    return 0;
}
