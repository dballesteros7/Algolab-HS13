#include <iostream>
#include <vector>
#include <utility>

using namespace std;

// Beads are edges between their two colours; the bracelet is an Euler circuit
// of this multigraph. It exists iff every colour has even degree and all
// beads are connected, which Hierholzer's algorithm reveals by using up every
// bead in a single circuit.

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    int t;
    cin >> t;
    for(int i = 0; i < t; ++i){
        cout << "Case #" << (i + 1) << "\n";
        int n;
        cin >> n;
        vector< vector< pair<int,int> > > beads_of_color(50);
        int start = 0;
        for(int j = 0; j < n; ++j){
            int a,b;
            cin >> a >> b;
            --a;
            --b;
            beads_of_color[a].push_back(pair<int,int>(b, j));
            beads_of_color[b].push_back(pair<int,int>(a, j));
            start = b;
        }
        bool even = true;
        for(int j = 0; j < 50; ++j){
            if(beads_of_color[j].size() % 2 == 1){
                even = false;
                break;
            }
        }
        vector<int> sequence;
        if(even){
            // Iterative Hierholzer: the colours are popped off the stack in
            // the (reversed) order of the circuit
            vector<bool> used(n, false);
            vector<size_t> next_bead(50, 0);
            vector<int> to_visit;
            to_visit.push_back(start);
            while(!to_visit.empty()){
                int color = to_visit.back();
                vector< pair<int,int> > &beads = beads_of_color[color];
                while(next_bead[color] < beads.size() && used[beads[next_bead[color]].second]){
                    next_bead[color]++;
                }
                if(next_bead[color] < beads.size()){
                    used[beads[next_bead[color]].second] = true;
                    to_visit.push_back(beads[next_bead[color]].first);
                } else {
                    sequence.push_back(color);
                    to_visit.pop_back();
                }
            }
        }
        if((int)sequence.size() != n + 1){
            cout << "some beads may be lost\n";
        } else {
            for(int j = n; j > 0; --j){
                cout << sequence[j] + 1 << " " << sequence[j - 1] + 1 << "\n";
            }
        }
        if(i != t - 1){
            cout << "\n";
        }
    }
}
