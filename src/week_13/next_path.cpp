#include <boost/graph/adjacency_list.hpp>
#include <iostream>
#include <map>
#include <queue>


using namespace std;
using namespace boost;

typedef adjacency_list<vecS, vecS, directedS> Graph;

int distance_to_target(const Graph &network, int start, bool *visited_in_p, int t, int n){
    if(start == t)
        return 0;
    bool *visited = new bool[n];
    int *distances = new int[n];
    for(int i = 0; i < n; ++i){
        visited[i] = visited_in_p[i];
    }
    distances[start] = 0;
    queue<int> to_visit;
    to_visit.push(start);
    while(!to_visit.empty()){
        int next = to_visit.front();
        to_visit.pop();
        graph_traits<Graph>::out_edge_iterator ei, e_end;
        for(tie(ei, e_end) = out_edges(next, network); ei != e_end; ++ei){
            int next_next = target(*ei, network);
            if(next_next == t){
                return distances[next] + 1;
            }
            if(!visited[next_next]){
                to_visit.push(next_next);
                visited[next_next] = true;
                distances[next_next] = distances[next] + 1;
            }
        }
    }
    return 10000000;
}

int main(){
    int t;
    cin >> t;
    for(int i = 0; i < t; ++i){
        int n, m;
        cin >> n >> m;
        int s, tar;
        cin >> s >> tar;
        --s;
        --tar;
        Graph network(n);
        Graph reverse_network(n);
        for(int j = 0; j < m; ++j){
            int a,b;
            cin >> a >> b;
            --a;
            --b;
            if(a == b)
                continue;
            add_edge(a,b, network);
            add_edge(b,a, reverse_network);
        }
        bool *visited = new bool[n];
        for(int j = 0; j < n; ++j){
            visited[j] = false;
        }
        queue<int> to_visit;
        int *predecessors = new int[n];
        int *distance_from_source = new int[n];
        for(int j = 0; j < n; ++j){
            visited[j] = false;
            predecessors[j] = j;
            distance_from_source[j] = 10000000;
        }
        visited[s] = true;
        distance_from_source[s] = 0;
        bool condition = true;
        to_visit.push(s);
        while(!to_visit.empty() && condition){
            int new_vertex = to_visit.front();
            to_visit.pop();
            graph_traits<Graph>::out_edge_iterator ei, e_end;
            for(tie(ei, e_end) = out_edges(new_vertex, network); ei != e_end; ++ei){
                int target_v = target(*ei, network);
                if(target_v == tar){
                    condition = false;
                    predecessors[target_v] = new_vertex;
                    distance_from_source[target_v] = distance_from_source[new_vertex] + 1;
                    break;
                }
                if(!visited[target_v]){
                    to_visit.push(target_v);
                    visited[target_v] = true;
                    predecessors[target_v] = new_vertex;
                    distance_from_source[target_v] = distance_from_source[new_vertex] + 1;
                }
            }
        }
        if(condition){
            cout << "no\n";
            continue;
        }
        int *successors = new int[n];
        for(int j = 0; j < n; ++j){
            successors[j] = j;
        }
        int next_to_visit = tar;
        while(next_to_visit != s){
            successors[predecessors[next_to_visit]] = next_to_visit;
            next_to_visit = predecessors[next_to_visit];
        }
        bool *visited_in_p = new bool[n];
        for(int j = 0; j < n; ++j){
            visited_in_p[j] = false;
        }
        next_to_visit = s;
        int min_cost_2 = 1000000;
        do{
            visited_in_p[next_to_visit] = true;
            graph_traits<Graph>::out_edge_iterator ei, e_end;
            for(tie(ei, e_end) = out_edges(next_to_visit, network); ei != e_end; ++ei){
                int possible_exit = target(*ei, network);
                if(possible_exit != successors[next_to_visit]){
                    int cost_from_there = distance_from_source[next_to_visit] + 1 + distance_to_target(network, possible_exit, visited_in_p, tar, n);
                    if(cost_from_there < min_cost_2){
                        min_cost_2 = cost_from_there;
                    }
                }
            }
            next_to_visit = successors[next_to_visit];
        } while (next_to_visit != tar);
        if(min_cost_2 == 1000000){
            cout << "no\n";
        } else {
            cout << min_cost_2 << "\n";
        }
    }
}
