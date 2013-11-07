#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <vector>
#include <cmath>

using namespace std;

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;
typedef CGAL::Point_2<K> Point;
typedef Triangulation::Finite_edges_iterator EdgeIter;
typedef Triangulation::Face_handle FaceHandle;

int main(){
    ios_base::sync_with_stdio(false);
    cout.setf(ios::fixed);
    int n;
    cin >> n;
    while(n != 0){
        vector<Point> points(n);
        for(int i = 0; i < n; ++i){
            double a,b;
            cin >> a >> b;
            Point x(a,b);
            points[i] = x;
        }
        Triangulation trian;
        trian.insert(points.begin(), points.end());
        K::FT min_distance;
        bool first = true;
        for(EdgeIter it = trian.finite_edges_begin(); it != trian.finite_edges_end(); ++it){
            K::FT distance = trian.segment(*it).squared_length();
            if(first || distance < min_distance){
                first = false;
                min_distance = distance;
            }

        }
        K::FT exact_time_s = 50*50*min_distance;
        double approx = ceil(sqrt(CGAL::to_double(exact_time_s)));
        while(approx*approx < exact_time_s) approx++;
        while((approx - 1)*(approx-1) >= exact_time_s) approx--;
        cout << setprecision(0) << approx << "\n";
        cin >> n;
    }
}
