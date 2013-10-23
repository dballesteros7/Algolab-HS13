#include <iostream>
#include <stack>
#include <set>
#include <deque>
#include <vector>

using namespace std;

typedef pair<int, int> E;

int main(){
    int t;
    cin >> t;
    for(int i = 0; i < t; ++i){
        int n, m, k;
        cin >> n;
        cin >> m;
        cin >> k;
        int visited_count = 0;
        vector< deque<E> > vertices(n, deque<E>());
        bool* visited = new bool[n];
        int *color = new int[n];
        vector< int > *colored = new vector<int>[k];
        for(int j = 0; j < n; ++j){
            visited[j] = false;
            color[j] = 0;
        }
        for(int j = 0; j < k; ++j){
            colored[j] = vector<int>();
        }
        for(int j = 0; j < m; ++j){
            int a,b;
            cin >> a;
            cin >> b;
            E tmp(a,b);
            E tmp2(b,a);
            vertices[a].push_back(tmp);
            vertices[b].push_back(tmp2);
        }
        int current_color = 0;
        int first_vertex = 0;
        stack<E> to_visit;
        for(int j = 0; j < vertices[first_vertex].size(); ++j){
            to_visit.push(vertices[first_vertex][j]);
        }
        color[first_vertex] = current_color;
        colored[current_color].push_back(first_vertex);
        visited[first_vertex] = true;
        ++current_color;
        ++visited_count;
        while(!to_visit.empty()){
            E current = to_visit.top();
            to_visit.pop();
            if(!visited[current.second]){
                for(int j = 0; j < vertices[current.second].size(); ++j){
                    E nextEdge = vertices[current.second][j];
                    if(!visited[nextEdge.second]){
                        to_visit.push(nextEdge);
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
                    for(int z = 0; z < colored[j].size(); ++z){
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
