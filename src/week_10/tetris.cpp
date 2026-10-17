#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;
using namespace boost;

typedef adjacency_list_traits<vecS, vecS, directedS> Traits;
typedef Traits::edge_descriptor Edge;
typedef property<edge_capacity_t, int> EdgeCapacity;
typedef property<edge_residual_capacity_t, int, EdgeCapacity> EdgeResidualCapacity;
typedef property<edge_reverse_t, Traits::edge_descriptor, EdgeResidualCapacity> EdgeReverse;
typedef adjacency_list<vecS, vecS, directedS, no_property, EdgeReverse> Graph;

void add_unit_edge(int u, int v, Graph &field){
    Edge e, reverse_e;
    tie(e, tuples::ignore) = add_edge(u, v, field);
    tie(reverse_e, tuples::ignore) = add_edge(v, u, field);
    put(edge_capacity, field, e, 1);
    put(edge_capacity, field, reverse_e, 0);
    put(edge_reverse, field, e, reverse_e);
    put(edge_reverse, field, reverse_e, e);
}

int main(){
    ios_base::sync_with_stdio(false);
    int T;
    cin >> T;
    for(int i = 0; i < T; ++i){
        int w, n;
        cin >> w >> n;
        // A brick spanning the whole width is a layer on its own.
        int full_bricks = 0;
        vector< pair<int,int> > bricks;
        bricks.reserve(n);
        for(int j = 0; j < n; ++j){
            int a,b;
            cin >> a >> b;
            if(a > b) swap(a, b);
            if(b - a == w){
                ++full_bricks;
            } else {
                bricks.push_back(make_pair(a, b));
            }
        }
        // Every crack is used by at most one layer, so a second copy of a
        // shorter brick never helps.
        sort(bricks.begin(), bricks.end());
        bricks.erase(unique(bricks.begin(), bricks.end()), bricks.end());
        // Crack j is split into 2j (in) and 2j + 1 (out) joined by a unit edge.
        Graph field(2*(w + 1));
        for(int j = 1; j < w; ++j){
            add_unit_edge(2*j, 2*j + 1, field);
        }
        for(size_t j = 0; j < bricks.size(); ++j){
            add_unit_edge(2*bricks[j].first + 1, 2*bricks[j].second, field);
        }
        int flow = push_relabel_max_flow(field, 1, 2*w);
        cout << full_bricks + flow << "\n";
    }
}
