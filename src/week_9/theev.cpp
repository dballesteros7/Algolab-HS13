#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>
#include <algorithm>
#include <vector>
#include <limits>

using namespace std;

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef CGAL::Point_2<K> Point;
typedef CGAL::Circle_2<K> Circle;
typedef CGAL::Min_circle_2_traits_2<K> Min_circle_traits;
typedef CGAL::Min_circle_2<Min_circle_traits> Min_circle;

int main(){
    ios_base::sync_with_stdio(false);
    cout.setf(ios::fixed);
    if (numeric_limits<int>::max() < 33554432.0){
        throw std::range_error("max(int) < 2^(25)");
    }
    int t;
    cin >> t;
    for(int i = 0; i < t; ++i){
        int n;
        cin >> n;
        vector< pair<K::FT, Point> > points;
        for(int j = 0; j < n; ++j){
            int x, y;
            cin >> x;
            cin >> y;
            if(j == 0){
                points.push_back(pair<K::FT, Point>(0, Point(x,y)));
            } else {
                Point city = Point(x,y);
                K::FT distance = CGAL::squared_distance(city, points[0].second);
                points.push_back(pair<K::FT, Point>(distance, city));
            }
        }
        sort(points.begin(), points.end(), greater< pair<K::FT, Point> >());
        if(n < 3){
            cout << "0\n";
        }
        Min_circle secondAntenna;
        secondAntenna.insert(points[0].second);
        K::FT min_radius = points[1].first;
        for(vector< pair<K::FT, Point> >::iterator it = points.begin() + 1; it != points.end() - 1; ++it){
            secondAntenna.insert(it->second);
            K::FT current_radius = max((it + 1)->first, secondAntenna.circle().squared_radius());
            if(current_radius < min_radius){
                min_radius = current_radius;
            }
        }
        double value = ceil(CGAL::to_double(min_radius));
        while(value < min_radius) ++value;
        while(value - 1 >= min_radius) --value;
        cout << setprecision(0) << value << "\n";
    }
}
