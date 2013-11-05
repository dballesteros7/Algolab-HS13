#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <vector>
#include <cmath>

using namespace std;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;
typedef CGAL::Point_2<K> Point;

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
        for(vector<Point>::iterator it = points.begin(); it != points.end(); ++it){
            Point closestVertex = (trian.nearest_vertex(*it))->point();
            cout << closestVertex.x() << " " << closestVertex.y() << "\n";
            K::FT distance = CGAL::squared_distance(*it, closestVertex);
            if(first || distance < min_distance){
                first = false;
                min_distance = distance;
            }
        }
        double approximated_min = sqrt(CGAL::to_double(min_distance));
        cout << approximated_min << "\n";
        //while(approximated_min*approximated_min < min_distance) approximated_min++;
        //while((approximated_min - 1)*(approximated_min - 1) >= min_distance) approximated_min--;
        cout << setprecision(0) << approximated_min*100 << "\n";
        cin >> n;
    }
}
