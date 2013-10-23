#include <iostream>
#include <queue>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>


using namespace boost;
using namespace std;


typedef pair< int, int> E;
typedef property< edge_weight_t, vector< int >  > EdgeWeight;
typedef property< vertex_distance_t, int > VertexDistance;
typedef property< vertex_color_t, bool> VertexColor;
typedef adjacency_list<vecS, vecS, undirectedS, VertexColor, EdgeWeight> Graph;
typedef adjacency_list<vecS, vecS, undirectedS, no_property, property< edge_weight_t, int > > SimpleGraph;
typedef graph_traits< Graph >::edge_descriptor ed;
typedef graph_traits< Graph >::vertex_descriptor vd;
typedef pair<ed, int> edge_with_weight;


struct compare_strict{
    bool operator () (edge_with_weight & a, edge_with_weight & b) const{
        return a.second > b.second;
    }
};

void buildPrivateNetwork(Graph &forest, int s, int h, Graph &minimalNetwork){
    priority_queue< edge_with_weight, vector<edge_with_weight>, compare_strict > to_visit;
    graph_traits< Graph >::vertex_iterator v, v_end;
    tie(v,v_end) = vertices(forest);
    for(; v != v_end; ++v){
        put(vertex_color, forest, *v, false);
    }

    graph_traits< Graph >::out_edge_iterator e, e_end;
    tie(e, e_end) = out_edges(h, forest);
    for(; e != e_end; ++e){
        to_visit.push(edge_with_weight(*e, get(edge_weight, forest, *e)[s]));
    }
    put(vertex_color, forest, h, true);
    while(!to_visit.empty()){
        edge_with_weight  current = to_visit.top();
        to_visit.pop();
        bool current_color = get(vertex_color, forest, target(current.first, forest));
        if(current_color){
            continue;
        }
        //cout << "Best next route for the network is from: " << source(current.first, forest) << " to: " << target(current.first, forest) << " with cost " << current.second << "\n";
        pair<ed, bool> x = edge(source(current.first, forest), target(current.first, forest), minimalNetwork);
        if(x.second){
            int cost = get(edge_weight, minimalNetwork, x.first)[0];
            if(cost >  get(edge_weight, forest, current.first)[s]){
                put(edge_weight, minimalNetwork, x.first, vector<int>(1, get(edge_weight, forest, current.first)[s]));
            } 
        } else {
            vector<int> tmp_vector(1, get(edge_weight, forest, current.first)[s]);
            add_edge(source(current.first, forest), target(current.first, forest), tmp_vector, minimalNetwork);
        }
        graph_traits< Graph >::out_edge_iterator e, e_end;
        tie(e, e_end) = out_edges(target(current.first, forest), forest);
        for(; e != e_end; ++e){
            int vertex = target(*e, forest);
            bool color = get(vertex_color, forest, vertex);
            if(!color){
                to_visit.push(edge_with_weight(*e, get(edge_weight, forest, *e)[s]));
            } 
        }
        put(vertex_color, forest, target(current.first, forest), true);
    }
}

int main(){
    int n;

    cin >> n;
    for(int i = 0; i < n; i++){
        int t,e,s,a,b;
        cin >> t;
        cin >> e;
        cin >> s;
        cin >> a;
        cin >> b;
        vector<Graph> networks(s, Graph(t));
        Graph forest(t);
        Graph minimalNetwork(t);
        for(int j = 0; j < e; ++j){
            int e1, e2;
            cin >> e1;
            cin >> e2;
            vector< int > weights(s, 0);
            for(int k = 0; k < s;++k){
                cin >> weights[k];
            }
            add_edge(e1, e2, weights, forest);
        }
        //cout << "Size: " << s << "\n";
        for(int j = 0; j < s; ++j){
            int h;
            cin >> h;
            //cout << "Calling with hive: " << h << "\n";
            buildPrivateNetwork(forest, j, h, minimalNetwork);
        }
        graph_traits< Graph >::edge_iterator e_start, e_end;
        tie(e_start, e_end) = edges(minimalNetwork);
        SimpleGraph result(t);
        for(; e_start != e_end; ++e_start){
            add_edge(source(*e_start, minimalNetwork), target(*e_start, minimalNetwork), get(edge_weight, minimalNetwork, *e_start)[0], result);
        }
        vector<vd> p(num_vertices(result));
        vector<int> d(num_vertices(result));
        vd start_point = vertex(a, result);
        dijkstra_shortest_paths(result, start_point, predecessor_map(&p[0]).distance_map(&d[0]));
        cout << d[b] << "\n";
    }



    return 0;
}
