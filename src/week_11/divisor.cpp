#include <iostream>
#include <cmath>

using namespace std;


int gcd(int a, int b){
    if(b == 0){
        return a;
    } else {
        return gcd(b, a % b);
    }
}

int lpd(int a){
    if(a % 2 == 0){
        return a/2;
    }
    for(int i = 2; i < (a + 1)/2; ++i){
        if(a % i == 0){
            return a / i;
        }
    }
    return 1;
}

int calculate_distance(int v, int w){
    if(v == w){
        return 0;
    } 
    int gcd_v_w = gcd(v,w);
    if(gcd_v_w == w){
        return 1 + calculate_distance(lpd(v), w);
    } else {
        return calculate_distance(v, gcd_v_w) + calculate_distance(w, gcd_v_w);
    }
}

int main(){
    ios_base::sync_with_stdio(false);

    int T;
    cin >> T;
    for(int i = 0; i < T; ++i){
        int n, c;
        cin >> n >> c;
        for(int j = 0; j < c; ++j){
            int v, w;
            cin >> v >> w;
            int distance;
            if(v > w){
                distance = calculate_distance(v,w);
            } else {
                distance = calculate_distance(w,v);
            }
            cout << distance << "\n";
        }
    }
}
