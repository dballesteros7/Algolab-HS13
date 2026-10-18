#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <queue>
#include <vector>
#include <iostream>

using namespace boost;
using namespace std;

typedef adjacency_list_traits<vecS, vecS, directedS> Traits;
typedef Traits::edge_descriptor Edge;
typedef property<edge_capacity_t, int> EdgeCap;
typedef property<edge_residual_capacity_t, int, EdgeCap> EdgeRes;
typedef property<edge_reverse_t, Edge, EdgeRes> EdgeRev;
typedef adjacency_list<vecS, vecS, directedS, no_property, EdgeRev> Graph;

// Adds the arc a->b with the given capacity and its reverse arc
void add_capacity_edge(int a, int b, int capacity, Graph &uu){
    Edge forward, backward;
    tie(forward, tuples::ignore) = add_edge(a, b, uu);
    tie(backward, tuples::ignore) = add_edge(b, a, uu);
    put(edge_capacity, uu, forward, capacity);
    put(edge_capacity, uu, backward, 0);
    put(edge_reverse, uu, forward, backward);
    put(edge_reverse, uu, backward, forward);
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    int t;
    cin >> t;
    for(int i = 0; i < t; ++i){
        int n,m,s;
        cin >> n >> m >> s;
        vector<int> sweepers(s);
        vector<int> doors(s);
        vector<int> degrees(n, 0);
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
            add_capacity_edge(a, b, 1, uu);
            add_capacity_edge(b, a, 1, uu);
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
            add_capacity_edge(n, sweepers[j], 1, uu);
            add_capacity_edge(doors[j], n + 1, 1, uu);
        }
        // Every room with corridors (or sweepers/doors) must be reachable
        vector<bool> visited(n + 2);
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
