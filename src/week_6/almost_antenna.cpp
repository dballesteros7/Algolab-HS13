#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>
#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>

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

// Exact squared radius of the minimum enclosing circle of population, and its support points.
K::FT min_squared_radius(vector<Point> &population, vector<Point> &support){
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
        if(outside == 0){
            support = s;
            return exact.circle().squared_radius();
        }
    }
    vector<K::Point_2> exact_population;
    for(size_t i = 0; i < population.size(); ++i)
        exact_population.push_back(K::Point_2(population[i].x(), population[i].y()));
    Exact_min_circle exact(exact_population.begin(), exact_population.end(), true);
    support.clear();
    for(Exact_min_circle::Support_point_iterator it = exact.support_points_begin(); it != exact.support_points_end(); ++it)
        support.push_back(Point(to_double(it->x()), to_double(it->y())));
    return exact.circle().squared_radius();
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
        vector<Point> support;
        K::FT min_squared_radius_found = min_squared_radius(population, support);
        // Dropping a point that is not a support point leaves the circle unchanged.
        for(size_t j = 0; j < support.size(); ++j){
            vector<Point>::iterator support_pos = find(population.begin(), population.end(), support[j]);
            Point support_point = *support_pos;
            *support_pos = population.back();
            population.pop_back();
            vector<Point> ignored;
            K::FT squared_radius = population.empty() ? K::FT(0) : min_squared_radius(population, ignored);
            if(squared_radius < min_squared_radius_found){
                min_squared_radius_found = squared_radius;
            }
            population.push_back(support_point);
        }
        cout << setprecision(0) << ceil_sqrt(min_squared_radius_found) << "\n";
        cin >> n;
    }

}
