#include <iostream>
#include <queue>
#include <vector>
#include <climits>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>


using namespace boost;
using namespace std;


typedef adjacency_list<vecS, vecS, undirectedS, no_property, property< edge_weight_t, int > > SimpleGraph;
typedef graph_traits< SimpleGraph >::vertex_descriptor vd;
typedef pair<int, int> E;


struct edge_with_weight{
    int weight;
    int target;
    int index;
    edge_with_weight(int w, int t, int i) : weight(w), target(t), index(i) {}
};

struct compare_strict{
    bool operator () (const edge_with_weight & a, const edge_with_weight & b) const{
        return a.weight > b.weight;
    }
};

// Prim from the hive: repeatedly take the cheapest edge leaving the network.
// best[index] keeps the cheapest time any species offers on that edge.
void buildPrivateNetwork(const vector< vector<E> > &forest, const int *weights, int h, vector<bool> &visited, vector<int> &best){
    priority_queue< edge_with_weight, vector<edge_with_weight>, compare_strict > to_visit;
    fill(visited.begin(), visited.end(), false);
    for(size_t j = 0; j < forest[h].size(); ++j){
        to_visit.push(edge_with_weight(weights[forest[h][j].second], forest[h][j].first, forest[h][j].second));
    }
    visited[h] = true;
    while(!to_visit.empty()){
        edge_with_weight current = to_visit.top();
        to_visit.pop();
        if(visited[current.target]){
            continue;
        }
        if(current.weight < best[current.index]){
            best[current.index] = current.weight;
        }
        const vector<E> &next = forest[current.target];
        for(size_t j = 0; j < next.size(); ++j){
            if(!visited[next[j].first]){
                to_visit.push(edge_with_weight(weights[next[j].second], next[j].first, next[j].second));
            }
        }
        visited[current.target] = true;
    }
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    int n;

    cin >> n;
    for(int i = 0; i < n; i++){
        int t,e,s,a,b;
        cin >> t;
        cin >> e;
        cin >> s;
        cin >> a;
        cin >> b;
        // forest[v] lists (neighbour, edge index) in input order; species k
        // reads its times from weights[k*e + edge index].
        vector< vector<E> > forest(t);
        vector<E> endpoints(e);
        vector<int> weights(s * e);
        for(int j = 0; j < e; ++j){
            int e1, e2;
            cin >> e1;
            cin >> e2;
            endpoints[j] = E(e1, e2);
            forest[e1].push_back(E(e2, j));
            forest[e2].push_back(E(e1, j));
            for(int k = 0; k < s;++k){
                cin >> weights[k * e + j];
            }
        }
        vector<int> best(e, INT_MAX);
        vector<bool> visited(t);
        for(int j = 0; j < s; ++j){
            int h;
            cin >> h;
            buildPrivateNetwork(forest, weights.data() + j * e, h, visited, best);
        }
        SimpleGraph result(t);
        for(int j = 0; j < e; ++j){
            if(best[j] != INT_MAX){
                add_edge(endpoints[j].first, endpoints[j].second, best[j], result);
            }
        }
        vector<int> d(t);
        dijkstra_shortest_paths(result, a, distance_map(&d[0]));
        cout << d[b] << "\n";
    }



    return 0;
}
