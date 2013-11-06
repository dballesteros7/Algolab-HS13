#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

using namespace std;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;
typedef CGAL::Point_2<K> Point;

int main(){
    ios_base::sync_with_stdio(false);
    cout.setf(ios::fixed);
    int n;
    cin >> n;
    while(n > 0){
        vector<Point> restaurants(n);
        for(int i = 0; i < n; ++i){
            double a,b;
            cin >> a >> b;
            restaurants[i] = Point(a,b); 
        }
        Triangulation restaurant_network;
        restaurant_network.insert(restaurants.begin(), restaurants.end());
        int m;
        cin >> m;
        for(int i = 0; i < m; ++i){
            double a,b;
            cin >> a >> b;
            Point x = restaurant_network.nearest_vertex(Point(a,b))->point();
            cout << setprecision(0) << CGAL::squared_distance(Point(a,b), x) << "\n";
        }
        cin >> n;
    }
}
