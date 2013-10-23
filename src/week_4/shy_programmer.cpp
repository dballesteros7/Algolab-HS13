#include <iostream>
#include <boost/graph/adjacency_list.hpp>

#include <boost/graph/boyer_myrvold_planar_test.hpp>

using namespace std;
using namespace boost;

typedef adjacency_list<vecS, vecS, undirectedS> Graph;

int main(){
    int t;
    cin >> t;
    for(int i = 0; i < t; ++i){
        int n,m;
        cin >> n;
        cin >> m;
        Graph programmers(n+1);
        bool *infinite_added = new bool[n];
        for(int j = 0; j < n; ++j){
            infinite_added[j] = false;
        }
        for(int j = 0; j < m; ++j){
            int p1, p2;
            cin >> p1;
            cin >> p2;
            add_edge(p1, p2, programmers);
            if(!infinite_added[p1])
                add_edge(p1, n, programmers);
            if(!infinite_added[p2])
                add_edge(p2, n, programmers);
        }
        bool is_planar = boyer_myrvold_planarity_test(programmers);
        if(is_planar)
            cout << "yes\n";
        else
            cout << "no\n";
    }    


}
