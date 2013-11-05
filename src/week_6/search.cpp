#include <iostream>
#include <utility>
#include <vector>
#include <algorithm>
#include <map>
#include <queue>
#include <climits>

using namespace std;

typedef pair<long,int> Pos;
typedef priority_queue<Pos> MaxQueue;
typedef priority_queue<Pos, vector<Pos>, std::greater<Pos> > MinQueue;

int main(){
    ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    for(int i = 0; i < t; ++i){
        int n;
        cin >> n;
        long *occurrences = new long[n];
        for(int j = 0; j < n; ++j){
            cin >> occurrences[j];
        }
        MinQueue mins;
        MaxQueue maxes;
        vector<long> *posLists = new vector<long>[n];
        for(int j = 0; j < n; ++j){
            posLists[j] = vector<long>(occurrences[j]);
            for(int k = 0; k < occurrences[j]; ++k){
                cin >> posLists[j][k];
            }
            Pos tmp(posLists[j][0],j);
            mins.push(tmp);
            maxes.push(tmp);
        }
        vector<long>::iterator *pointers = new vector<long>::iterator [n];
        for(int j = 0; j < n; ++j){
            pointers[j] = posLists[j].begin();
        }
        long min_distance = maxes.top().first - mins.top().first + 1;
        bool condition = false;
        while(!condition){
            Pos min_pos = mins.top();
            if(pointers[min_pos.second] + 1 == posLists[min_pos.second].end()){
                condition = true;
            } else {
                pointers[min_pos.second]++;
                mins.pop();
                Pos tmp(*(pointers[min_pos.second]),min_pos.second);
                mins.push(tmp);
                maxes.push(tmp);
                long interval = maxes.top().first - mins.top().first + 1;
                if(interval < min_distance){
                    min_distance = interval;
                }
            }
        }
        cout << min_distance << "\n";
    }
}
