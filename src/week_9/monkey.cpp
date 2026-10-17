#include <iostream>
#include <vector>
#include <limits>
#include <boost/graph/compressed_sparse_row_graph.hpp>
#include <boost/graph/strong_components.hpp>

using namespace std;
using namespace boost;

typedef compressed_sparse_row_graph<directedS> Graph;

int main(){
    ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    for(int i = 0; i < t; ++i){
        int n,m;
        cin >> n >> m;
        vector< pair<int,int> > roads(m);
        for(int j = 0; j < m; ++j){
            cin >> roads[j].first >> roads[j].second;
            --roads[j].first;
            --roads[j].second;
        }
        Graph island(edges_are_unsorted_multi_pass, roads.begin(), roads.end(), n);
        vector<int> costs(n);
        for(int j = 0; j < n; ++j){
            cin >> costs[j];
        }
        vector<int> componentNumbering(n);
        int numComponents = strong_components(island,
                make_iterator_property_map(componentNumbering.begin(), get(vertex_index, island)));
        // A component needs a station iff no road enters it from another component;
        // the station then goes to its cheapest location.
        vector<bool> reachable(numComponents, false);
        for(int j = 0; j < m; ++j){
            int from = componentNumbering[roads[j].first];
            int to = componentNumbering[roads[j].second];
            if(from != to){
                reachable[to] = true;
            }
        }
        vector<int> componentCosts(numComponents, numeric_limits<int>::max());
        for(int j = 0; j < n; ++j){
            if(costs[j] < componentCosts[componentNumbering[j]]){
                componentCosts[componentNumbering[j]] = costs[j];
            }
        }
        long long minimalCost = 0;
        for(int j = 0; j < numComponents; ++j){
            if(!reachable[j]){
                minimalCost += componentCosts[j];
            }
        }
        cout << minimalCost << "\n";
    }
    return 0;
}
