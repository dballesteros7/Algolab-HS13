#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/biconnected_components.hpp>
#include <queue>

using namespace std;
using namespace boost;

namespace boost
{
    struct edge_component_t
    {
        enum
        { 
            num = 555 
        };
        typedef edge_property_tag kind;
    }
    edge_component;
}

typedef adjacency_list<vecS,vecS,undirectedS, no_property, property<edge_component_t, int> > Graph;

int new_color(int colorA, int colorB){
    if(colorA == 0){
        if(colorB == 0){
            return 1;
        } else if (colorB == 1){
            return 2;
        } else if (colorB == 2){
            return 1;
        }
    } else if(colorA == 1){
        if(colorB == 0){
            return 2;
        } else if(colorB == 1){
            return 0;
        } else if(colorB == 2){
            return 0;
        }
    } else if(colorA == 2){
        if(colorB == 0){
            return 1;
        } else if(colorB == 1){
            return 0;
        } else if(colorB == 2){
            return 1;
        }
    }
    return -1;
}

int main(){

    int t;
    cin >> t;
    for(int i = 0; i < t; ++i){
        int n,m;
        cin >> n >> m;
        Graph friendship_is_magic(n);
        for(int j = 0; j < m; ++j){
            int a,b;
            cin >> a >> b;
            add_edge(a,b,friendship_is_magic);
        }
        property_map< Graph, edge_component_t >::type component = get(edge_component, friendship_is_magic);
        vector<int> articulation_points;
        vector<int> discover_time(n);
        vector<int> lowpoint(n);
        vector<int> predecessors(n);
        int num_comps;
        tie(num_comps, tuples::ignore) = biconnected_components(friendship_is_magic, component, std::back_inserter(articulation_points), discover_time_map(&discover_time[0]).lowpoint_map(&lowpoint[0]).predecessor_map(&predecessors[0]));
        if(num_comps != 1){
            cout << "no\n";
            continue;
        }
        int *colors = new int[n];
        int root;
        vector< vector<int> > successors(n, vector<int>());
        for(int j = 0; j < n; ++j){
            if(predecessors[j] != j){
                successors[predecessors[j]].push_back(j);
            } else {
                root = j;
                //cout << "Root: " << j << "\n";
            }
        }
        std::queue<int> to_visit;
        to_visit.push(root);
        colors[root] = 0;
        for(int j = 0; j < n; ++j){
            if(j != root){
                colors[j] = 1;
            }
        }
        int *reverse_discover_map = new int[n + 1];
        for(int j = 0; j < n; ++j){
            reverse_discover_map[discover_time[j]] = j;
        }
        while(!to_visit.empty()){
            int next = to_visit.front();
            to_visit.pop();
            for(int j = 0; j < successors[next].size(); ++j){
                int next_next = successors[next][j];
                int next_color = new_color(colors[predecessors[next_next]], colors[reverse_discover_map[lowpoint[next_next]]]);
                colors[next_next] = next_color;
                to_visit.push(next_next);
            }
        }
        vector<int> color_1;
        vector<int> color_2;
        vector<int> color_3;
        for(int j = 0; j < n; ++j){
            //cout << "For " << j << " color: " << colors[j] << "\n";
            if(colors[j] == 0){
                color_1.push_back(j);
            } else if(colors[j] == 1){
                color_2.push_back(j);
            } else{
                color_3.push_back(j);
            }
        }
        cout << "yes\n";
        cout << color_1.size();
        for(int j = 0; j < color_1.size(); ++j){
            cout << " " << color_1[j];
        }
        cout << "\n";
        cout << color_2.size();
        for(int j = 0; j < color_2.size(); ++j){
            cout << " " << color_2[j];
        }
        cout << "\n";
        cout << color_3.size();
        for(int j = 0; j < color_3.size(); ++j){
            cout << " " << color_3[j];
        }
        cout << "\n";
    }
}
