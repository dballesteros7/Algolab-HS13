#include <iostream>
#include <set>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    int M, N, K;
    int P;
    int i, j, k;
    int tmp;
    int *left, *right, *suspicion;
    vector< set<int> > left_cases;
    vector< set<int> > right_cases;
    set<int> left_set, right_set;
    char comparison;
    cin >> M;
    for (i = 0; i < M; i++) {
        left_cases = vector< set<int> >();
        right_cases = vector< set<int> >();
        cin >> N;
        cin >> K;
        suspicion = new int[N+1];
        for (j = 0; j <= N; j++) {
            suspicion[j] = 0;
        }
        for (j = 0; j < K; j++) {
            cin >> P;
            left = new int[P];
            right = new int[P];
            for (k = 0; k < P * 2; k++) {
                cin >> tmp;
                if (k < P) {
                    left[k] = tmp;
                } else {
                    right[k - P] = tmp;
                }
            }
            cin >> comparison;
            if (comparison == '=') {
                for (k = 0; k < P; k++) {
                    suspicion[left[k]] = 3;
                    suspicion[right[k]] = 3;
                }
            } else if (comparison == '<') {
                left_set = set<int>();
                right_set = set<int>();
                for (k = 0; k < P; k++) {
                    if (suspicion[left[k]] == 0 || suspicion[left[k]] == 2) {
                        suspicion[left[k]] = 2;
                        left_set.insert(left[k]);
                    } else {
                        suspicion[left[k]] = 3;
                    }
                    if (suspicion[right[k]] == 0 || suspicion[right[k]] == 1) {
                        suspicion[right[k]] = 1;
                        right_set.insert(right[k]);
                    } else {
                        suspicion[right[k]] = 3;
                    }
                }
                left_cases.push_back(left_set);
                right_cases.push_back(right_set);

            } else {
                left_set = set<int>();
                right_set = set<int>();
                for (k = 0; k < P; k++) {
                    if (suspicion[left[k]] == 0 || suspicion[left[k]] == 1) {
                        suspicion[left[k]] = 1;
                        right_set.insert(left[k]);
                    } else {
                        suspicion[left[k]] = 3;
                    }
                    if (suspicion[right[k]] == 0 || suspicion[right[k]] == 2) {
                        suspicion[right[k]] = 2;
                        left_set.insert(right[k]);
                    } else {
                        suspicion[right[k]] = 3;
                    }
                }
                left_cases.push_back(left_set);
                right_cases.push_back(right_set);
            }
            delete [] left;
            delete [] right;
        }
        for (k = 0; k < left_cases.size(); k++) {
            set<int>::iterator it;
            
            it = left_cases[k].begin();
            while(it != left_cases[k].end()){
                if(suspicion[*it] == 3){
                    left_cases[k].erase(it++);
                } else {
                    it++;
                }
            }
            it = right_cases[k].begin();
            while(it != right_cases[k].end()){
                if(suspicion[*it] == 3){
                    right_cases[k].erase(it++);
                } else {
                    it++;
                }
            }
            if(left_cases[k].size() == 0 && right_cases[k].size() == 1){
                cout << *(right_cases[k].begin());
                cout << "\n";
                break;
            } else if(left_cases[k].size() == 1 && right_cases[k].size() == 0){
                cout << *(left_cases[k].begin());
                cout << "\n";
                break;
            }
        }
        if (k == left_cases.size()){
            j = 0;
            int red_herring;
            for(k = 1; k <= N; k++){
                if(suspicion[k] != 3){
                    j++;
                    red_herring = k;
                    if(j > 1){
                        break;
                    }
                }
            }
            if(j == 1){
                cout << red_herring << "\n";
            } else {
                cout << 0 << "\n";
            }
        }
        delete [] suspicion;
    }


    return 0;
}
