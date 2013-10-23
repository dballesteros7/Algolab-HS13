#include <iostream>
#include <map>
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

int main(){
    ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    for(int i = 0; i < t; ++i){
        int n,m;
        cin >> n;
        cin >> m;
        int *known_scores = new int[n];
        Graph matches(2*n + 2);
        int *sources = new int[n*n];
        int unknown_count = 0;
        for(int j = 0; j < n; ++j){
            known_scores[j] = 0;
            for(int k = 0; k < n; ++k){
                sources[j*n + k] = -1;
            }
            Traits::edge_descriptor foward, rev;
            bool dummy;
            tie(foward, dummy) = add_edge(j + 1, j + 1 + n, matches);
            tie(rev, dummy) = add_edge(j + 1 + n, j + 1, matches);
            put(edge_capacity, matches, foward, 0);
            put(edge_capacity, matches, rev, 0);
            put(edge_reverse, matches, foward, rev);
            put(edge_reverse, matches, rev, foward);
            Traits::edge_descriptor infinite_f;
            tie(infinite_f, dummy) = add_edge(j + 1 + n, 2*n + 1, matches);
            tie(rev, dummy) = add_edge(2*n + 1, j + 1 + n, matches);
            put(edge_capacity, matches, infinite_f, 1024);
            put(edge_capacity, matches, rev, 0);
            put(edge_reverse, matches, infinite_f, rev);
            put(edge_reverse, matches, rev, infinite_f);
        }
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
                unknown_count++;
                Traits::edge_descriptor edge_to_a, edge_to_b, edge_to_source;
                bool dummy;
                int source = n*min(a,b) + max(a,b);
                if(sources[source] != -1){
                    tie(edge_to_a, dummy) = edge(sources[source], a + 1, matches);
                    tie(edge_to_b, dummy) = edge(sources[source], b + 1, matches);
                    tie(edge_to_source, dummy) = edge(0, sources[source], matches);
                    int current_cap = get(edge_capacity, matches, edge_to_a);
                    put(edge_capacity, matches, edge_to_a, current_cap + 1);
                    put(edge_capacity, matches, edge_to_b, current_cap + 1);
                    put(edge_capacity, matches, edge_to_source, current_cap + 1);
                } else {
                    int new_vertex = add_vertex(matches);
                    sources[source] = new_vertex;
                    Traits::edge_descriptor foward, rev;
                    bool dummy;
                    tie(foward, dummy) = add_edge(new_vertex, a + 1, matches);
                    tie(rev, dummy) = add_edge(a + 1, new_vertex, matches);
                    put(edge_capacity, matches, foward, 1);
                    put(edge_capacity, matches, rev, 0);
                    put(edge_reverse, matches, foward, rev);
                    put(edge_reverse, matches, rev, foward);
                    tie(foward, dummy) = add_edge(new_vertex, b + 1, matches);
                    tie(rev, dummy) = add_edge(b + 1, new_vertex, matches);
                    put(edge_capacity, matches, foward, 1);
                    put(edge_capacity, matches, rev, 0);
                    put(edge_reverse, matches, foward, rev);
                    put(edge_reverse, matches, rev, foward);
                    tie(foward, dummy) = add_edge(0, new_vertex, matches);
                    tie(rev, dummy) = add_edge(new_vertex, 0, matches);
                    put(edge_capacity, matches, foward, 1);
                    put(edge_capacity, matches, rev, 0);
                    put(edge_reverse, matches, foward, rev);
                    put(edge_reverse, matches, rev, foward);
                }
            }
        }
        int *expected_scores = new int[n];
        bool bullshit = false;
        for(int j = 0; j < n; j++){
            int s;
            cin >> s;
            expected_scores[j] = s - known_scores[j];
            if(expected_scores[j] < 0){
                bullshit = true;
            } else {
                bool dummy;
                Traits::edge_descriptor cap_edge;
                tie(cap_edge, dummy) = edge(j + 1, j + 1 + n, matches);
                put(edge_capacity, matches, cap_edge, expected_scores[j]);
            }
        }
        if(bullshit){
            cout << "no\n";
            continue;
        }
        //for(int j = 0; j < num_vertices(matches); ++j){
        //    graph_traits<Graph>::out_edge_iterator ei, eiend;
        //    tie(ei, eiend) = out_edges(j, matches);
        //    for(; ei != eiend; ++ei){
        //        cout << "From: " << source(*ei, matches) << " To: " << target(*ei, matches) << " cap: " << get(edge_capacity, matches, *ei) << "\n";
        //    }
        //}
        int flow = push_relabel_max_flow(matches, 0, 2*n + 1);
        if(flow != unknown_count){
            cout << "no\n";
            continue;
        }
        bool possible = true;
        for(int j = 0; j < n; ++j){
            Traits::edge_descriptor cap_edge;
            bool dummy;
            tie(cap_edge, dummy) = edge(j + 1, j + 1 + n, matches);
            if(get(edge_residual_capacity, matches, cap_edge) != 0){
                possible = false;
                cout << "no\n";
                break;
            }
        }
        if(possible){
            cout << "yes\n";
        }


    }


}
