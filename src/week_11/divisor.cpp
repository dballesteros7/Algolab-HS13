#include <iostream>
#include <cmath>
#include <vector>
#include <map>
using namespace std;


int gcd(int a, int b){
    if(b == 0){
        return a;
    } else {
        return gcd(b, a % b);
    }
}

int lpd(int a){
    int ceiling = ceil(a);
    for(int i = 2; i < ceiling; ++i){
        if(a % i == 0){
            return a/i;
        } else {
            ceiling = ceil(a/i);
        }
    }
    return 1;
}

int calculate_distance(int v, int w, map<int,int> &lpds){
    if(v == w){
        return 0;
    }
    vector<int> sequence_v;
    int m = v;
    while(m > 1){
        sequence_v.push_back(m);
        map<int,int>::iterator lpds_iter = lpds.find(m);
        if(lpds_iter != lpds.end()){
            m = lpds_iter->second;
        } else {
            int lpd_m = lpd(m);
            lpds.insert(pair<int,int>(m,lpd_m));
            m = lpd_m;
        }
    }
    vector<int> sequence_w;
    int n = w;
    while(n > 1){
        sequence_w.push_back(n);
        map<int,int>::iterator lpds_iter = lpds.find(n);
        if(lpds_iter != lpds.end()){
            n = lpds_iter->second;
        } else {
            int lpd_n = lpd(n);
            lpds.insert(pair<int,int>(n,lpd_n));
            n = lpd_n;
        }
    }
    vector<int>::iterator it_v = sequence_v.begin();
    vector<int>::iterator it_w = sequence_w.begin();
    while(it_v != sequence_v.end() && it_w != sequence_w.end()){
        if(*it_v == *it_w){
            return distance(sequence_v.begin(), it_v) + distance(sequence_w.begin(), it_w);
        } else if(*it_v > *it_w){
            it_v++;
        } else {
            it_w++;
        }
    }
    return sequence_w.size() + sequence_v.size();
}

int main(){
    ios_base::sync_with_stdio(false);

    int T;
    cin >> T;
    map<int, int> lpds;
    for(int i = 0; i < T; ++i){
        int n, c;
        cin >> n >> c;
        for(int j = 0; j < c; ++j){
            int v, w;
            cin >> v >> w;
            int distance;
            if(v > w){
                distance = calculate_distance(v,w, lpds);
            } else {
                distance = calculate_distance(w,v, lpds);
            }
            cout << distance << "\n";
        }
    }
}
