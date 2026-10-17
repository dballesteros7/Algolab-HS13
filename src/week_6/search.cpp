#include <iostream>
#include <utility>
#include <vector>
#include <algorithm>
#include <queue>
#include <climits>

using namespace std;

typedef pair<long,int> Pos;
typedef priority_queue<Pos, vector<Pos>, std::greater<Pos> > MinQueue;

int main(){
    ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    for(int i = 0; i < t; ++i){
        int n;
        cin >> n;
        vector<int> occurrences(n);
        for(int j = 0; j < n; ++j){
            cin >> occurrences[j];
        }
        MinQueue mins;
        // Pointers only move forward, so the largest position seen so far is the window's maximum.
        long max_pos = LONG_MIN;
        vector< vector<long> > posLists(n);
        for(int j = 0; j < n; ++j){
            posLists[j].resize(occurrences[j]);
            for(int k = 0; k < occurrences[j]; ++k){
                cin >> posLists[j][k];
            }
            mins.push(Pos(posLists[j][0],j));
            max_pos = max(max_pos, posLists[j][0]);
        }
        vector<int> pointers(n, 0);
        long min_distance = max_pos - mins.top().first + 1;
        while(true){
            int word = mins.top().second;
            if(++pointers[word] == occurrences[word])
                break;
            long next = posLists[word][pointers[word]];
            mins.pop();
            mins.push(Pos(next, word));
            max_pos = max(max_pos, next);
            min_distance = min(min_distance, max_pos - mins.top().first + 1);
        }
        cout << min_distance << "\n";
    }
}
