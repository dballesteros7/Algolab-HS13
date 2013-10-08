#include<iostream>
#include<queue>
#include<utility>
#include<vector>
#include<algorithm>

using namespace std;

struct node{
    
    bool operator()(const pair<int,long>& x, const pair<int,long>& y) const {
        return x.second > y.second;
    }
};
int main(){
    ios_base::sync_with_stdio(false);
    int n, k, t;
    int i, j, m;
    vector<long> costs;
    priority_queue<pair<int, long>, vector< pair<int,long> >, node> what_i_need;
    int tmp;
    pair<int,long> x;
    cin >> t;
    
    for(i = 0; i < t; i++){
        cin >> n;
        cin >> k;
        costs = vector<long>();
        costs.reserve(n);
        what_i_need = priority_queue<pair<int, long>, vector< pair<int,long> >, node>();
        for(j = 0; j < n; j++){
            cin >> tmp;
            if(j > k){
                while(true){
                    pair<int,long> w = what_i_need.top();
                    if(w.first >= j - k){
                        costs.push_back(tmp + w.second);
                        x = pair<int,long>();
                        x.first = j;
                        x.second = tmp + w.second;
                        what_i_need.push(x);
                        break;
                    } else {
                        what_i_need.pop();
                    }
                }
            } else {
                costs.push_back(tmp);
                x = pair<int,long>();
                x.first = j;
                x.second = tmp;
                what_i_need.push(x);
            }
        }
        cout << costs[n-1] << "\n";
    }
    
}