#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>

using namespace std;

// The second shortest s-t walk is found with a BFS in which every vertex may
// be expanded twice: the k-th time a vertex is taken out of the queue gives
// the k-th shortest walk to it. Walks are told apart by their sequence of
// vertices, so parallel edges are merged first.

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    int t;
    cin >> t;
    for(int i = 0; i < t; ++i){
        int n, m;
        cin >> n >> m;
        int s, tar;
        cin >> s >> tar;
        --s;
        --tar;
        vector< vector<int> > network(n);
        for(int j = 0; j < m; ++j){
            int a,b;
            cin >> a >> b;
            --a;
            --b;
            network[a].push_back(b);
        }
        for(int j = 0; j < n; ++j){
            sort(network[j].begin(), network[j].end());
            network[j].erase(unique(network[j].begin(), network[j].end()), network[j].end());
        }
        vector<int> times_visited(n, 0);
        queue< pair<int,int> > to_visit;
        to_visit.push(pair<int,int>(s, 0));
        int min_cost_2 = -1;
        while(!to_visit.empty()){
            int next = to_visit.front().first;
            int cost = to_visit.front().second;
            to_visit.pop();
            if(times_visited[next] == 2){
                continue;
            }
            times_visited[next]++;
            if(next == tar && times_visited[next] == 2){
                min_cost_2 = cost;
                break;
            }
            for(size_t j = 0; j < network[next].size(); ++j){
                int next_next = network[next][j];
                if(times_visited[next_next] < 2){
                    to_visit.push(pair<int,int>(next_next, cost + 1));
                }
            }
        }
        if(min_cost_2 < 0)
            cout << "no\n";
        else
            cout << min_cost_2 << "\n";
    }
}
