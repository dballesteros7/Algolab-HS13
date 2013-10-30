#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Point_2.h>
#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>
#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>

using namespace CGAL;
using namespace std;

typedef Exact_predicates_exact_constructions_kernel_with_sqrt K;
typedef Point_2<K> Point;
typedef Min_circle_2_traits_2<K> CircleTraits;
typedef Min_circle_2<CircleTraits> Min_circle;

int main(){
    ios_base::sync_with_stdio(false);
    cout.setf(ios::fixed);
    int n;
    cin >> n;
    while(n != 0){
        vector<Point> population(n);
        for(int i = 0; i < n; ++i){
            double x,y;
            cin >> x;
            cin >> y;
            Point citizen(x,y);
            population[i] = citizen;
        }
        Min_circle antenna(population.begin(), population.end(), true);
        CircleTraits::Circle circle = antenna.circle();
        K::FT min_squared_radius = circle.squared_radius();
        for(Min_circle::Support_point_iterator it = antenna.support_points_begin(); it != antenna.support_points_end(); ++it){
            vector<Point>::iterator support_pos = find(population.begin(), population.end(), (*it));
            Point support_point = *support_pos;
            population.erase(support_pos);
            Min_circle currentAntenna(population.begin(), population.end(), true);
            if(currentAntenna.circle().squared_radius() < min_squared_radius){
                min_squared_radius = currentAntenna.circle().squared_radius();
            }
            population.push_back(support_point);
        }
        K::FT exact_radius = sqrt(min_squared_radius);
        double radius = ceil(to_double(exact_radius));
        while(radius < exact_radius) radius++;
        while(radius - 1 >= exact_radius) radius--;
        cout << setprecision(0) << radius << "\n"; 
        cin >> n;
    }

}
