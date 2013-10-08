#include <iostream>
#include <algorithm>
#include <set>
#include <vector>

using namespace std;

struct compare_strict {
    bool operator() (const vector<int>& a, const vector<int>& b) const{
        if(a[0] < b[0]){
            return true;
        } else if(a[0] == b[0]){
            if(a[1] < b[1]){
                return true;
            } else if(a[1] == b[1]){
                if(a[2] < b[2]){
                    return true;
                } else {
                    return false;
                }
            } else{
                return false;
            }
        } else {
            return false;
        }
    }
};


int main(){
    ios_base::sync_with_stdio(false);
    
    // From problem definition
    // t -> test cases
    // n, m -> aliens and humans
    int t;
    int n, m;
    
    // Temporal and indexes
    int i, j, k;
    int tmp1, tmp2, tmp3;
    vector<int> tmp;
 
    // Sanity check on human defeat
    int min_covered, max_covered;
    
    // Candidate aliens
    set< vector<int>, compare_strict > aliens;
    
    cin >> t;
    for(i = 0; i < t; i++){
        aliens = set< vector<int>, compare_strict>();
        cin >> n;
        cin >> m;
        max_covered = 0;
        min_covered = m+1;
        for(j = 0; j < n; j++){
            cin >> tmp1;
            cin >> tmp2;
            if(tmp1 != 0){
                tmp = vector<int>();
                tmp.push_back(tmp1);
                tmp.push_back(tmp2);
                tmp.push_back(j);
                aliens.insert(tmp);
                if(tmp1 < min_covered){
                    min_covered = tmp1;
                }
                if(tmp2 > max_covered){
                    max_covered = tmp2;
                }
            }
            
        }

        if(max_covered != m || min_covered != 1 || aliens.size() == 0){
            cout << 0 << "\n";
            continue;
        }
        if(aliens.size() == 1){
            cout << 1 << "\n";
            continue;
        }
        int upper_limit = 1;
        bool panic = false;
        set< vector<int>, compare_strict>::iterator dominant, previous_to_dominant;
        int count = 0;
        dominant = aliens.begin();
        upper_limit = (*dominant)[1];
        set< vector<int>, compare_strict>::iterator it = aliens.begin();
        ++it;
        for(; it != aliens.end(); it++){
            if((*it)[0] > (*dominant)[0] && (*it)[1] < (*dominant)[1]){
                continue;
            } else if((*it)[0] == (*dominant)[0] && (*it)[1] >= (*dominant)[1]){
                dominant = it;
                --it;
                previous_to_dominant = it;
                ++it;
                upper_limit = (*dominant)[1];
            } else if((*it)[0] > (*dominant)[0] && (*it)[1] > (*dominant)[1]){  
                if(dominant == aliens.begin() || 
                        (*(previous_to_dominant))[0] != (*dominant)[0] || (*(previous_to_dominant))[1] != (*dominant)[1]){
                    count++;
                }
                if((*it)[0] > (*dominant)[1] && (*it)[0] != upper_limit+1){
                    panic = true;
                    break;
                }
                dominant = it;
                --it;
                previous_to_dominant = it;
                ++it;
                upper_limit = (*dominant)[1];
            }
            
        }
        if(panic){
            cout << 0 << "\n";
            continue;
        }
        if(dominant == aliens.begin() || 
                (*dominant)[0] != (*(previous_to_dominant))[0] || (*dominant)[1] != (*(previous_to_dominant))[1]){
            count++;
        }
        cout << count << "\n";  
    }    
    return 0;
}

