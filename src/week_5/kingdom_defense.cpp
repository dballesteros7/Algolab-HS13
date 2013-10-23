#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
using namespace std;
using namespace boost;

struct vertex_demand_t {
    typedef vertex_property_tag kind;
};

typedef adjacency_list_traits<vecS, vecS, bidirectionalS> Traits;
typedef property<vertex_demand_t, int> DemandProperty;
typedef property<edge_capacity_t, int> EdgeCapacity;
typedef property<edge_residual_capacity_t, int, EdgeCapacity> EdgeResidualCap;
typedef property<edge_reverse_t, Traits::edge_descriptor, EdgeResidualCap> EdgeReverse;
typedef adjacency_list<vecS, vecS, bidirectionalS, DemandProperty, EdgeReverse> Graph;

int main(){
    ios_base::sync_with_stdio(false);
    int t;
    int MAX_CAP = 1000000000;
    cin >> t;
    for(int i = 0; i < t; ++i){
        int l, p;
        cin >> l;
        cin >> p;
        Graph kingdom(2*l + 2);
        for(int j = 0; j < l; ++j){
            int g, d;
            cin >> g;
            cin >> d;
            put(vertex_demand_t(), kingdom, j, -g);
            put(vertex_demand_t(), kingdom, j + l, d);
        }
        for(int j = 0; j < p; ++j){
            int fi, fj, c, bigC;
            cin >> fi;
            cin >> fj;
            cin >> c;
            cin >> bigC;
            if(fi != fj){
                Traits::edge_descriptor edge_foward, rev_edge;
                tie(edge_foward, tuples::ignore) = add_edge(fi + l, fj, kingdom);
                int d_fi = get(vertex_demand_t(), kingdom, fi + l);
                int d_fj = get(vertex_demand_t(), kingdom, fj);
                put(vertex_demand_t(), kingdom, fi + l, d_fi + c);
                put(vertex_demand_t(), kingdom, fj, d_fj - c);
                put(edge_capacity, kingdom, edge_foward, bigC - c);
                tie(rev_edge, tuples::ignore) = add_edge(fj, fi + l, kingdom);
                put(edge_capacity, kingdom, rev_edge, 0);
                put(edge_reverse, kingdom, edge_foward, rev_edge);
                put(edge_reverse, kingdom, rev_edge, edge_foward);
            } else {
                int supply = get(vertex_demand_t(), kingdom, fi);
                int demand = get(vertex_demand_t(), kingdom, fi + l);
                put(vertex_demand_t(), kingdom, fi, supply + c);
                put(vertex_demand_t(), kingdom, fi + l, demand - c);
            }
        }
        for(int j = 0; j < 2*l; ++j){
            int demand = get(vertex_demand_t(), kingdom, j);
            if(demand < 0){
                Traits::edge_descriptor edge_foward, rev_edge;
                tie(edge_foward, tuples::ignore) = add_edge(2*l, j, kingdom);
                put(edge_capacity, kingdom, edge_foward, -demand);
                tie(rev_edge, tuples::ignore) = add_edge(j, 2*l, kingdom);
                put(edge_capacity, kingdom, rev_edge, 0);
                put(edge_reverse, kingdom, edge_foward, rev_edge);
                put(edge_reverse, kingdom, rev_edge, edge_foward);
            } else if(demand > 0) {
                Traits::edge_descriptor edge_foward, rev_edge;
                tie(edge_foward, tuples::ignore) = add_edge(j, 2*l + 1, kingdom);
                put(edge_capacity, kingdom, edge_foward, demand);
                tie(rev_edge, tuples::ignore) = add_edge(2*l + 1, j, kingdom);
                put(edge_capacity, kingdom, rev_edge, 0);
                put(edge_reverse, kingdom, edge_foward, rev_edge);
                put(edge_reverse, kingdom, rev_edge, edge_foward);
            }
            if(j < l){
                if(demand < 0){
                    Traits::edge_descriptor inner_edge, rev_edge;
                    tie(inner_edge, tuples::ignore) = add_edge(j, j + l, kingdom);
                    put(edge_capacity, kingdom, inner_edge, MAX_CAP);
                    tie(rev_edge, tuples::ignore) = add_edge(j + l, j, kingdom);
                    put(edge_capacity, kingdom, rev_edge, 0);
                    put(edge_reverse, kingdom, inner_edge, rev_edge);
                    put(edge_reverse, kingdom, rev_edge, inner_edge);
                } else {
                    Traits::edge_descriptor inner_edge, rev_edge;
                    tie(inner_edge, tuples::ignore) = add_edge(j, j + l, kingdom);
                    put(edge_capacity, kingdom, inner_edge, 0);
                    tie(rev_edge, tuples::ignore) = add_edge(j + l, j, kingdom);
                    put(edge_capacity, kingdom, rev_edge, MAX_CAP);
                    put(edge_reverse, kingdom, inner_edge, rev_edge);
                    put(edge_reverse, kingdom, rev_edge, inner_edge);
                }
            }
        }
        //for(int j = 0; j < 2*l + 2; ++j){
        //    graph_traits<Graph>::out_edge_iterator ei, eiend;
        //    tie(ei, eiend) = out_edges(j, kingdom);
        //    cout << "Demand at " << j << ": " << get(vertex_demand_t(), kingdom, j) << "\n" ;
        //    for(; ei != eiend; ++ei){
        //        cout << "To: " << target(*ei, kingdom) << " Cap: " << get(edge_capacity, kingdom, *ei) << "\n";
        //    }
        //}
        int flow = push_relabel_max_flow(kingdom, 2*l, 2*l + 1);
        //graph_traits<Graph>::out_edge_iterator ei, eiend;
        //tie(ei, eiend) = out_edges(2*l, kingdom);
        bool bad = false;
        //for(; ei != eiend; ++ei){
        //    int res = get(edge_residual_capacity, kingdom, *ei);
        //    if(res != 0){
        //        cout << "no\n";
        //        bad = true;
        //        break;
        //    }
        //}
        //if(bad){
        //    continue;
        //}
        graph_traits<Graph>::in_edge_iterator ii, iiend;
        tie(ii, iiend) = in_edges(2*l + 1, kingdom);
        for(; ii != iiend; ++ii){
            int res = get(edge_residual_capacity, kingdom, *ii);
            //cout << edge_flow << "\n";
            if(res != 0){
                bad = true;
            }
        }
        if(!bad){
            cout << "yes\n";
        } else {
            cout << "no\n";
        }
    }
    return 0;
}
