#include <iostream>
#include <vector>
#include <algorithm>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/biconnected_components.hpp>

using namespace std;
using namespace boost;

namespace boost
{
  struct edge_component_t
  {
    enum
    { num = 555 };
    typedef edge_property_tag kind;
  }
  edge_component;
}

typedef property< edge_component_t, int > EdgeProperty;
typedef adjacency_list<vecS, vecS, undirectedS, no_property, EdgeProperty> Graph;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    int t;
    cin >> t;
    for(int i = 0; i < t; i++){
        int n,m;
        cin >> n;
        cin >> m;
        // Islands are numbered 1..n.
        Graph cityNetwork(n + 1);
        for(int j = 0; j < m; j++){
            int e1,e2;
            cin >> e1;
            cin >> e2;
            add_edge(e1,e2,cityNetwork);
        }
        property_map < Graph, edge_component_t >::type component = get(edge_component, cityNetwork);
        int num_comps = biconnected_components(cityNetwork, component);
        // A bridge is a biconnected component made of a single edge.
        vector<int> edgesPerComp(num_comps, 0);
        graph_traits< Graph >::edge_iterator e_i, e_end;
        for(tie(e_i, e_end) = edges(cityNetwork); e_i != e_end; ++e_i){
            edgesPerComp[component[*e_i]]++;
        }
        vector< pair<int, int> > result;
        for(tie(e_i, e_end) = edges(cityNetwork); e_i != e_end; ++e_i){
            if(edgesPerComp[component[*e_i]] == 1){
                int u = source(*e_i, cityNetwork);
                int v = target(*e_i, cityNetwork);
                result.push_back(pair<int,int>(min(u, v), max(u, v)));
            }
        }
        sort(result.begin(), result.end());
        cout << result.size() << "\n";
        for(size_t j = 0; j < result.size(); ++j){
            cout << result[j].first << " " << result[j].second << "\n";
        }
    }
    return 0;

}
