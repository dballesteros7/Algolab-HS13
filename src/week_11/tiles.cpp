#include <iostream>
#include <vector>
#include <queue>

using namespace std;

// The floor is a grid graph, which is bipartite (colour cells like a chess
// board), so a tiling exists iff there is a perfect matching between the free
// black and the free white cells. It is found with Hopcroft-Karp directly on
// the grid: the grid is padded with a blocked border so the four neighbours
// of a cell are just fixed offsets.

const int INF = 1 << 30;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    int n;
    cin >> n;
    for(int i = 0; i < n; ++i){
        int w,h;
        cin >> w >> h;
        int W = w + 2;
        int cells = W*(h + 2);
        vector<char> floorplan(cells, false);
        vector<int> black_cells;
        int free_white = 0;
        for(int j = 0 ; j < h; ++j){
            for(int z = 0; z < w; ++z){
                char p;
                cin >> p;
                if(p == '.'){
                    int u = (j + 1)*W + z + 1;
                    floorplan[u] = true;
                    if((j + z) % 2 == 0){
                        black_cells.push_back(u);
                    } else {
                        free_white++;
                    }
                }
            }
        }
        if((int)black_cells.size() != free_white){
            cout << "no\n";
            continue;
        }
        const int offsets[4] = {1, -1, W, -W};
        vector<int> mate(cells, -1);
        int matched = 0;
        // Karp-Sipser initial matching: a cell with only one free neighbour
        // left has to be matched with it; when there is no such cell, match
        // the next unmatched cell arbitrarily. As long as only forced choices
        // were made, a cell without any free neighbour can never be covered.
        vector<int> degree(cells, 0);
        vector<int> forced;
        for(size_t b = 0; b < black_cells.size(); ++b){
            int u = black_cells[b];
            for(int d = 0; d < 4; ++d){
                int v = u + offsets[d];
                if(floorplan[v]){
                    degree[u]++;
                    degree[v]++;
                }
            }
        }
        for(int u = 0; u < cells; ++u){
            if(floorplan[u] && degree[u] <= 1){
                forced.push_back(u);
            }
        }
        bool impossible = false;
        bool guessed = false;
        for(int u = 0; u < cells && !impossible; ++u){
            int start = floorplan[u] ? u : -1;
            while(!impossible){
                int a;
                if(!forced.empty()){
                    a = forced.back();
                    forced.pop_back();
                } else if(start != -1){
                    a = start;
                    start = -1;
                } else {
                    break;
                }
                if(mate[a] != -1){
                    continue;
                }
                if(degree[a] == 0){
                    impossible = !guessed;
                    continue;
                }
                if(degree[a] > 1){
                    guessed = true;
                }
                int b = -1;
                for(int d = 0; d < 4 && b == -1; ++d){
                    int v = a + offsets[d];
                    if(floorplan[v] && mate[v] == -1){
                        b = v;
                    }
                }
                mate[a] = b;
                mate[b] = a;
                matched++;
                for(int d = 0; d < 4; ++d){
                    int x = a + offsets[d];
                    if(floorplan[x] && mate[x] == -1 && --degree[x] <= 1){
                        forced.push_back(x);
                    }
                    x = b + offsets[d];
                    if(floorplan[x] && mate[x] == -1 && --degree[x] <= 1){
                        forced.push_back(x);
                    }
                }
            }
        }
        if(impossible){
            cout << "no\n";
            continue;
        }
        vector<int> dist(cells);
        vector<int> next_neighbour(cells);
        vector<int> path;
        while(matched < free_white){
            // BFS from the unmatched black cells builds the layered graph
            queue<int> to_visit;
            for(size_t b = 0; b < black_cells.size(); ++b){
                int u = black_cells[b];
                if(mate[u] == -1){
                    dist[u] = 0;
                    to_visit.push(u);
                } else {
                    dist[u] = INF;
                }
            }
            // Stop after the first layer that reaches an unmatched white cell
            int found_layer = INF;
            while(!to_visit.empty()){
                int u = to_visit.front();
                to_visit.pop();
                if(dist[u] > found_layer){
                    break;
                }
                for(int d = 0; d < 4; ++d){
                    int v = u + offsets[d];
                    if(!floorplan[v]){
                        continue;
                    }
                    int next = mate[v];
                    if(next == -1){
                        found_layer = dist[u];
                    } else if(dist[next] == INF){
                        dist[next] = dist[u] + 1;
                        to_visit.push(next);
                    }
                }
            }
            if(found_layer == INF){
                break;
            }
            // Iterative DFS along the layers for vertex disjoint augmenting paths
            for(size_t b = 0; b < black_cells.size(); ++b){
                next_neighbour[black_cells[b]] = 0;
            }
            for(size_t b = 0; b < black_cells.size(); ++b){
                int root = black_cells[b];
                if(mate[root] != -1){
                    continue;
                }
                path.clear();
                path.push_back(root);
                while(!path.empty()){
                    int u = path.back();
                    if(next_neighbour[u] == 4){
                        dist[u] = INF;
                        path.pop_back();
                        continue;
                    }
                    int v = u + offsets[next_neighbour[u]++];
                    if(!floorplan[v]){
                        continue;
                    }
                    int next = mate[v];
                    if(next == -1){
                        // Flip the augmenting path
                        for(int k = path.size() - 1; k >= 0; --k){
                            int previous = mate[path[k]];
                            mate[path[k]] = v;
                            mate[v] = path[k];
                            v = previous;
                        }
                        matched++;
                        break;
                    }
                    if(dist[next] == dist[u] + 1){
                        path.push_back(next);
                    }
                }
            }
        }
        if(matched == free_white){
            cout << "yes\n";
        } else {
            cout << "no\n";
        }
    }
}
