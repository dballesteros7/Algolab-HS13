#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

using namespace std;
using namespace boost;


int main(){

    typedef pair< int, int> E;
    typedef adjacency_list< vecS, vecS, undirectedS, no_property, property< edge_weight_t, int > > G;
    typedef graph_traits< G >::vertex_descriptor vertex_descriptor;
    int t;
    int n, m;
    int e1, e2;
    int w;
    cin >> t;

    for(int i = 0; i < t; ++i){
        cin >> n;
        cin >> m;
        G graph(n);
        for(int j = 0; j < m; ++j){
            cin >> e1;
            cin >> e2;
            cin >> w;
            add_edge(e1, e2, w, graph);
        }
        vector < graph_traits< G >::vertex_descriptor > parents(num_vertices(graph));
        prim_minimum_spanning_tree(graph, &parents[0]);
        int total_weight = 0;
        graph_traits< G >::edge_descriptor e;
        for (size_t j = 0; j < parents.size(); ++j){
            if(parents[j] != j){
                bool exists;
                tie(e, exists) = edge(j, parents[j], graph);
                total_weight += get(edge_weight, graph, e);
            }
        }
        std::vector<vertex_descriptor> p(num_vertices(graph));
        std::vector<int> d(num_vertices(graph));
        vertex_descriptor s = vertex(0, graph);
        dijkstra_shortest_paths(graph, s, predecessor_map(&p[0]).distance_map(&d[0]));
        cout << total_weight << " " << *max_element(d.begin(), d.end()) << "\n";
        
    }

}
