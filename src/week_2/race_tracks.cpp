#include <iostream>
#include <queue>
#include <vector>
#include <set>
#include <algorithm>
#include <cmath>

using namespace std;

struct compare_strict{

    bool operator() (const vector<int> &a, const vector<int> &b) const{
        for(int i = 0; i < 4; i++){
            if(a[i] < b[i]){
                return true;
            } else if(a[i] > b[i]){
                return false;
            }
        }
        return false;
    }
};

struct compare_heuristic{
    bool operator() (const vector<int> &a, const vector<int> &b) const{
        return a[4] > b[4];
    }
};


int calculate_heuristic(const vector<int>& point, int goal_x, int goal_y){
    return max(abs(point[0] - goal_x)/3,abs(point[1] - goal_y)/3);
}

int main(){
    int N;
    int X, Y;
    int x1, x2, y1, y2;
    int P;
    int i, j, k, m, l;
    bool *obstacles;
    int tmp_1, tmp_2, tmp_3, tmp_4;
    int x_state, y_state, x_speed, y_speed;
    vector<int> tmp_container;
    vector<int> current_state;
    priority_queue< vector<int>, vector< vector<int> >, compare_heuristic > to_visit;
    int g_cost, f_estimate, h_estimate;
    int *g_costs;
    bool sane;
    
    cin >> N;
    for(i = 0; i < N; i++){
        cin >> X;
        cin >> Y;
        cin >> x1;
        cin >> y1;
        cin >> x2;
        cin >> y2;
        cin >> P;
        obstacles = new bool [X*Y];
        g_costs = new int[X*Y*7*7];
        if(obstacles == 0 || g_costs == 0){
            return 1;
        }
        to_visit = priority_queue< vector<int>, vector< vector<int> >, compare_heuristic >();
        for(j = 0; j < X; j++){
            for(k = 0; k < Y; k++){
                obstacles[j*Y + k] = true;
                for(l = 0; l < 7; l++){
                    for(m = 0; m < 7; m++){
                        g_costs[((j*Y+k)*7+l)*7 + m] = X*Y;
                    }
                }
            }
        }
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
        sane = obstacles[x2*Y + y2];
        if(!sane){
            cout << "No solution.\n";
            delete [] obstacles;
            delete [] g_costs;
            continue;
        }

        tmp_container = vector<int>();
        x_state = x1;
        y_state = y1;
        x_speed = 0;
        y_speed = 0; 

        tmp_container.push_back(x_state);
        tmp_container.push_back(y_state);
        tmp_container.push_back(x_speed);
        tmp_container.push_back(y_speed);
        h_estimate = calculate_heuristic(tmp_container, x2, y2);
        f_estimate = 0 + h_estimate;
        tmp_container.push_back(f_estimate);
        g_costs[((x_state*Y+y_state)*7+(x_speed + 3))*7 + (y_speed + 3)] = 0;
        to_visit.push(tmp_container);
        while( !to_visit.empty() && (x_state != x2 || y_state != y2)){
            current_state = to_visit.top();
            to_visit.pop();
            x_state = current_state[0];
            y_state = current_state[1];
            x_speed = current_state[2];
            y_speed = current_state[3];
            g_cost = g_costs[((x_state*Y + y_state)*7+(x_speed + 3))*7 + (y_speed + 3)];
            //cout << x_state << " " << y_state << " " << x_speed << " " << y_speed <<"\n";
            for(j = 1; j >= -1; j--){
                if((x_speed + j) > 3 || (x_speed + j) < -3){
                    continue;
                }
                if((x_state + x_speed + j) >= X || (x_state + x_speed + j) < 0){
                    continue;
                }
                for(k = 1; k >= -1; k--){
                    if((y_speed + k) > 3 || (y_speed + k) < -3){
                        continue;
                    }
                    if((y_state + y_speed + k) >= Y || (y_state + y_speed + k) < 0){
                        continue;
                    }
                    if(y_speed + k == 0 && x_speed + j == 0){
                        continue;
                    }
                    tmp_container = vector<int>();
                    tmp_container.push_back(x_state + x_speed + j);
                    tmp_container.push_back(y_state + y_speed + k);
                    tmp_container.push_back(x_speed + j);
                    tmp_container.push_back(y_speed + k);
                    
                    if(g_cost + 1 < g_costs[(((x_state + x_speed + j)*Y+(y_state + y_speed + k))*7+((x_speed + j) + 3))*7 + ((y_speed + k) + 3)] &&
                            obstacles[(x_state + x_speed + j)*Y + (y_state + y_speed + k)]){
                        g_costs[(((x_state + x_speed + j)*Y+(y_state + y_speed + k))*7+((x_speed + j) + 3))*7 + ((y_speed + k) + 3)] = g_cost + 1;
                        tmp_container.push_back(g_cost + 1 + calculate_heuristic(tmp_container, x2, y2));
                        to_visit.push(tmp_container);
                        //cout << x_state + x_speed + j << " " << y_state + y_speed + k << " " << x_speed + j << " " << y_speed + k << " " << g_cost + 1 << "\n";
                    }
                }
            }
        }
        delete [] obstacles;
        delete [] g_costs;
        while(!to_visit.empty()){
            to_visit.pop();
        }
        if(x_state == x2 && y_state == y2){
            cout << "Optimal solution takes " << g_cost << " hops.\n";
        } else {
            cout << "No solution.\n";
        }
        
    }
    
    return 0;
}
