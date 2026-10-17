#include<iostream>
#include<vector>

using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    int n, k, t;
    int i, j;
    vector<long> costs;
    // Sliding window minimum: indices with increasing costs, the front
    // is the cheapest position among the last k
    vector<int> window;
    int head, tail;
    int tmp;
    cin >> t;

    for(i = 0; i < t; i++){
        cin >> n;
        cin >> k;
        costs.resize(n);
        window.resize(n);
        head = 0;
        tail = 0;
        for(j = 0; j < n; j++){
            cin >> tmp;
            if(j > k){
                while(window[head] < j - k){
                    head++;
                }
                costs[j] = tmp + costs[window[head]];
            } else {
                costs[j] = tmp;
            }
            while(tail > head && costs[window[tail - 1]] >= costs[j]){
                tail--;
            }
            window[tail++] = j;
        }
        cout << costs[n-1] << "\n";
    }

}
