#include <iostream>
#include <vector>

using namespace std;

// Residual network stored as arc lists; arc e and e^1 are each other's reverse.
struct Network {
    vector<int> first, next, to, capacity;
    Network(int n) : first(n, -1) {}
    void add_arcs(int a, int b, int forward, int backward){
        to.push_back(b); capacity.push_back(forward); next.push_back(first[a]); first[a] = to.size() - 1;
        to.push_back(a); capacity.push_back(backward); next.push_back(first[b]); first[b] = to.size() - 1;
    }
};

int main(){
    ios_base::sync_with_stdio(false);
    int T;
    cin >> T;
    for(int i = 0; i < T; ++i){
        int n, m, s;
        cin >> n >> m >> s;
        // Vertex n is the sink; every store sends one unit there.
        Network cityNetwork(n + 1);
        for(int j = 0; j < s; j++){
            int store;
            cin >> store;
            cityNetwork.add_arcs(store, n, 1, 0);
        }
        // A street can be used once, in either direction.
        for(int j = 0; j < m; ++j){
            int a,b;
            cin >> a >> b;
            cityNetwork.add_arcs(a, b, 1, 1);
        }
        // The flow is at most s, so augment along BFS paths from home (0) one
        // unit at a time, stopping each search as soon as it reaches the sink.
        int flow = 0;
        vector<int> arrivedBy(n + 1), queue(n + 1);
        while(flow < s){
            fill(arrivedBy.begin(), arrivedBy.end(), -1);
            int head = 0, tail = 0;
            queue[tail++] = 0;
            arrivedBy[0] = -2;
            while(head < tail && arrivedBy[n] == -1){
                int u = queue[head++];
                for(int e = cityNetwork.first[u]; e != -1; e = cityNetwork.next[e]){
                    int v = cityNetwork.to[e];
                    if(cityNetwork.capacity[e] > 0 && arrivedBy[v] == -1){
                        arrivedBy[v] = e;
                        queue[tail++] = v;
                    }
                }
            }
            if(arrivedBy[n] == -1){
                break;
            }
            for(int v = n; v != 0; v = cityNetwork.to[arrivedBy[v] ^ 1]){
                --cityNetwork.capacity[arrivedBy[v]];
                ++cityNetwork.capacity[arrivedBy[v] ^ 1];
            }
            ++flow;
        }
        if(flow == s){
            cout << "yes\n";
        } else {
            cout << "no\n";
        }
    }
}
