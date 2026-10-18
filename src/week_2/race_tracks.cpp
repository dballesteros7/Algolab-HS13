#include <iostream>
#include <vector>

using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    int N;
    int X, Y;
    int x1, x2, y1, y2;
    int P;
    int i, j, k, m;
    int tmp_1, tmp_2, tmp_3, tmp_4;
    vector<bool> obstacles;
    // State (x, y, x_speed, y_speed) -> ((x*Y + y)*7 + x_speed + 3)*7 + y_speed + 3
    vector<int> hops;
    vector<int> to_visit;

    cin >> N;
    for(i = 0; i < N; i++){
        cin >> X;
        cin >> Y;
        cin >> x1;
        cin >> y1;
        cin >> x2;
        cin >> y2;
        cin >> P;
        obstacles.assign(X*Y, true);
        for(j = 0; j < P; j++){
            cin >> tmp_1;
            cin >> tmp_2;
            cin >> tmp_3;
            cin >> tmp_4;
            for(k = tmp_1; k <= tmp_3; k++){
                for(m = tmp_2; m <= tmp_4; m++){
                    obstacles[k*Y + m] = false;
                }
            }
        }

        // Sanity test
        if(!obstacles[x2*Y + y2]){
            cout << "No solution.\n";
            continue;
        }

        // Every hop costs the same, so a BFS over the states is optimal
        int result = -1;
        if(x1 == x2 && y1 == y2){
            result = 0;
        } else {
            hops.assign(X*Y*7*7, -1);
            to_visit.resize(X*Y*7*7);
            int head = 0, tail = 0;
            int start = ((x1*Y + y1)*7 + 3)*7 + 3;
            hops[start] = 0;
            to_visit[tail++] = start;
            while(head < tail && result == -1){
                int state = to_visit[head++];
                int y_speed = state % 7 - 3;
                int x_speed = (state / 7) % 7 - 3;
                int position = state / 49;
                int x_state = position / Y;
                int y_state = position % Y;
                int g_cost = hops[state];
                for(j = -1; j <= 1 && result == -1; j++){
                    int new_x_speed = x_speed + j;
                    int new_x = x_state + new_x_speed;
                    if(new_x_speed > 3 || new_x_speed < -3 || new_x >= X || new_x < 0){
                        continue;
                    }
                    for(k = -1; k <= 1; k++){
                        int new_y_speed = y_speed + k;
                        int new_y = y_state + new_y_speed;
                        if(new_y_speed > 3 || new_y_speed < -3 || new_y >= Y || new_y < 0){
                            continue;
                        }
                        if(new_x_speed == 0 && new_y_speed == 0){
                            continue;
                        }
                        if(!obstacles[new_x*Y + new_y]){
                            continue;
                        }
                        if(new_x == x2 && new_y == y2){
                            result = g_cost + 1;
                            break;
                        }
                        int next = ((new_x*Y + new_y)*7 + (new_x_speed + 3))*7 + (new_y_speed + 3);
                        if(hops[next] == -1){
                            hops[next] = g_cost + 1;
                            to_visit[tail++] = next;
                        }
                    }
                }
            }
        }
        if(result >= 0){
            cout << "Optimal solution takes " << result << " hops.\n";
        } else {
            cout << "No solution.\n";
        }

    }

    return 0;
}
