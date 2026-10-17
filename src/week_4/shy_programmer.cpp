#include <iostream>
#include <vector>
#include <algorithm>
#include <boost/graph/adjacency_list.hpp>

#include <boost/graph/boyer_myrvold_planar_test.hpp>

using namespace std;
using namespace boost;

typedef adjacency_list<vecS, vecS, undirectedS> Graph;
typedef pair<int, int> E;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    int t;
    cin >> t;
    for(int i = 0; i < t; ++i){
        int n,m;
        cin >> n;
        cin >> m;
        vector<E> friendships(m);
        for(int j = 0; j < m; ++j){
            int p1, p2;
            cin >> p1;
            cin >> p2;
            friendships[j] = E(min(p1, p2), max(p1, p2));
        }
        // Everyone reaches the outside iff the graph plus one vertex joined to
        // all programmers is planar, i.e. the graph is outerplanar. A simple
        // outerplanar graph has at most 2n - 3 edges, which settles dense
        // inputs without running the planarity test.
        int max_edges = max(0, 2*n - 3);
        if(m > max_edges){
            sort(friendships.begin(), friendships.end());
            friendships.erase(unique(friendships.begin(), friendships.end()), friendships.end());
            int simple_edges = 0;
            for(size_t j = 0; j < friendships.size(); ++j){
                if(friendships[j].first != friendships[j].second){
                    ++simple_edges;
                }
            }
            if(simple_edges > max_edges){
                cout << "no\n";
                continue;
            }
        }
        Graph programmers(n+1);
        for(size_t j = 0; j < friendships.size(); ++j){
            add_edge(friendships[j].first, friendships[j].second, programmers);
        }
        for(int j = 0; j < n; ++j){
            add_edge(j, n, programmers);
        }
        bool is_planar = boyer_myrvold_planarity_test(programmers);
        if(is_planar)
            cout << "yes\n";
        else
            cout << "no\n";
    }


}
