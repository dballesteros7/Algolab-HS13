#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

using namespace boost;
using namespace std;


typedef adjacency_list_traits<vecS, vecS, directedS> Traits;
typedef property<edge_capacity_t, long> EdgeCapacity;
typedef property<edge_residual_capacity_t, long, EdgeCapacity> EdgeResidual;
typedef property<edge_reverse_t, Traits::edge_descriptor, EdgeResidual> EdgeReverse;
typedef adjacency_list<vecS, vecS, directedS, no_property, EdgeReverse> Graph;

void add_capacity_edge(Graph &g, int from, int to, long capacity){
    Traits::edge_descriptor foward, rev;
    tie(foward, tuples::ignore) = add_edge(from, to, g);
    tie(rev, tuples::ignore) = add_edge(to, from, g);
    put(edge_capacity, g, foward, capacity);
    put(edge_capacity, g, rev, 0);
    put(edge_reverse, g, foward, rev);
    put(edge_reverse, g, rev, foward);
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    int t;
    cin >> t;
    for(int i = 0; i < t; ++i){
        int n,m;
        cin >> n;
        cin >> m;
        vector<int> known_scores(n, 0);
        vector< pair<int, int> > unknown;
        for(int j = 0; j < m; ++j){
            int a, b, c;
            cin >> a;
            cin >> b;
            cin >> c;
            if(c == 2){
                known_scores[b]++;
            } else if(c == 1){
                known_scores[a]++;
            } else {
                unknown.push_back(make_pair(min(a,b), max(a,b)));
            }
        }
        vector<int> expected_scores(n);
        bool bullshit = false;
        long expected_total = 0;
        for(int j = 0; j < n; j++){
            int s;
            cin >> s;
            expected_scores[j] = s - known_scores[j];
            if(expected_scores[j] < 0){
                bullshit = true;
            }
            expected_total += expected_scores[j];
        }
        // Every unknown game hands out exactly one point, so the missing
        // points must add up to the number of unknown games.
        if(bullshit || expected_total != (long)unknown.size()){
            cout << "no\n";
            continue;
        }
        // source -> pair of players (one vertex per pair, capacity = games
        // between them) -> each of the two players -> sink (missing points).
        int source = n;
        int sink = n + 1;
        Graph matches(n + 2);
        for(int j = 0; j < n; ++j){
            if(expected_scores[j] > 0){
                add_capacity_edge(matches, j, sink, expected_scores[j]);
            }
        }
        sort(unknown.begin(), unknown.end());
        for(size_t j = 0; j < unknown.size(); ){
            size_t k = j;
            while(k < unknown.size() && unknown[k] == unknown[j]){
                ++k;
            }
            int games = add_vertex(matches);
            add_capacity_edge(matches, source, games, k - j);
            add_capacity_edge(matches, games, unknown[j].first, k - j);
            add_capacity_edge(matches, games, unknown[j].second, k - j);
            j = k;
        }
        long flow = push_relabel_max_flow(matches, source, sink);
        if(flow == (long)unknown.size()){
            cout << "yes\n";
        } else {
            cout << "no\n";
        }


    }


}
