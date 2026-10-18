#include <iostream>
#include <vector>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
using namespace std;
using namespace boost;

typedef adjacency_list_traits<vecS, vecS, directedS> Traits;
typedef property<edge_capacity_t, long> EdgeCapacity;
typedef property<edge_residual_capacity_t, long, EdgeCapacity> EdgeResidualCap;
typedef property<edge_reverse_t, Traits::edge_descriptor, EdgeResidualCap> EdgeReverse;
typedef adjacency_list<vecS, vecS, directedS, no_property, EdgeReverse> Graph;

void add_capacity_edge(Graph &g, int from, int to, long capacity){
    Traits::edge_descriptor edge_foward, rev_edge;
    tie(edge_foward, tuples::ignore) = add_edge(from, to, g);
    tie(rev_edge, tuples::ignore) = add_edge(to, from, g);
    put(edge_capacity, g, edge_foward, capacity);
    put(edge_capacity, g, rev_edge, 0);
    put(edge_reverse, g, edge_foward, rev_edge);
    put(edge_reverse, g, rev_edge, edge_foward);
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    int t;
    cin >> t;
    for(int i = 0; i < t; ++i){
        int l, p;
        cin >> l;
        cin >> p;
        // balance[j] = soldiers available at j minus soldiers that must leave
        // or stay at j. The c soldiers forced along a path are taken from fi
        // and handed to fj, so only the optional C - c remain as edges.
        // Soldiers may walk through locations, hence one vertex per location.
        vector<long> balance(l);
        for(int j = 0; j < l; ++j){
            int g, d;
            cin >> g;
            cin >> d;
            balance[j] = g - d;
        }
        Graph kingdom(l + 2);
        int source = l;
        int sink = l + 1;
        for(int j = 0; j < p; ++j){
            int fi, fj, c, bigC;
            cin >> fi;
            cin >> fj;
            cin >> c;
            cin >> bigC;
            if(fi != fj){
                balance[fi] -= c;
                balance[fj] += c;
                if(bigC > c){
                    add_capacity_edge(kingdom, fi, fj, bigC - c);
                }
            }
        }
        long needed = 0;
        for(int j = 0; j < l; ++j){
            if(balance[j] > 0){
                add_capacity_edge(kingdom, source, j, balance[j]);
            } else if(balance[j] < 0){
                add_capacity_edge(kingdom, j, sink, -balance[j]);
                needed -= balance[j];
            }
        }
        long flow = push_relabel_max_flow(kingdom, source, sink);
        if(flow == needed){
            cout << "yes\n";
        } else {
            cout << "no\n";
        }
    }
    return 0;
}
