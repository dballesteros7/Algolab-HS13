#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

#include <iostream>

using namespace std;
using namespace boost;

typedef adjacency_list < listS, vecS, directedS, no_property, property < edge_weight_t, int > > Graph;
typedef pair<int, int> Edge;

int main(){
    int t;
    cin >> t;
    for(int i = 0; i < t; ++i){
        int n, m;
        cin >> n >> m;
        int s, t;
        cin >> s >> t;
        Graph weird_graph(4*n);
        for(int j = 0; j < m; ++j){
            int s_i, t_i, w;
            cin >> s_i >> t_i >> w;
            if(w % 2 == 0){
                add_edge(s_i*4, 4*t_i + 3, w, weird_graph);
                add_edge(4*s_i + 1, 4*t_i + 2, w, weird_graph);
                add_edge(4*s_i + 2, 4*t_i + 1, w, weird_graph);
                add_edge(4*s_i + 3, 4*t_i, w, weird_graph);
            } else {
                add_edge(4*s_i, 4*t_i + 1, w, weird_graph);
                add_edge(4*s_i + 1, 4*t_i, w, weird_graph);
                add_edge(4*s_i + 2, 4*t_i + 3, w, weird_graph);
                add_edge(4*s_i + 3, 4*t_i + 2, w, weird_graph);
            }
        }
        vector<int> p(4*n);
        vector<int> d(4*n);
        dijkstra_shortest_paths(weird_graph, 4*s + 1, predecessor_map(&p[0]).distance_map(&d[0]));
        if(d[4*t] == numeric_limits<int>::max()){
            cout << "no\n";
        } else {
            cout << d[4*t] << "\n";
        }
    }
}
