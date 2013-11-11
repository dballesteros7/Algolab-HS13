#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

using namespace std;
using namespace CGAL;

typedef Exact_predicates_inexact_constructions_kernel K;
typedef Point_2<K> Point;
typedef Delaunay_triangulation_2<K> Triangulation;
typedef Triangulation::Edge Edge;
typedef Triangulation::Finite_edges_iterator Iter;

int main(){
    ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    for(int i = 0; i < t; ++i){
        long n;
        cin >> n;
        long d;
        cin >> d;
        vector<Point> planes(n);
        set<Point> inDanger;
        for(int j = 0; j < n; ++j){
            long x,y;
            cin >> x >> y;
            planes[j] = Point(x,y);
        }
        Triangulation traffic;
        double d_squared = d*d;
        traffic.insert(planes.begin(), planes.end());
        for(Iter it = traffic.finite_edges_begin(); it != traffic.finite_edges_end(); ++it){
            Edge current = *it;
            Point source = current.first->vertex(current.first->cw(current.second))->point();
            Point target = current.first->vertex(current.first->ccw(current.second))->point();
            K::FT distance = traffic.segment(current).squared_length();
            if(distance < d_squared){
                inDanger.insert(source);
                inDanger.insert(target);
            }
        }
        cout << inDanger.size() << "\n";

    }
}
