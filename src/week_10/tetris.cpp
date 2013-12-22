#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
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

int main(){
    ios_base::sync_with_stdio(false);
    int T;
    cin >> T;
    for(int i = 0; i < T; ++i){
        int w, n;
        cin >> w >> n;
        Graph field(2*(w + 1));
        for(int j = 0; j <= w; ++j){
            Edge tight_edge, dummy_edge;
            tie(tight_edge, tuples::ignore) = add_edge(2*j, 2*j + 1, field);
            tie(dummy_edge, tuples::ignore) = add_edge(2*j + 1, 2*j, field);
            put(edge_capacity, field, tight_edge, 1);
            put(edge_capacity, field, dummy_edge, 0);
            put(edge_reverse, field, tight_edge, dummy_edge);
            put(edge_reverse, field, dummy_edge, tight_edge);
        }
        for(int j = 0; j < n; ++j){
            int a,b;
            cin >> a >> b;
            Edge brick, dummy_brick, possible_brick;
            bool duplicate;
            if(a < b){
                tie(possible_brick, duplicate) = edge(2*a + 1, 2*b, field);
            } else {
                tie(possible_brick, duplicate) = edge(2*b + 1, 2*a, field);
            }
            if(duplicate && (a - b < w) && (b -a < w)){
                continue;
            } else if(duplicate){
                int cap = get(edge_capacity, field, possible_brick);
                put(edge_capacity, field, possible_brick, cap + 1);
            } else {
                if(a < b){
                    tie(brick, tuples::ignore) = add_edge(2*a + 1, 2*b, field); 
                    tie(dummy_brick, tuples::ignore) = add_edge(2*b, 2*a + 1, field);
                } else {
                    tie(brick, tuples::ignore) = add_edge(2*b + 1, 2*a, field);
                    tie(dummy_brick, tuples::ignore) = add_edge(2*a, 2*b + 1, field);
                }
                put(edge_capacity, field, brick, 1);
                put(edge_capacity, field, dummy_brick, 0);
                put(edge_reverse, field, brick, dummy_brick);
                put(edge_reverse, field, dummy_brick, brick);
            }
        }
        int flow;
        flow = push_relabel_max_flow(field, 1, 2*w);
        cout << flow << "\n";
    }
}
