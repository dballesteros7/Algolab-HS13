#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <vector>
#include <queue>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;
typedef CGAL::Point_2<K> Point;
typedef CGAL::Segment_2<K> Segment;
typedef CGAL::Ray_2<K> Ray;
typedef Triangulation::Face_handle FaceHandle;
typedef Triangulation::Edge Edge;

using namespace std;
using namespace CGAL;

int main(){
    ios_base::sync_with_stdio(false);
    cout.setf(ios::fixed);
    int n;
    cin >> n;
    while(n > 0){
        vector<Point> infected(n);
        for(int i = 0; i < n; ++i){
            double x,y;
            cin >> x >> y;
            infected[i] = Point(x,y);
        }
        Triangulation walking_dead;
        walking_dead.insert(infected.begin(), infected.end());
        int m;
        cin >> m;
        for(int i = 0; i < m; ++i){
            double x,y,d;
            cin >> x >> y >> d;
            Point survivor(x,y);
            Point walker = walking_dead.nearest_vertex(survivor)->point();
            if(squared_distance(walker, survivor) < d){
                cout << "n";
                continue;
            }
            FaceHandle startFace = walking_dead.locate(survivor);
            if(walking_dead.is_infinite(startFace)){
                cout << "y";
                continue;
            }
            queue<FaceHandle> toVisit;
            set<FaceHandle> visited;
            toVisit.push(startFace);
            bool exitFound = false;
            while(!toVisit.empty() && !exitFound){
                FaceHandle current = toVisit.front();
                toVisit.pop();
                if(visited.find(current) != visited.end()){
                    continue;
                }
                for(int j = 0; j < 3; ++j){
                    FaceHandle neighbor = current->neighbor(j);
                    if(visited.find(neighbor) != visited.end()){
                        continue;
                    }
                    Edge escapeEdge(current, j);
                    if(!walking_dead.is_infinite(neighbor)){
                        // First check that we can escape through the segment
                        if(walking_dead.segment(escapeEdge).squared_length() < 4*d){
                            continue;
                        }
                        // We can move there, add it to the visit list
                        toVisit.push(neighbor);
                    } else{
                        // This is a final escape route, we need to make sure we can go through it
                        Segment tmp = walking_dead.segment(escapeEdge);
                        // Check that the escapeEdge is big enough
                        if(tmp.squared_length() < 4*d){
                            continue;
                        }
                        // If it is then we found an exit!
                        exitFound = true;
                        break;
                    }
                }
                visited.insert(current);
            }
            if(exitFound){
                cout << "y";
            } else {
                cout << "n";
            }
        }
        cout << "\n";
        cin >> n;
    }
}   
