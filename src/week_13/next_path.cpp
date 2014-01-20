#include <boost/graph/adjacency_list.hpp>
#include <iostream>
#include <map>
#include <queue>
#include <limits>
#include <list>
#include <vector>

using namespace std;
using namespace boost;

typedef adjacency_list<vecS, vecS, directedS> Graph;

const int MAX = numeric_limits<int>::max();

int distance_to_target(const Graph &network, int start, int t, int n, vector<int> &predators){
    if(start == t)
        return 0;
    bool *visited = new bool[n];
    int *distances = new int[n];
    for(int i = 0; i < n; ++i){
        visited[i] = false;
        predators.push_back(-1);
    }
    distances[start] = 0;
    queue<int> to_visit;
    to_visit.push(start);
    visited[start] = true;
    while(!to_visit.empty()){
        int next = to_visit.front();
        to_visit.pop();
        graph_traits<Graph>::out_edge_iterator ei, e_end;
        for(tie(ei, e_end) = out_edges(next, network); ei != e_end; ++ei){
            int next_next = target(*ei, network);
            if(next_next == t){
                predators[next_next] = next;
                return distances[next] + 1;
            }
            if(!visited[next_next]){
                to_visit.push(next_next);
                visited[next_next] = true;
                distances[next_next] = distances[next] + 1;
                predators[next_next] = next;
            }
        }
    }
    return -1;
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
        for(int j = 0; j < m; ++j){
            int a,b;
            cin >> a >> b;
            --a;
            --b;
            add_edge(a,b, network);
        }
        vector<int> shortestPredators;
        int min_cost = distance_to_target(network, s, tar, n, shortestPredators);
        if(min_cost < 0){
            cout << "no\n";
            continue;
        }
        vector<int> P(min_cost + 1);
        P[min_cost] = tar;
        int current = tar;
        for(int j = min_cost - 1; j >= 0; --j){
            P[j] = shortestPredators[current];
            current = shortestPredators[current];
        }
        int min_cost_2 = MAX;
        for(int j = 0; j < min_cost + 1; ++j){
            graph_traits<Graph>::out_edge_iterator ei, e_end;
            for(tie(ei, e_end) = out_edges(P[j], network); ei != e_end; ++ei){
                if(j == min_cost || target(*ei, network) != P[j + 1]){
                    vector<int> dummy;
                    int another_cost = distance_to_target(network, target(*ei, network), tar, n, dummy);
                    if(another_cost >= 0 && another_cost + 1 + j < min_cost_2)
                        min_cost_2 = another_cost + 1 + j;
                }
            }
        }
        if(min_cost_2 == MAX)
            cout << "no\n";
        else
            cout << min_cost_2 << "\n";
    }
}
