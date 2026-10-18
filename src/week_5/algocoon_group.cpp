#include <iostream>
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
    cin.tie(0);
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
        // Every cut separates figure 0 from some figure j, on one side or the
        // other, so the cheapest cut is the smallest of these 2(n-1) flows.
        long min_flow = 5000*1000 + 1;
        int min_flow_s = 0;
        int min_flow_t = 0;
        int fixed = 0;
        for(int j = 1; j < n; ++j){
            long flow_1 = push_relabel_max_flow(sculpture, fixed, j);
            long flow_2 = push_relabel_max_flow(sculpture, j, fixed);
            long flow = min(flow_1, flow_2);
            if(flow < min_flow){
                min_flow = flow;
                if(flow_1 < flow_2){
                    min_flow_s = fixed;
                    min_flow_t = j;
                } else {
                    min_flow_s = j;
                    min_flow_t = fixed;
                }
            }
        }
        // Recompute only the best flow and take everything reachable from its
        // source in the residual graph (reverse edges included).
        vector<int> min_cut;
        vector<bool> visited(n, false);
        vector<int> to_visit;
        if(n > 1){
            push_relabel_max_flow(sculpture, min_flow_s, min_flow_t);
            to_visit.push_back(min_flow_s);
            visited[min_flow_t] = true;
        }
        while(!to_visit.empty()){
            int current = to_visit.back();
            to_visit.pop_back();
            if(visited[current]){
                continue;
            }
            graph_traits<Graph>::out_edge_iterator oi, oi_end;
            tie(oi, oi_end) = out_edges(current, sculpture);
            for(; oi != oi_end; ++oi){
                if(get(edge_residual_capacity, sculpture, *oi) > 0){
                    if(!visited[target(*oi, sculpture)]){
                        to_visit.push_back(target(*oi, sculpture));
                    }
                }
            }
            min_cut.push_back(current);
            visited[current] = true;
        }
        cout << min_flow << "\n";
        cout << min_cut.size() << " ";
        for(size_t j = 0; j < min_cut.size(); ++j){
            if(j == min_cut.size() - 1){
                cout << min_cut[j] << "\n";
            } else {
                cout << min_cut[j] << " ";
            }
        }
    }
    return 0;
}
