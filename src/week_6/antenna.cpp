#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Point_2.h>
#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>
#include <iostream>
#include <cmath>

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
        Point* population = new Point[n];
        Min_circle antenna;
        for(int i = 0; i < n; ++i){
            double x,y;
            cin >> x;
            cin >> y;
            Point citizen(x,y);
            population[i] = citizen;
        }
        Min_circle antenna(&population[0], &population[n], true);
        CircleTraits::Circle circle = antenna.circle();
        K::FT exact_radius = sqrt(circle.squared_radius());
        double radius = ceil(to_double(exact_radius));
        while(radius < exact_radius) radius++;
        while(radius - 1 >= exact_radius) radius--;
        cout << setprecision(0) << radius << "\n"; 
        cin >> n;
    }

}
