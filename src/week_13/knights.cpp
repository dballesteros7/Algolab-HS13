#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/boykov_kolmogorov_max_flow.hpp>
#include <iostream>
#include <vector>


using namespace std;
using namespace boost;

typedef adjacency_list_traits<vecS, vecS, directedS> Traits;
typedef property<edge_capacity_t, int> EdgeCapacity;
typedef property<edge_residual_capacity_t, int, EdgeCapacity> EdgeResidualCapacity;
typedef property<edge_reverse_t, Traits::edge_descriptor, EdgeResidualCapacity> EdgeReverse;
typedef adjacency_list<vecS, vecS, directedS, no_property, EdgeReverse> Graph;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    int t;
    cin >> t;
    for(int i = 0; i < t; ++i){
        int m, n, k;
        cin >> m >> n >> k;
        Graph castle(2*n*m + 2);
        for(int j = 0; j < m; ++j){
            for(int z = 0; z < n; ++z){
                // Define an internal path for the intersection
                Traits::edge_descriptor inner_inter, dummy;
                tie(inner_inter, tuples::ignore) = add_edge(2*(j*n + z), 2*(j*n + z) + 1, castle);
                tie(dummy, tuples::ignore) = add_edge(2*(j*n + z) + 1, 2*(j*n + z), castle);
                put(edge_capacity, castle, inner_inter, 1);
                put(edge_capacity, castle, dummy, 0);
                put(edge_reverse, castle, inner_inter, dummy);
                put(edge_reverse, castle, dummy, inner_inter);
                if(j == 0 || j == m - 1){
                    // Put a path from this intersection to the exit
                    Traits::edge_descriptor c_exit;
                    tie(c_exit, tuples::ignore) = add_edge(2*(j*n + z) + 1, 2*n*m + 1, castle);
                    tie(dummy, tuples::ignore) = add_edge(2*n*m + 1, 2*(j*n + z) + 1, castle);
                    put(edge_capacity, castle, c_exit, 1);
                    put(edge_capacity, castle, dummy, 0);
                    put(edge_reverse, castle, c_exit, dummy);
                    put(edge_reverse, castle, dummy, c_exit);
                }
                if(z == 0 || z == n - 1){
                    // Put a path from this intersection to the exit
                    Traits::edge_descriptor c_exit;
                    tie(c_exit, tuples::ignore) = add_edge(2*(j*n + z) + 1, 2*n*m + 1, castle);
                    tie(dummy, tuples::ignore) = add_edge(2*n*m + 1, 2*(j*n + z) + 1, castle);
                    put(edge_capacity, castle, c_exit, 1);
                    put(edge_capacity, castle, dummy, 0);
                    put(edge_reverse, castle, c_exit, dummy);
                    put(edge_reverse, castle, dummy, c_exit);
                }
                // Connect with other nodes
                Traits::edge_descriptor connector;
                if(j > 0){
                    tie(connector, tuples::ignore) = add_edge(2*(j*n + z) + 1, 2*((j-1)*n + z), castle);
                    tie(dummy, tuples::ignore) = add_edge(2*((j-1)*n + z), 2*(j*n + z) + 1, castle);
                    put(edge_capacity, castle, connector, 1);
                    put(edge_capacity, castle, dummy, 0);
                    put(edge_reverse, castle, connector, dummy);
                    put(edge_reverse, castle, dummy, connector);
                }
                if(j < m - 1){
                    tie(connector, tuples::ignore) = add_edge(2*(j*n + z) + 1, 2*((j+1)*n + z), castle);
                    tie(dummy, tuples::ignore) = add_edge(2*((j+1)*n + z), 2*(j*n + z) + 1, castle);
                    put(edge_capacity, castle, connector, 1);
                    put(edge_capacity, castle, dummy, 0);
                    put(edge_reverse, castle, connector, dummy);
                    put(edge_reverse, castle, dummy, connector);
                }
                if(z > 0){
                    tie(connector, tuples::ignore) = add_edge(2*(j*n + z) + 1, 2*(j*n + z - 1), castle);
                    tie(dummy, tuples::ignore) = add_edge(2*(j*n + z - 1), 2*(j*n + z) + 1, castle);
                    put(edge_capacity, castle, connector, 1);
                    put(edge_capacity, castle, dummy, 0);
                    put(edge_reverse, castle, connector, dummy);
                    put(edge_reverse, castle, dummy, connector);
                }
                if(z < n - 1){
                    tie(connector, tuples::ignore) = add_edge(2*(j*n + z) + 1, 2*(j*n + z + 1), castle);
                    tie(dummy, tuples::ignore) = add_edge(2*(j*n + z + 1), 2*(j*n + z) + 1, castle);
                    put(edge_capacity, castle, connector, 1);
                    put(edge_capacity, castle, dummy, 0);
                    put(edge_reverse, castle, connector, dummy);
                    put(edge_reverse, castle, dummy, connector);
                }
            }
        }
        for(int j = 0; j < k; ++j){
            int x, y;
            cin >> x >> y;
            Traits::edge_descriptor entry_point, dummy;
            tie(entry_point, tuples::ignore) = add_edge(2*n*m, 2*(x*n + y) + 1, castle);
            tie(dummy, tuples::ignore) = add_edge(2*(x*n + y) + 1, 2*n*m, castle);
            put(edge_capacity, castle, entry_point, 1);
            put(edge_capacity, castle, dummy, 0);
            put(edge_reverse, castle, entry_point, dummy);
            put(edge_reverse, castle, dummy, entry_point);
            Traits::edge_descriptor useless;
            tie(useless, tuples::ignore) = edge(2*(x*n + y), 2*(x*n + y) + 1, castle);
            put(edge_capacity, castle, useless, 0);
        }
        // Boykov-Kolmogorov is considerably faster than push-relabel on grid graphs
        vector<default_color_type> color(2*n*m + 2);
        vector<long> distance(2*n*m + 2);
        vector<Traits::edge_descriptor> predecessor(2*n*m + 2);
        int flow = boykov_kolmogorov_max_flow(castle, get(edge_capacity, castle), get(edge_residual_capacity, castle), get(edge_reverse, castle),
                                              &predecessor[0], &color[0], &distance[0], get(vertex_index, castle), 2*n*m, 2*n*m + 1);
        cout << flow << "\n";
    }
}
