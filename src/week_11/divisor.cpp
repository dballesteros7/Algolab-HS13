#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// The parent of v in the divisor tree is its largest proper divisor, i.e.
// v / (smallest prime factor of v); primes hang directly below 1.
// smallest_factor[v] holds that factor for composite v (it is <= sqrt(v), so
// it fits in 16 bits) and 0 for primes.
inline int parent(int v, const vector<unsigned short> &smallest_factor){
    return smallest_factor[v] ? v/smallest_factor[v] : 1;
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    // Read every query first so that one sieve up to the largest queried
    // number serves all test cases.
    int T;
    cin >> T;
    vector<pair<int,int> > queries;
    int max_value = 1;
    for(int i = 0; i < T; ++i){
        int n, c;
        cin >> n >> c;
        for(int j = 0; j < c; ++j){
            int v, w;
            cin >> v >> w;
            queries.push_back(pair<int,int>(v, w));
            max_value = max(max_value, max(v, w));
        }
    }

    vector<unsigned short> smallest_factor(max_value + 1, 0);
    for(int p = 2; (long long)p*p <= max_value; ++p){
        if(smallest_factor[p] == 0){
            for(int j = p*p; j <= max_value; j += p){
                if(smallest_factor[j] == 0){
                    smallest_factor[j] = p;
                }
            }
        }
    }

    // The larger of the two numbers can never be their common ancestor, so
    // always move it one step towards the root until both meet.
    for(size_t q = 0; q < queries.size(); ++q){
        int v = queries[q].first;
        int w = queries[q].second;
        int distance = 0;
        while(v != w){
            if(v > w){
                v = parent(v, smallest_factor);
            } else {
                w = parent(w, smallest_factor);
            }
            ++distance;
        }
        cout << distance << "\n";
    }
}
