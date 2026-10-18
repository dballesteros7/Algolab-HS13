#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>
#include <iostream>
#include <vector>
#include <cmath>

using namespace CGAL;
using namespace std;

// The fast kernel finds the candidate support points, the exact kernel only ever sees those (at most 3).
typedef Exact_predicates_inexact_constructions_kernel IK;
typedef Exact_predicates_exact_constructions_kernel K;
typedef IK::Point_2 Point;
typedef Min_circle_2<Min_circle_2_traits_2<IK> > Min_circle;
typedef Min_circle_2<Min_circle_2_traits_2<K> > Exact_min_circle;

// Smallest integer d with d*d >= x.
double ceil_sqrt(const K::FT &x){
    double d = ceil(sqrt(to_double(x)));
    while(K::FT(d) * K::FT(d) < x) d++;
    while(d > 0 && K::FT(d - 1) * K::FT(d - 1) >= x) d--;
    return d;
}

// Exact squared radius of the minimum enclosing circle of population.
K::FT min_squared_radius(vector<Point> &population){
    vector<K::Point_2> candidates;
    try {
        Min_circle approximate(population.begin(), population.end(), true);
        for(Min_circle::Support_point_iterator it = approximate.support_points_begin(); it != approximate.support_points_end(); ++it)
            candidates.push_back(K::Point_2(it->x(), it->y()));
    } catch(Assertion_exception &){
        // Inexact circle constructions on (nearly) collinear points can trip an assertion.
        candidates.push_back(K::Point_2(population[0].x(), population[0].y()));
    }
    // The exact circle of a subset is the answer iff it contains every point. Points outside
    // (only possible if the inexact kernel erred) are added to the subset, which is retried a few times.
    for(int round = 0; round < 3; ++round){
        Exact_min_circle exact(candidates.begin(), candidates.end(), true);
        vector<Point> s;
        for(Exact_min_circle::Support_point_iterator it = exact.support_points_begin(); it != exact.support_points_end(); ++it)
            s.push_back(Point(to_double(it->x()), to_double(it->y())));
        size_t outside = 0;
        for(size_t i = 0; i < population.size() && outside < 100; ++i){
            const Point &p = population[i];
            // Exact predicates on input points only, no constructions.
            bool contained;
            if(s.size() == 3)
                contained = side_of_bounded_circle(s[0], s[1], s[2], p) != ON_UNBOUNDED_SIDE;
            else if(s.size() == 2)
                contained = side_of_bounded_circle(s[0], s[1], p) != ON_UNBOUNDED_SIDE;
            else
                contained = (p == s[0]);
            if(!contained){
                candidates.push_back(K::Point_2(p.x(), p.y()));
                ++outside;
            }
        }
        if(outside == 0)
            return exact.circle().squared_radius();
    }
    vector<K::Point_2> exact_population;
    for(size_t i = 0; i < population.size(); ++i)
        exact_population.push_back(K::Point_2(population[i].x(), population[i].y()));
    return Exact_min_circle(exact_population.begin(), exact_population.end(), true).circle().squared_radius();
}

int main(){
    ios_base::sync_with_stdio(false);
    cout.setf(ios::fixed);
    int n;
    cin >> n;
    while(n != 0){
        vector<Point> population(n);
        for(int i = 0; i < n; ++i){
            // Coordinates are integers below 2^48, so they are exact as doubles.
            long x,y;
            cin >> x >> y;
            population[i] = Point(x,y);
        }
        cout << setprecision(0) << ceil_sqrt(min_squared_radius(population)) << "\n";
        cin >> n;
    }

}
