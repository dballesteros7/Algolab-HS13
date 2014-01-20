#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <queue>
#include <iostream>

using namespace boost;
using namespace std;

typedef adjacency_list_traits<vecS, vecS, directedS> Traits;
typedef Traits::edge_descriptor Edge;
typedef property<edge_capacity_t, int> EdgeCap;
typedef property<edge_residual_capacity_t, int, EdgeCap> EdgeRes;
typedef property<edge_reverse_t, Edge, EdgeRes> EdgeRev;
typedef adjacency_list<vecS, vecS, directedS, no_property, EdgeRev> Graph;

int main(){
    int t;
    cin >> t;
    for(int i = 0; i < t; ++i){
        int n,m,s;
        cin >> n >> m >> s;
        int *sweepers = new int[s];
        int *doors = new int[s];
        int *degrees = new int[n];
        for(int j = 0; j < n; ++j){
            degrees[j] = 0;
        }
        for(int j = 0; j < s; ++j){
            cin >> sweepers[j];
            degrees[sweepers[j]]++;
        }
        for(int j = 0; j < s; ++j){
            cin >> doors[j];
            degrees[doors[j]]++;
        }
        Graph uu(n + 2);
        for(int j = 0; j < m; ++j){
            int a,b;
            cin >> a >> b;
            graph_traits<Graph>::edge_descriptor f_edge, b_edge, dummy;
            tie(f_edge, tuples::ignore) = add_edge(a,b, uu);
            tie(dummy, tuples::ignore) = add_edge(b,a,uu);
            put(edge_capacity, uu, f_edge, 1);
            put(edge_capacity, uu, dummy, 0);
            put(edge_reverse, uu, f_edge, dummy);
            put(edge_reverse, uu, dummy, f_edge);
            tie(b_edge, tuples::ignore) = add_edge(b,a, uu);
            tie(dummy, tuples::ignore) = add_edge(a,b,uu);
            put(edge_capacity, uu, b_edge, 1);
            put(edge_capacity, uu, dummy, 0);
            put(edge_reverse, uu, b_edge, dummy);
            put(edge_reverse, uu, dummy, b_edge);
            degrees[a]++;
            degrees[b]++;
        }
        bool is_eulerian = true;
        for(int j = 0; j < n; ++j){
            if(degrees[j] % 2 != 0){
                cout << "no\n";
                is_eulerian = false;
                break;
            }
        }
        if(!is_eulerian){
            continue;
        }
        for(int j = 0; j < s; ++j){
            graph_traits<Graph>::edge_descriptor in_edge, out_edge, dummy;
            tie(in_edge, tuples::ignore) = add_edge(n, sweepers[j], uu);
            tie(dummy, tuples::ignore) = add_edge(sweepers[j], n, uu);
            put(edge_capacity, uu, in_edge, 1);
            put(edge_capacity, uu, dummy, 0);
            put(edge_reverse, uu, in_edge, dummy);
            put(edge_reverse, uu, dummy, in_edge);

            tie(out_edge, tuples::ignore) = add_edge(doors[j], n + 1, uu);
            tie(dummy, tuples::ignore) = add_edge(n + 1, doors[j], uu);
            put(edge_capacity, uu, out_edge, 1);
            put(edge_capacity, uu, dummy, 0);
            put(edge_reverse, uu, out_edge, dummy);
            put(edge_reverse, uu, dummy, out_edge);
        }
        bool *visited = new bool[n];
        for(int j = 0; j < n; ++j){
            visited[j] = (degrees[j] == 0);
        }
        std::queue<int> to_visit;
        to_visit.push(n);
        visited[n] = true;
        while(!to_visit.empty()){
            int next = to_visit.front();
            to_visit.pop();
            graph_traits<Graph>::out_edge_iterator ei, e_end;
            for(tie(ei, e_end) = out_edges(next, uu); ei != e_end; ++ei){
                int next_next = target(*ei, uu);
                if(!visited[next_next]){
                    visited[next_next] = true;
                    to_visit.push(next_next);
                }
            }
        }
        bool is_connected = true;
        for(int j = 0; j < n; ++j){
            if(!visited[j]){
                is_connected = false;
                cout << "no\n";
                break;
            }
        }
        if(!is_connected){
            continue;
        }
        int flow = push_relabel_max_flow(uu, n, n+1);
        if(flow == s){
            cout << "yes\n";
        } else {
            cout << "no\n";
        }

    }
}
