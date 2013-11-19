#include <iostream>
#include <vector>
#include <queue>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/strong_components.hpp>

using namespace std;
using namespace boost;

typedef adjacency_list<vecS,vecS,bidirectionalS> Graph;
typedef graph_traits<Graph>::out_edge_iterator OutEdgeIter;

int main(){
    ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    for(int i = 0; i < t; ++i){
        int n,m;
        cin >> n >> m;
        Graph island(n);
        for(int j = 0; j < m; ++j){
            int a,b;
            cin >> a >> b;
            if(a == b){
                continue;
            }
            bool exists = false;
            tie(tuples::ignore, exists) = edge(a - 1, b - 1, island);
            if(!exists){
                add_edge(a - 1, b - 1, island);
            }
        }
        int *costs = new int[n];
        for(int j = 0; j < n; ++j){
            cin >> costs[j];
        }
        vector<int> componentNumbering(n);
        int numComponents = strong_components(island, &componentNumbering[0]);
        vector<int> componentCosts(numComponents, 100001);
        for(int j = 0; j < n; ++j){
           if(costs[j] < componentCosts[componentNumbering[j]]){
                componentCosts[componentNumbering[j]] = costs[j];
           }
        }
        Graph betterIsland(numComponents);
        for(int j = 0; j < n; ++j){
            OutEdgeIter outEdgeBegin, outEdgeEnd;
            tie(outEdgeBegin, outEdgeEnd) = out_edges(j, island);
            for(; outEdgeBegin != outEdgeEnd; ++outEdgeBegin){
                int targetV;
                targetV = target(*outEdgeBegin, island);
                if(componentNumbering[j] != componentNumbering[targetV]){
                    bool exists;
                    tie(tuples::ignore, exists) = edge(componentNumbering[j], componentNumbering[targetV], betterIsland);
                    if(!exists){
                        add_edge(componentNumbering[j], componentNumbering[targetV], betterIsland);
                    }
                }
            }
        }
        int minimalCost = 0;
        for(int j = 0; j < numComponents; ++j){
            if(in_degree(j, betterIsland) == 0){
                minimalCost += componentCosts[j];
            }
        }
        cout << minimalCost << "\n";
    }
    return 0;
}
