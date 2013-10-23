#include <iostream>
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

struct compare_strict{
    bool operator () (const pair<int,int> &a, const pair<int,int> &b) const{
        if(a.first < b.first){
            return true;
        } else if (a.first == b.first){
            return  a.second < b.second;
        } else {
            return false;
        }

    }

};
int main(){
    int t;
    cin >> t;
    for(int i = 0; i < t; i++){
        int n,m;
        cin >> n;
        cin >> m;
        Graph cityNetwork(n);
        for(int j = 0; j < m; j++){
            int e1,e2;
            cin >> e1;
            cin >> e2;
            add_edge(e1,e2,cityNetwork);
        }
        property_map < Graph, edge_component_t >::type component = get(edge_component, cityNetwork);
        int num_comps = biconnected_components(cityNetwork, component);
        int *edgesPerComp = new int[num_comps];
        for(int j = 0; j < num_comps; ++j){
            edgesPerComp[j] = 0;
        }
        vector< vector < pair<int, int> > > inverse_component(num_comps, vector< pair<int,int> >());
        graph_traits< Graph >::edge_iterator e_i, e_end;
        tie(e_i, e_end) = edges(cityNetwork);
        for(; e_i != e_end; ++e_i){
            edgesPerComp[component[*e_i]]++;
            if(source(*e_i, cityNetwork) < target(*e_i, cityNetwork)){
                inverse_component[component[*e_i]].push_back(pair<int,int>(source(*e_i, cityNetwork), target(*e_i, cityNetwork)));
            } else {
                inverse_component[component[*e_i]].push_back(pair<int,int>(target(*e_i, cityNetwork), source(*e_i, cityNetwork)));
            }
        }
        set< pair<int, int>, compare_strict > result;
        int criticalBridges = 0;
        for(int j = 0; j < num_comps; ++j){
            if(edgesPerComp[j] == 1){
                ++criticalBridges;
                result.insert(inverse_component[j][0]);
            }
        }
        cout << criticalBridges << "\n";
        set< pair< int, int > >::iterator it;
        for(it = result.begin(); it != result.end(); ++it){
            cout << (*it).first << " " << (*it).second << "\n";
        }
    }
    return 0;

}
