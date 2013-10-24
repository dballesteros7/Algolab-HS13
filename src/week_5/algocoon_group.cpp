#include <iostream>
#include <stack>
#include <vector>
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
    int t;
    cin >> t;
    for(int i = 0; i < t; ++i){
        int n, m;
        cin >> n;
        cin >> m;
        Graph sculpture(n);
        for(int j = 0; j < m; ++j){
            int a,b,c;
            cin >> a;
            cin >> b;
            cin >> c;
            bool exists;
            Traits::edge_descriptor limb;
            tie(limb, exists) = edge(a, b, sculpture);
            if(exists && get(edge_capacity, sculpture, limb) > 0){
                put(edge_capacity, sculpture, limb, get(edge_capacity, sculpture, limb) + c);
            } else {
                tie(limb, tuples::ignore) = add_edge(a, b, sculpture);
                put(edge_capacity, sculpture, limb, c);
                Traits::edge_descriptor rev;
                tie(rev, tuples::ignore) = add_edge(b, a, sculpture);
                put(edge_capacity, sculpture, rev, 0);
                put(edge_reverse, sculpture, limb, rev);
                put(edge_reverse, sculpture, rev, limb);
            }
        } 
        property_map<Graph, edge_residual_capacity_t>::type min_residual_cap;
        int min_flow = 5000*1000 + 1;
        int min_flow_s = 0;
        int min_flow_t = 0;
        int fixed = 0;
        vector<int> min_cut;
        bool *visited = new bool[n];
        for(int j = 0; j < n; ++j){
            visited[j] = false;
        }
        for(int j = 1; j < n; ++j){
            int flow_1 = push_relabel_max_flow(sculpture, fixed, j);
            int flow_2 = push_relabel_max_flow(sculpture, j, fixed);
            int flow = flow_2;
            if(flow_1 < flow_2){
                push_relabel_max_flow(sculpture, fixed, j);
                flow = flow_1;
            }
            if(flow < min_flow){
                min_flow = flow;
                if(flow_1 < flow_2){
                    min_flow_s = fixed;
                    min_flow_t = j;
                } else {
                    min_flow_s = j;
                    min_flow_t = fixed;
                }
                min_cut = vector<int>();
                stack<int> to_visit;
                to_visit.push(min_flow_s);
                for(int m = 0; m < n; ++m)
                    visited[m] = false;
                visited[min_flow_t] = true;
                while(!to_visit.empty()){
                    int current = to_visit.top();
                    to_visit.pop();
                    if(visited[current]){
                        continue;
                    }
                    graph_traits<Graph>::out_edge_iterator oi, oi_end;
                    tie(oi, oi_end) = out_edges(current, sculpture);
                    for(; oi != oi_end; ++oi){
                        if(get(edge_capacity, sculpture, *oi) == 0){
                            continue;
                        }
                        if(get(edge_residual_capacity, sculpture, *oi) > 0){
                            if(!visited[target(*oi, sculpture)]){
                                to_visit.push(target(*oi, sculpture));
                            }
                        }
                    }
                    min_cut.push_back(current);
                    visited[current] = true;
                }
            }
        }
        cout << min_flow << "\n";
        cout << min_cut.size() << " ";
        for(int j = 0; j < min_cut.size(); ++j){
            if(j == min_cut.size() - 1){
                cout << min_cut[j] << "\n";   
            } else {
                cout << min_cut[j] << " ";
            }
        }
    }
    return 0;
}
