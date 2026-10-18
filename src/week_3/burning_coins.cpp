#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;



int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    int t, N, i, j, m, length;
    vector<int> coins;
    // solutions[m] -> best guaranteed value on coins[m..m+length-1] when it's
    // our turn. Only lengths with the parity of N are needed (we move, then
    // the opponent moves), and each length only depends on length - 2, so
    // the table is updated in place.
    vector<int> solutions;
    cin >> t;
    for(i = 0; i < t; i++){
        cin >> N;
        coins.resize(N);
        for(j = 0; j < N; j++)
            cin >> coins[j];
        if(N == 0){
            cout << 0 << "\n";
            continue;
        }
        solutions.resize(N);
        if(N % 2 == 1){
            length = 1;
            for(m = 0; m < N; m++){
                solutions[m] = coins[m];
            }
        } else {
            length = 2;
            for(m = 0; m < N - 1; m++){
                solutions[m] = max(coins[m], coins[m+1]);
            }
        }
        for(length += 2; length <= N; length += 2){
            for(m = 0; m <= N - length; m++){
                solutions[m] = max(coins[m] + min(solutions[m+1], solutions[m+2]),
                                   coins[m + length - 1] + min(solutions[m], solutions[m+1]));
            }
        }
        cout << solutions[0] << "\n";
    }
    return 0;
}
