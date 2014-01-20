#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <iostream>


using namespace std;
using namespace boost;

typedef adjacency_list_traits<vecS, vecS, directedS> Traits;
typedef property<edge_capacity_t, int> EdgeCapacity;
typedef property<edge_residual_capacity_t, int, EdgeCapacity> EdgeResidualCapacity;
typedef property<edge_reverse_t, Traits::edge_descriptor, EdgeResidualCapacity> EdgeReverse;
typedef adjacency_list<vecS, vecS, directedS, no_property, EdgeReverse> Graph;

int main(){
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
        int flow = push_relabel_max_flow(castle, 2*n*m, 2*n*m + 1);
        cout << flow << "\n";
        graph_traits<Graph>::vertex_iterator u_iter, u_end;
        graph_traits<Graph>::out_edge_iterator ei, e_end;
        for(tie(u_iter, u_end) = vertices(castle); u_iter != u_end; ++u_iter){
            for (tie(ei, e_end) = out_edges(*u_iter, castle); ei != e_end; ++ei){
                if(get(edge_capacity, castle, *ei) > 0){
                    //cout << "f " << *u_iter << " " << target(*ei, castle) << " " << (get(edge_capacity, castle, *ei) - get(edge_residual_capacity, castle, *ei)) << "\n";
                }
            }
        }
    }
}
