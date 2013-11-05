#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/intersections.h>
#include <iostream>
#include <vector>
#include <queue>
#include <utility>
#include <ctime>
#include <cmath>

using namespace std;
using namespace CGAL;

typedef Exact_predicates_exact_constructions_kernel K;
typedef Line_2<K> Line;
typedef Point_2<K> Point;
typedef Triangle_2<K> Triangle;
typedef pair<int, int> Position;
typedef priority_queue<Position> MaxHeap;
typedef priority_queue<Position, vector<Position>, greater<Position> > MinHeap;

int main(){
    ios_base::sync_with_stdio(false);
    int c;
    cin >> c;
    for(int i = 0; i < c; ++i){
        int m,n;
        cin >> m >> n;
        vector<Point> path(m);
        vector<int> *positions = new vector<int> [m - 1];
        vector<int>::iterator *pointers = new vector<int>::iterator [m-1];
        bool *inTriangle = new bool[m];
        for(int j = 0; j < m; ++j){
            double px, py;
            cin >> px >> py;
            Point p(px,py);
            path[j] = p;
            if(j < m - 1)
                positions[j] = vector<int>();
            inTriangle[j] = false;
        }
        for(int j = 0; j < n; ++j){
            double q0x, q0y,q1x,q1y,q2x,q2y,q3x,q3y,q4x,q4y,q5x,q5y;
            cin >> q0x >> q0y >> q1x >> q1y >> q2x >> q2y >> q3x >> q3y >> q4x >> q4y >> q5x >> q5y;
            Line first(Point(q0x, q0y) ,Point(q1x, q1y));
            Line second(Point(q2x, q2y), Point(q3x, q3y));
            Line third(Point(q4x, q4y), Point(q5x, q5y));
            Object vertexA = intersection(first, second);
            Object vertexB = intersection(first, third);
            Object vertexC = intersection(second, third);
            Triangle map_part(*object_cast<Point>(&vertexA), *object_cast<Point>(&vertexB), *object_cast<Point>(&vertexC));
            for(int k = 0; k < m; ++k){
                Point a = path[k];
                if(map_part.has_on_bounded_side(a) || map_part.has_on_boundary(a)){
                    inTriangle[k] = true;
                }
            }
            for(int k = 0; k < m - 1; ++k){
                if(inTriangle[k] && inTriangle[k+1]){
                    positions[k].push_back(j);
                }
                inTriangle[k] = false;
            }
            inTriangle[m - 1] = false;
        }
        //if(i == 3 || i == 4){
        //    for(int j = 0; j < m - 1; ++j){
        //        cout << "Leg " << j << ":";
        //        for(vector<int>::iterator it = positions[j].begin(); it != positions[j].end(); ++it){
        //            cout << " " << *it;
        //        }
        //        cout << "\n";
        //    }
        //}
        MaxHeap maxes;
        MinHeap mins;
        for(int j = 0; j < m - 1; ++j){
            pointers[j] = positions[j].begin();
            if(!positions[j].empty()){
                Position tmp(positions[j][0], j);
                maxes.push(tmp);
                mins.push(tmp);
            }
        }
        int min_cost = maxes.top().first - mins.top().first + 1;
        bool condition = true;
        while(condition){
            Position current_min = mins.top();
            if(pointers[current_min.second] + 1 == positions[current_min.second].end()){
                condition = false;
            } else {    
                pointers[current_min.second]++;
                mins.pop();
                Position tmp(*(pointers[current_min.second]), current_min.second);
                mins.push(tmp);
                maxes.push(tmp);
                int cost = maxes.top().first - mins.top().first + 1;
                if(cost < min_cost){
                    //cout << maxes.top().first << " " << mins.top().first << "\n";
                    min_cost = cost;
                }
            }
        }
        cout << min_cost << "\n";
    }

}
