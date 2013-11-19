#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

using namespace std;
using namespace boost;

typedef adjacency_list_traits<vecS, vecS, directedS> Traits;
typedef property<edge_capacity_t, long> EdgeCap;
typedef property<edge_residual_capacity_t, long, EdgeCap> EdgeResidualCap;
typedef property<edge_reverse_t, Traits::edge_descriptor, EdgeResidualCap> EdgeRev;
typedef adjacency_list<vecS, vecS, directedS, no_property, EdgeRev> Graph;

int main(){
    ios_base::sync_with_stdio(false);
    int T;
    cin >> T;
    for(int i = 0; i < T; ++i){
        int n, m, s;
        cin >> n >> m >> s;
        Graph cityNetwork(n + 1);
        vector<int> storeNumbers(s);
        for(int j = 0; j < s; j++){
            cin >> storeNumbers[j];
            bool exists = false;
            Traits::edge_descriptor route;
            tie(route, exists) = edge(storeNumbers[j], n, cityNetwork);
            if(exists){
                put(edge_capacity, cityNetwork, route, get(edge_capacity,cityNetwork, route) + 1);
            } else {
                tie(route, tuples::ignore) = add_edge(storeNumbers[j], n, cityNetwork);
                put(edge_capacity, cityNetwork, route, 1);
                Traits::edge_descriptor inverseRoute;
                tie(inverseRoute, tuples::ignore) = add_edge(n, storeNumbers[j], cityNetwork);
                put(edge_capacity, cityNetwork, inverseRoute, 0);
                put(edge_reverse, cityNetwork, route, inverseRoute);
                put(edge_reverse, cityNetwork, inverseRoute, route);
            }
        }
        for(int j = 0; j < m; ++j){
            int a,b;
            cin >> a >> b;
            if(a != 0 && b != 0){
                Traits::edge_descriptor routeA, inverseRouteA, routeB, inverseRouteB;
                tie(routeA, tuples::ignore) = add_edge(a, b, cityNetwork);
                tie(inverseRouteA, tuples::ignore) = add_edge(b, a, cityNetwork);
                tie(routeB, tuples::ignore) = add_edge(b, a, cityNetwork);
                tie(inverseRouteB, tuples::ignore) = add_edge(a, b, cityNetwork);
                
                put(edge_capacity, cityNetwork, routeA, 1);
                put(edge_capacity, cityNetwork, routeB, 1);
                put(edge_capacity, cityNetwork, inverseRouteA, 0);
                put(edge_capacity, cityNetwork, inverseRouteB, 0);

                put(edge_reverse, cityNetwork, routeA, inverseRouteA);
                put(edge_reverse, cityNetwork, routeB, inverseRouteB);
                put(edge_reverse, cityNetwork, inverseRouteA, routeA);
                put(edge_reverse, cityNetwork, inverseRouteB, routeB);
            } else {
                Traits::edge_descriptor routeA, inverseRouteA;
                tie(routeA, tuples::ignore) = add_edge(a,b, cityNetwork);
                tie(inverseRouteA, tuples::ignore) = add_edge(b,a,cityNetwork);
                
                put(edge_reverse, cityNetwork, routeA, inverseRouteA);
                put(edge_reverse, cityNetwork, inverseRouteA, routeA);

                if(a == 0){
                    put(edge_capacity, cityNetwork, routeA, 1);
                    put(edge_capacity, cityNetwork, inverseRouteA, 0);
                } else {
                    put(edge_capacity, cityNetwork, inverseRouteA, 1);
                    put(edge_capacity, cityNetwork, routeA, 0);
                }
            }
        }
        long flow = push_relabel_max_flow(cityNetwork, 0, n);
        graph_traits<Graph>::vertex_iterator u_iter, u_end;
        graph_traits<Graph>::out_edge_iterator ei, e_end;
        //for (tie(u_iter, u_end) = vertices(cityNetwork); u_iter != u_end; ++u_iter)
        //    for (tie(ei, e_end) = out_edges(*u_iter, cityNetwork); ei != e_end; ++ei)
        //        if (get(edge_capacity,cityNetwork, *ei) > 0)
                    //std::cout << "f " << *u_iter << " " << target(*ei, cityNetwork) << " " << (get(edge_capacity, cityNetwork, *ei) - get(edge_residual_capacity, cityNetwork, *ei)) << std::endl;

        if(flow == s){
            cout << "yes\n";
        } else {
            cout << "no\n";
        }
    }
}
