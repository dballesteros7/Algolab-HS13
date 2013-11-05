#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/intersections.h>
#include <iostream>
#include <vector>
#include <queue>
#include <utility>
#include <ctime>
#include <cmath>

using namespace std;
using namespace CGAL;

typedef Exact_predicates_inexact_constructions_kernel K;
typedef Line_2<K> Line;
typedef Point_2<K> Point;
typedef Triangle_2<K> Triangle;
typedef Direction_2<K> Direction;
typedef pair<int, int> Position;
typedef priority_queue<Position> MaxHeap;
typedef priority_queue<Position, vector<Position>, greater<Position> > MinHeap;

vector<Line> sortLines(Point a1, Point a2, Point b1, Point b2, Point c1, Point c2){
    vector<Line> result;
    Line a(a1,a2);
    Line b(b1,b2);
    Line c(c1,c2);
    if(!a.has_on_positive_side(b1)){
        a = a.opposite();
    }
    if(!b.has_on_positive_side(a1)){
        b = b.opposite();
    }
    if(!c.has_on_positive_side(a1)){
        c = c.opposite();
    }
    result.push_back(a);
    result.push_back(b);
    result.push_back(c);
    return result;
}

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
            inTriangle[j] = true;
        }
        MaxHeap maxes;
        MinHeap mins;
        for(int j = 0; j < n; ++j){
            double q0x, q0y,q1x,q1y,q2x,q2y,q3x,q3y,q4x,q4y,q5x,q5y;
            cin >> q0x >> q0y >> q1x >> q1y >> q2x >> q2y >> q3x >> q3y >> q4x >> q4y >> q5x >> q5y;
            vector<Line> oriented_lines = sortLines(Point(q0x,q0y), Point(q1x,q1y), Point(q2x, q2y), Point(q3x,q3y), Point(q4x, q4y), Point(q5x, q5y));
            for(int k = 0; k < m; ++k){
                Point a = path[k];
                for(vector<Line>::iterator it = oriented_lines.begin(); it != oriented_lines.end(); ++it){
                    if(!(it->has_on_positive_side(a) || it->has_on(a))){
                        inTriangle[k] = false;
                        break;
                    }
                }
                if(k > 0){
                    if(inTriangle[k-1] && inTriangle[k]){
                        positions[k-1].push_back(j);
                    }
                    inTriangle[k-1] = true;
                }
            }
            inTriangle[m - 1] = true;
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
