#include <iostream>
#include <algorithm>
#include <utility>
#include <vector>

using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    // From problem definition
    // t -> test cases
    // n, m -> aliens and humans
    int t;
    int n, m;

    // Temporal and indexes
    int i, j;
    int tmp1, tmp2;

    // Wounded intervals stored as (p, -q): sorted by p ascending and, for
    // equal p, by q descending, so any interval containing another one comes
    // first (or is an identical neighbour).
    vector< pair<int,int> > aliens;

    cin >> t;
    for(i = 0; i < t; i++){
        cin >> n;
        cin >> m;
        aliens.clear();
        for(j = 0; j < n; j++){
            cin >> tmp1;
            cin >> tmp2;
            if(tmp1 != 0){
                aliens.push_back(make_pair(tmp1, -tmp2));
            }
        }
        sort(aliens.begin(), aliens.end());

        // covered -> humans 1..covered are all wounded
        // max_end -> furthest q among the intervals seen so far
        int covered = 0;
        int max_end = 0;
        int count = 0;
        bool panic = false;
        int size = aliens.size();
        for(j = 0; j < size; j++){
            int p = aliens[j].first;
            int q = -aliens[j].second;
            if(p > covered + 1){
                panic = true;
                break;
            }
            covered = max(covered, q);
            // Not contained in a previous interval and not duplicated
            if(q > max_end && (j + 1 == size || aliens[j + 1] != aliens[j])){
                count++;
            }
            max_end = max(max_end, q);
        }
        if(panic || covered != m){
            cout << 0 << "\n";
        } else {
            cout << count << "\n";
        }
    }
    return 0;
}
