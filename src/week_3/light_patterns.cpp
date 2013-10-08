#include<iostream>
#include<numeric>

using namespace std;

int main(){

    int t;
    int i, j;
    int n, k, x;
    int *pattern;
    int *state;
    int *values_individual;
    int *values_block;
    int tmp;
    cin >> t;
    for(i = 0; i < t; i++){
        cin >> n;
        cin >> k;
        cin >> x;
        j = k - 1;
        pattern = new int[k];
        state = new int[n];
        values_individual = new int[n/k];
        values_block = new int[n/k];
        while(j >= 0){
            if(x % 2 == 0){
                pattern[j] = 0;
            } else {
                pattern[j] = 1;
            }
            x /= 2;
            j--;
        }
        for(j = 0; j < n; j++){
            cin >> tmp;
            state[j] = (pattern[j % k] + tmp) % 2;   
        }
        values_individual[0] = accumulate(state, state + k, 0);
        values_block[0] = k - values_individual[0] + 1;
        for(j = k; j < n; j+=k){
             tmp = accumulate(state + j, state + j + k, 0);
             values_individual[j/k] = min(values_individual[(j/k) - 1], values_block[(j/k) -1]) + tmp;
             values_block[j/k] = min(values_block[(j/k) - 1] + k - tmp, values_individual[(j/k) - 1] + k - tmp + 2);
        }
        cout << min(values_individual[(n/k) - 1], values_block[(n/k) - 1]) << "\n";
        delete [] pattern;
        delete [] state;
        delete [] values_individual;
        delete [] values_block;
    }    

}
