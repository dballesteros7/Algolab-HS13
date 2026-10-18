#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <vector>
#include <cmath>

using namespace std;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;
typedef CGAL::Point_2<K> Point;
typedef Triangulation::Finite_edges_iterator EdgeIter;
typedef Triangulation::Face_handle FaceHandle;

int main(){
    ios_base::sync_with_stdio(false);
    int n;
    cin >> n;
    while(n != 0){
        vector<Point> points(n);
        for(int i = 0; i < n; ++i){
            long a,b;
            cin >> a >> b;
            Point x(a,b);
            points[i] = x;
        }
        Triangulation trian;
        trian.insert(points.begin(), points.end());
        // The closest pair is a Delaunay edge. Coordinates are integers below 2^24,
        // so the squared lengths are computed exactly with 64-bit integers.
        long min_distance = -1;
        for(EdgeIter it = trian.finite_edges_begin(); it != trian.finite_edges_end(); ++it){
            FaceHandle face = it->first;
            const Point &p = face->vertex(Triangulation::cw(it->second))->point();
            const Point &q = face->vertex(Triangulation::ccw(it->second))->point();
            long dx = (long) p.x() - (long) q.x();
            long dy = (long) p.y() - (long) q.y();
            long distance = dx*dx + dy*dy;
            if(min_distance < 0 || distance < min_distance){
                min_distance = distance;
            }
        }
        // Smallest integer t with t^2 >= 50^2 * d^2, i.e. ceil(50 * d). Fits in 63 bits.
        long exact_time_s = 50*50*min_distance;
        long approx = ceil(sqrt((double) exact_time_s));
        while(approx*approx < exact_time_s) approx++;
        while((approx - 1)*(approx - 1) >= exact_time_s) approx--;
        cout << approx << "\n";
        cin >> n;
    }
}
