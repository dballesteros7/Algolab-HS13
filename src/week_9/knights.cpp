#include <iostream>
#include <queue>
#include <vector>
#include <utility>
#include <set>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

using namespace std;
using namespace boost;

typedef pair<int,int> X;
typedef adjacency_list<vecS,vecS,undirectedS> Graph;

int main(){
    ios_base::sync_with_stdio(false);
    const X possibilities[8] = {X(-2,-1), X(-2,1), X(2,-1), X(2,1), X(-1,-2), X(-1,2), X(1,-2), X(1,2)};
    int t;
    cin >> t;
    for(int i = 0; i < t; ++i){
        int n;
        cin >> n;
        int realCount = 0;
        Graph board(n*n);
        bool **existence = new bool*[n];
        for(int j = 0; j < n; ++j){
            existence[j] = new bool[n];
            for(int k = 0; k < n; ++k){
                int exists;
                cin >> exists;
                existence[j][k] = false;
                if(exists == 1){
                    existence[j][k] = true;
                    ++realCount;
                }
            }
        }
        for(int j = 0; j < n; ++j){
            for(int k = 0; k < n; ++k){
                if(existence[j][k]){
                    for(int m = 0; m < 8; ++m){
                        X newPos(j + possibilities[m].first, k + possibilities[m].second);
                        if(!(newPos.first < n && newPos.first >= 0 && newPos.second < n && newPos.second >= 0)){
                            continue;
                        }
                        if(!existence[newPos.first][newPos.second]){
                            continue;
                        }
                        bool already = false;
                        tie(tuples::ignore, already) = edge(j + k*n, newPos.first + n*newPos.second, board);
                        if(already){
                            continue;
                        }
                        add_edge(j + k*n, newPos.first + n*newPos.second, board);
                    }
                }
            }
        }
        vector<int> mate(n*n);
        edmonds_maximum_cardinality_matching(board, &mate[0]);
        int matchingSize = matching_size(board, &mate[0]);
        cout << realCount - matchingSize << "\n";
    }
}
