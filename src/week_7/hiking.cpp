#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>

using namespace std;

typedef pair<long, long> Point;

// Sign of the cross product (q - p) x (r - p): 1 for a left turn, -1 for a right turn.
// The coordinates are integers, so with 128-bit products this is exact.
int orientation(const Point &p, const Point &q, const Point &r){
    __int128 left = (__int128)(q.first - p.first)*(r.second - p.second);
    __int128 right = (__int128)(q.second - p.second)*(r.first - p.first);
    return (left > right) - (left < right);
}

// Orient the three lines so that the triangle lies on their left side. The points of the
// next line lie on the boundary of the triangle; if the first one is the shared vertex use
// the second one.
void sortLines(Point q[6]){
    for(int j = 0; j < 6; j += 2){
        int oriented = orientation(q[j], q[j+1], q[(j+2)%6]);
        if(oriented == 0){
            oriented = orientation(q[j], q[j+1], q[(j+3)%6]);
        }
        if(oriented < 0){
            swap(q[j], q[j+1]);
        }
    }
}

int main(){
    ios_base::sync_with_stdio(false);
    int c;
    cin >> c;
    for(int i = 0; i < c; ++i){
        int m,n;
        cin >> m >> n;
        vector<Point> path(m);
        for(int j = 0; j < m; ++j){
            long px, py;
            cin >> px >> py;
            path[j] = Point(px,py);
        }
        // For every map part, the legs it contains completely, stored one after the other.
        vector<int> legs;
        vector<int> legs_start(n + 1, 0);
        for(int j = 0; j < n; ++j){
            Point q[6];
            for(int k = 0; k < 6; ++k){
                long qx, qy;
                cin >> qx >> qy;
                q[k] = Point(qx,qy);
            }
            sortLines(q);
            bool previousInTriangle = false;
            for(int k = 0; k < m; ++k){
                const Point &a = path[k];
                bool inTriangle = orientation(q[0], q[1], a) >= 0 && orientation(q[2], q[3], a) >= 0 && orientation(q[4], q[5], a) >= 0;
                if(previousInTriangle && inTriangle){
                    legs.push_back(k-1);
                }
                previousInTriangle = inTriangle;
            }
            legs_start[j + 1] = legs.size();
        }
        // Sliding window over the map parts, keep the window minimal while it covers every leg.
        vector<int> covered(m - 1, 0);
        int missing = m - 1;
        int min_cost = n;
        int first = 0;
        for(int last = 0; last < n; ++last){
            for(int k = legs_start[last]; k < legs_start[last + 1]; ++k){
                if(covered[legs[k]]++ == 0){
                    missing--;
                }
            }
            while(missing == 0){
                min_cost = min(min_cost, last - first + 1);
                for(int k = legs_start[first]; k < legs_start[first + 1]; ++k){
                    if(--covered[legs[k]] == 0){
                        missing++;
                    }
                }
                first++;
            }
        }
        cout << min_cost << "\n";
    }

}
