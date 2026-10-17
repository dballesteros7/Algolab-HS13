#include <iostream>
#include <vector>

using namespace std;

// Counts inversions of a permutation of 1..N (mod 10000) with a Fenwick
// tree: for every element, the number of previous elements greater than it.
int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    vector<int> tree;
    int t;
    int N;
    int i, j, k;
    int tmp;
    long long count;
    cin >> t;
    for(i = 0; i < t; i++){
        cin >> N;
        tree.assign(N + 1, 0);
        count = 0;
        for(j = 0; j < N; j++){
            cin >> tmp;
            // j previous elements, minus those smaller than tmp
            count += j;
            for(k = tmp; k > 0; k -= k & -k){
                count -= tree[k];
            }
            for(k = tmp; k <= N; k += k & -k){
                tree[k]++;
            }
        }
        cout << count % 10000 << "\n";
    }
    return 0;
}
