#include <iostream>
#include <stack>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

using namespace std;
using namespace boost;

typedef adjacency_list<vecS, vecS, undirectedS> Graph;

int main(){
    ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    for(int i = 0; i < t; ++i){
        int g, s, l;
        cin >> g;
        cin >> s;
        cin >> l;

        Graph network(g + s);

        for(int j = 0; j < l; ++j){
            int a,b;
            cin >> a;
            cin >> b;
            add_edge(a, b + g, network);
        }
        vector<graph_traits<Graph>::vertex_descriptor> mate(g + s);
        bool success = checked_edmonds_maximum_cardinality_matching(network, &mate[0]);
        assert(success);
        bool *visited = new bool[g + s];
        for(int j = 0; j < s + g; ++j){
            visited[j] = false;
        }
        stack<int> to_visit;
        for(int j = 0; j < g; ++j){
            if(get(&mate[0], j) == graph_traits<Graph>::null_vertex()){
                to_visit.push(j);
            }
        }
        while(!to_visit.empty()){
            int current = to_visit.top();
            to_visit.pop();
            if(visited[current])
                continue;
            if(current < g){
                int matched = get(&mate[0], current);
                if(matched == graph_traits<Graph>::null_vertex()){
                    graph_traits<Graph>::out_edge_iterator ei, ei_end;
                    tie(ei, ei_end) = out_edges(current, network);
                    for(; ei != ei_end; ++ei){
                        if(!visited[target(*ei, network)])
                            to_visit.push(target(*ei, network));
                    }
                } else {
                    graph_traits<Graph>::out_edge_iterator ei, ei_end;
                    tie(ei, ei_end) = out_edges(current, network);
                    for(; ei != ei_end; ++ei){
                        if(!visited[target(*ei, network)] && target(*ei, network) != matched)
                            to_visit.push(target(*ei, network));
                    }
                }
            } else {
                int matched = get(&mate[0], current);
                if(matched != graph_traits<Graph>::null_vertex()){
                    if(!visited[matched])
                        to_visit.push(matched);
                }
            }
            visited[current] = true;
        }
        int ground_cover = 0;
        vector<int> ground_cover_list;
        int sat_cover = 0;
        vector<int> sat_cover_list;
        for(int j = 0; j < g; ++j){
            if(!visited[j]){
                ++ground_cover;
                ground_cover_list.push_back(j);
            }
        }
        for(int j = g; j < s + g; ++j){
            if(visited[j]){
                ++sat_cover;
                sat_cover_list.push_back(j - g);
            }
        }
        cout << ground_cover << " " << sat_cover << "\n";
        for(int j = 0; j < ground_cover_list.size(); ++j){
            cout << ground_cover_list[j] << " ";
        }
        for(int j = 0; j < sat_cover_list.size(); ++j){
            if(j == sat_cover_list.size() - 1){
                cout << sat_cover_list[j] << "\n";
            } else {
                cout << sat_cover_list[j] << " ";
            }
        }
        if(sat_cover_list.size() == 0){
            cout << "\n";
        }
    }

}

