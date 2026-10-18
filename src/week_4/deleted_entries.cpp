#include <iostream>
#include <vector>

using namespace std;

typedef pair<int, int> E;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    int t;
    cin >> t;
    for(int i = 0; i < t; ++i){
        int n, m, k;
        cin >> n;
        cin >> m;
        cin >> k;
        int visited_count = 0;
        vector< vector<int> > vertices(n);
        vector<bool> visited(n, false);
        vector<int> color(n, 0);
        vector< vector<int> > colored(k);
        for(int j = 0; j < m; ++j){
            int a,b;
            cin >> a;
            cin >> b;
            vertices[a].push_back(b);
            vertices[b].push_back(a);
        }
        int current_color = 0;
        int first_vertex = 0;
        vector<E> to_visit;
        for(size_t j = 0; j < vertices[first_vertex].size(); ++j){
            to_visit.push_back(E(first_vertex, vertices[first_vertex][j]));
        }
        color[first_vertex] = current_color;
        colored[current_color].push_back(first_vertex);
        visited[first_vertex] = true;
        ++current_color;
        ++visited_count;
        while(!to_visit.empty()){
            E current = to_visit.back();
            to_visit.pop_back();
            if(!visited[current.second]){
                const vector<int> &next = vertices[current.second];
                for(size_t j = 0; j < next.size(); ++j){
                    if(!visited[next[j]]){
                        to_visit.push_back(E(current.second, next[j]));
                    }
                }
                if(color[current.first] == current_color % k){
                    ++current_color;
                }
                color[current.second] = current_color % k;
                colored[current_color % k].push_back(current.second);
                visited[current.second] = true;
                ++current_color;
                ++visited_count;
            }
        }
        if(visited_count != n){
            cout << "no\n";
        } else {
            if(current_color >= k){
                cout << "yes\n";
                for(int j = 0; j < k; j++){
                    cout << colored[j].size();
                    for(size_t z = 0; z < colored[j].size(); ++z){
                        cout << " " << colored[j][z];
                    }
                    cout << "\n";
                }
            } else {
                cout << "no\n";
            }
        }
    }
}
