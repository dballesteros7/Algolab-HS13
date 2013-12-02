#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

using namespace std;
using namespace boost;

typedef adjacency_list<vecS, vecS, undirectedS> Graph;

int main(){
    ios_base::sync_with_stdio(false);
    int n;
    cin >> n;
    for(int i = 0; i < n; ++i){
        int w,h;
        cin >> w >> h;
        bool **floorplan = new bool*[ h];
        for(int j = 0 ; j < h; ++j){
            floorplan[j] = new bool[w];
            for(int z = 0; z < w; ++z){
                char p;
                cin >> p;
                if(p == '.'){
                    floorplan[j][z] = true;
                } else {
                    floorplan[j][z] = false;
                }
            }
        }
        Graph floor(w*h);
        for(int j = 0; j < h; ++j){
            for(int z = 0; z < w; ++z){
                if(floorplan[j][z]){
                    if(z < w - 1 && floorplan[j][z + 1]){
                        add_edge(z + j*w, z + 1 + j*w, floor);
                    }
                    if(j < h - 1 && floorplan[j + 1][z]){
                        add_edge(z + j*w, z + (j + 1)*w, floor);
                    }
                } 
            }
        }
        vector<graph_traits<Graph>::vertex_descriptor> mate(w*h);
        edmonds_maximum_cardinality_matching(floor, &mate[0]);
        bool stop = false;
        for(int j = 0; j < h && !stop; ++j){
            for(int z = 0; z < w && !stop; ++z){
                if(floorplan[j][z]){
                    if(mate[z + j*w] == graph_traits<Graph>::null_vertex()){
                        cout << "no\n";
                        stop = true;
                    }
                }
            }
        }
        if(!stop){
            cout << "yes\n";
        }
    }
}
