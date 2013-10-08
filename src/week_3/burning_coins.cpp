#include <iostream>
#include <vector>

using namespace std;



int main(){
    
    int t, N, i, j, k, m;
    int *coins;
    vector< vector<int> > solutions;
    cin >> t;
    for(i = 0; i < t; i++){
        cin >> N;
        coins = new int[N];
        solutions = vector< vector<int> >();
        for(j = 0; j < N; j++)
            cin >> coins[j];
        if(N == 0){
            cout << 0 << "\n";
        } else if(N == 1){
            cout << coins[0] << "\n";
        } 
        else if(N % 2 == 0){
            for(k = 0; k < N/2; k++){
                solutions.push_back(vector<int>());
                for(m = 0; m < N - (1 + 2*k); m++){
                    if(k == 0){
                        solutions[k].push_back(max(coins[m], coins[m+1]));
                    } else {
                        solutions[k].push_back(max(coins[m] + min(solutions[k-1][m+1],
                                                                  solutions[k-1][m+2]),
                                                   coins[m + (1+2*k)] + min(solutions[k-1][m],
                                                                            solutions[k-1][m+1])));
                    }
                }
            }
            cout << solutions[N/2 - 1][0] << "\n";
        } else {
            for(k = 0; k < N/2; k++){
                solutions.push_back(vector<int>());
                for(m = 0; m < N - 2*(k+1); m++){
                    if(k == 0){
                        solutions[k].push_back(max(coins[m], coins[m+2]) +
                                               min(coins[m], min(coins[m+1], coins[m+2])));
                    } else {
                        solutions[k].push_back(max(coins[m] + min(solutions[k-1][m+1],
                                                                  solutions[k-1][m+2]),
                                                   coins[m + 2*(k+1)] + min(solutions[k-1][m],
                                                                        solutions[k-1][m+1])));
                    }
                }
            }
            cout << solutions[N/2 - 1][0] << "\n";
        }
    }
    return 0;
}
