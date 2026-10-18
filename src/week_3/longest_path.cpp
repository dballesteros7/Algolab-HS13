#include <iostream>
#include <vector>
#include <utility>

using namespace std;

// Breadth first search from source; returns the farthest node and stores
// its distance (in edges) in max_distance.
int farthest_node(int source, const vector<int>& first, const vector<int>& neighbours,
                  vector<int>& distance, vector<int>& to_visit, int& max_distance){
    int N = distance.size();
    for(int j = 0; j < N; j++){
        distance[j] = -1;
    }
    int head = 0, tail = 0;
    distance[source] = 0;
    to_visit[tail++] = source;
    int node = source;
    while(head < tail){
        node = to_visit[head++];
        for(int k = first[node]; k < first[node + 1]; k++){
            int child = neighbours[k];
            if(distance[child] == -1){
                distance[child] = distance[node] + 1;
                to_visit[tail++] = child;
            }
        }
    }
    // The last node dequeued is at maximum distance
    max_distance = distance[node];
    return node;
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    int t, N;
    vector< pair<int,int> > edges;
    // Adjacency in compressed form: neighbours of j are
    // neighbours[first[j]..first[j+1]-1]
    vector<int> first, neighbours, distance, to_visit;
    cin >> t;
    for(int i = 0; i < t; i++){
        cin >> N;
        edges.resize(N - 1);
        first.assign(N + 1, 0);
        for(int j = 0; j < N - 1; j++){
            cin >> edges[j].first;
            cin >> edges[j].second;
            first[edges[j].first + 1]++;
            first[edges[j].second + 1]++;
        }
        for(int j = 0; j < N; j++){
            first[j + 1] += first[j];
        }
        neighbours.resize(2*(N - 1));
        distance.resize(N);
        to_visit.resize(N);
        // Use to_visit as the insertion cursor while filling neighbours
        for(int j = 0; j < N; j++){
            to_visit[j] = first[j];
        }
        for(int j = 0; j < N - 1; j++){
            neighbours[to_visit[edges[j].first]++] = edges[j].second;
            neighbours[to_visit[edges[j].second]++] = edges[j].first;
        }
        // Diameter of a tree: farthest node from the node farthest from any node
        int length;
        int end = farthest_node(0, first, neighbours, distance, to_visit, length);
        farthest_node(end, first, neighbours, distance, to_visit, length);
        cout << length + 1 << "\n";
    }
    return 0;
}
