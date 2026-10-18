#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>

using namespace std;

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef CGAL::Point_2<K> Point;
typedef CGAL::Min_circle_2_traits_2<K> Min_circle_traits;
typedef CGAL::Min_circle_2<Min_circle_traits> Min_circle;

struct City {
    long long distance; // squared distance to the first city
    int x, y;
    bool operator<(const City &other) const { return distance > other.distance; }
};

// Squared radius of the smallest circle around the k cities farthest from the first one.
K::FT second_radius(const vector<City> &cities, int k){
    vector<Point> points;
    points.reserve(k);
    for(int j = 0; j < k; ++j){
        points.push_back(Point(cities[j].x, cities[j].y));
    }
    Min_circle secondAntenna(points.begin(), points.end(), true);
    return secondAntenna.circle().squared_radius();
}

int main(){
    ios_base::sync_with_stdio(false);
    cout.setf(ios::fixed);
    int t;
    cin >> t;
    for(int i = 0; i < t; ++i){
        int n;
        cin >> n;
        vector<City> cities(n);
        for(int j = 0; j < n; ++j){
            cin >> cities[j].x >> cities[j].y;
            long long dx = cities[j].x - (long long)cities[0].x;
            long long dy = cities[j].y - (long long)cities[0].y;
            cities[j].distance = dx*dx + dy*dy;
        }
        if(n < 3){
            cout << "0\n";
            continue;
        }
        sort(cities.begin(), cities.end());
        // Giving the k farthest cities to the second antenna costs
        // max(second_radius(k), cities[k].distance): the first term grows with k,
        // the second shrinks, so binary search for the first k where they cross.
        int low = 1, high = n - 1;
        K::FT high_radius = -1; // second_radius(cities, high), once computed
        while(low < high){
            int mid = (low + high)/2;
            K::FT radius = second_radius(cities, mid);
            if(radius >= double(cities[mid].distance)){
                high = mid;
                high_radius = radius;
            } else {
                low = mid + 1;
            }
        }
        if(high_radius < 0){
            high_radius = second_radius(cities, high);
        }
        // Either cut at the crossing point or one city earlier.
        K::FT min_radius = high_radius;
        if(high > 1 && double(cities[high - 1].distance) < min_radius){
            min_radius = double(cities[high - 1].distance);
        }
        double value = ceil(CGAL::to_double(min_radius));
        while(value < min_radius) ++value;
        while(value - 1 >= min_radius) --value;
        cout << setprecision(0) << value << "\n";
    }
}
