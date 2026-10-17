#include <iostream>
#include <vector>
#include <queue>
#include <limits>

using namespace std;

typedef pair<long long, int> Entry;

// Dijkstra on the product graph of the vertices with the parities of the
// number of edges and of the total weight. The four states of vertex v are
// 4*v + k: an even edge maps state k to 3 - k, an odd edge maps k to k ^ 1.
// The walk starts in state 4*s + 1 (no edges, weight 0) and has to end in
// state 4*t (odd number of edges, odd weight). The states are generated on the
// fly instead of storing four copies of every edge.

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    int t;
    cin >> t;
    for(int i = 0; i < t; ++i){
        int n, m;
        cin >> n >> m;
        int s, t;
        cin >> s >> t;
        vector< vector< pair<int,int> > > weird_graph(n);
        for(int j = 0; j < m; ++j){
            int s_i, t_i, w;
            cin >> s_i >> t_i >> w;
            weird_graph[s_i].push_back(pair<int,int>(t_i, w));
        }
        const long long INF = numeric_limits<long long>::max();
        vector<long long> d(4*n, INF);
        priority_queue<Entry, vector<Entry>, greater<Entry> > to_visit;
        d[4*s + 1] = 0;
        to_visit.push(Entry(0, 4*s + 1));
        while(!to_visit.empty()){
            long long distance = to_visit.top().first;
            int state = to_visit.top().second;
            to_visit.pop();
            if(distance > d[state]){
                continue;
            }
            if(state == 4*t){
                break;
            }
            int v = state/4;
            int k = state % 4;
            const vector< pair<int,int> > &edges = weird_graph[v];
            for(size_t j = 0; j < edges.size(); ++j){
                int w = edges[j].second;
                int next = 4*edges[j].first + (w % 2 == 0 ? 3 - k : k ^ 1);
                if(distance + w < d[next]){
                    d[next] = distance + w;
                    to_visit.push(Entry(d[next], next));
                }
            }
        }
        if(d[4*t] == INF){
            cout << "no\n";
        } else {
            cout << d[4*t] << "\n";
        }
    }
}
