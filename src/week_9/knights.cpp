#include <iostream>
#include <queue>
#include <vector>

using namespace std;

// Hopcroft-Karp on the bipartite "knight move" graph: a knight always jumps
// between the two colours of the board.
vector< vector<int> > moves;
vector<int> mateOf, mateIn, level;

bool find_levels(){
    queue<int> frontier;
    bool augmentable = false;
    for(size_t u = 0; u < moves.size(); ++u){
        level[u] = (mateOf[u] == -1) ? 0 : -1;
        if(level[u] == 0){
            frontier.push(u);
        }
    }
    while(!frontier.empty()){
        int u = frontier.front();
        frontier.pop();
        for(size_t k = 0; k < moves[u].size(); ++k){
            int w = mateIn[moves[u][k]];
            if(w == -1){
                augmentable = true;
            } else if(level[w] == -1){
                level[w] = level[u] + 1;
                frontier.push(w);
            }
        }
    }
    return augmentable;
}

bool augment(int u){
    for(size_t k = 0; k < moves[u].size(); ++k){
        int v = moves[u][k];
        int w = mateIn[v];
        if(w == -1 || (level[w] == level[u] + 1 && augment(w))){
            mateOf[u] = v;
            mateIn[v] = u;
            return true;
        }
    }
    level[u] = -1;
    return false;
}

int main(){
    ios_base::sync_with_stdio(false);
    const int dx[8] = {-2, -2, 2, 2, -1, -1, 1, 1};
    const int dy[8] = {-1, 1, -1, 1, -2, 2, -2, 2};
    int t;
    cin >> t;
    for(int i = 0; i < t; ++i){
        int n;
        cin >> n;
        int realCount = 0;
        vector<bool> existence(n*n);
        for(int j = 0; j < n*n; ++j){
            int exists;
            cin >> exists;
            existence[j] = (exists == 1);
            realCount += existence[j];
        }
        // Edges go from the squares with j + k even to the ones they attack.
        moves.assign(n*n, vector<int>());
        for(int j = 0; j < n; ++j){
            for(int k = j % 2; k < n; k += 2){
                if(!existence[j*n + k]){
                    continue;
                }
                for(int m = 0; m < 8; ++m){
                    int x = j + dx[m], y = k + dy[m];
                    if(x >= 0 && x < n && y >= 0 && y < n && existence[x*n + y]){
                        moves[j*n + k].push_back(x*n + y);
                    }
                }
            }
        }
        mateOf.assign(n*n, -1);
        mateIn.assign(n*n, -1);
        level.assign(n*n, -1);
        int matchingSize = 0;
        while(find_levels()){
            for(int u = 0; u < n*n; ++u){
                if(mateOf[u] == -1 && !moves[u].empty() && augment(u)){
                    ++matchingSize;
                }
            }
        }
        // Konig: maximum independent set = squares - maximum matching.
        cout << realCount - matchingSize << "\n";
    }
}
