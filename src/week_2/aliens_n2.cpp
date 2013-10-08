#include <iostream>
#include <algorithm>
#include <list>
#include <vector>

using namespace std;

bool contains_set(vector<int> a, vector<int> b){
    if((a[0] >= b[0] && a[1] <= b[1])){
        return true;
    } else {
        return false;
    }
}

bool compare_strict(vector<int> &a, vector<int> &b){
    if(a[0] < b[0]){
        return true;
    } else if(a[1] < b[1]){
        return true;
    } else {
        return false;
    }
}

int contains(vector<int> a, vector<int> b){
    if(contains_set(a, b) && !contains_set(b, a)){
        return 1; 
    } else if(!contains_set(a, b) && contains_set(b, a)){
        return 2;
    } else if(contains_set(a, b) && contains_set(b, a)){
        return 0;
    } else {
        return -1;
    }
}

int main(){
    ios_base::sync_with_stdio(false);

    int t;
    int i, j, k;
    int n, m;
    int tmp1, tmp2, tmp3;
    vector<int> tmp;
    vector< vector<int> > intervals;
    int min_covered;
    int max_covered;
    int count;
    bool add, first_added;
    cin >> t;
    for(i = 0; i < t; i++){
        cin >> n;
        cin >> m;
        max_covered = 0;
        min_covered = m+1;
        count = 0;
        intervals = vector< vector <int> >();
        first_added = false;
        int penalty = 0;
        for(j = 0; j < n; j++){
            cin >> tmp1;
            cin >> tmp2;
            if(tmp1 != 0){
                tmp = vector<int>();
                tmp.push_back(tmp1);
                tmp.push_back(tmp2);
                add = true;
                vector< vector<int> >::iterator it = intervals.begin();
                while(it != intervals.end()){
                    tmp3 = contains((*it), tmp);
                    if(tmp3 == 0){
                        penalty++;
                        add = false;
                        ++it;
                    } else if(tmp3 == 1){
                        it = intervals.erase(it);
                    } else if(tmp3 == 2){
                        add = false;
                        ++it;
                    } else {
                        ++it;
                    }
                }
                if(!first_added || add){
                    intervals.push_back(tmp);
                    first_added = true;
                }
                if(tmp1 < min_covered){
                    min_covered = tmp1;
                }
                if(tmp2 > max_covered){
                    max_covered = tmp2;
                }
            }
        }
        if(min_covered != 1 || max_covered != m){
            cout << 0 << "\n";
            continue;
        } else {
            cout << intervals.size() - penalty << "\n";
        }

    }
    return 0;
}