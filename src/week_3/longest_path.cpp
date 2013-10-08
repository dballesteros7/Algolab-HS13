#include <iostream>
#include <vector>
#include <deque>
#include <map>
#include <queue>
#include <utility>
#include <set>

using namespace std;

int longest_path(int parent, int current_node, map<int, deque<int> >& adjacency){
    int count = 0;
    for(int i = 0; i < adjacency[current_node].size(); i++){
        int child = adjacency[current_node][i];
        if(child == parent){
            continue;
        }
        int tmp = longest_path(current_node, child, adjacency);
        if(tmp > count){
            count = tmp;
        }
    }
    return count + 1;
}

int main(){
    ios_base::sync_with_stdio(false);
    int t, N;
    map< int, set<int> > adjacency;
    deque< pair<int,int> > longest_connecting;
    queue<int> to_visit;
    cin >> t;
    for(int i = 0; i < t; i++){
        cin >> N;
        adjacency = map< int, set<int> >();
        longest_connecting = deque< pair<int, int> >();
        to_visit = queue<int>();
        for(int j = 0; j < N - 1; j++){
            int a, b;
            cin >> a;
            cin >> b;
            adjacency[a].insert(b);
            adjacency[b].insert(a);
            longest_connecting.push_back(pair<int,int>());
            longest_connecting[j].first = 0;
            longest_connecting[j].second = 0;
        }
        longest_connecting[N - 1].first = 0;
        longest_connecting[N - 1].second = 0;
        for(int j = 0; j < N; j++){
            if(adjacency[j].size() == 1){
                to_visit.push(j);
            }
        }
        while(!to_visit.empty()){
            int node = to_visit.front();
            to_visit.pop();
            set<int>::iterator it = adjacency[node].begin();
            for(; it != adjacency[node].end(); it++){
                int parent = (*it);
                adjacency[parent].erase(node);
                if(adjacency[parent].size() == 1){
                    to_visit.push(parent);
                    longest_connecting[parent].first = longest_connecting[node].first + 1;
                } else {
                    longest_connecting[parent].second = longest_connecting[node].first + 1;
                }
            }
            if(adjacency[node].size() == 0){
                cout << longest_connecting[node].first + longest_connecting[node].second + 1 << "\n";
            }
        }
    }
    return 0;
}