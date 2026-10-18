#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <cmath>

using namespace CGAL;
using namespace std;

// Exact constructions only for the few intersections actually computed; the cheap kernel
// stores the input and filters out obstacles that miss the ray (its predicates are exact).
typedef Exact_predicates_inexact_constructions_kernel IK;
typedef Exact_predicates_exact_constructions_kernel K;
typedef K::Ray_2 Ray;
typedef K::Segment_2 Segment;
typedef K::Point_2 Point;

// Exact floor of an exact number (to_double alone may be off by one).
double floor_to_double(const K::FT &x){
    double a = floor(to_double(x));
    while(a > x) a -= 1;
    while(a + 1 <= x) a += 1;
    return a;
}

int main(){
    ios_base::sync_with_stdio(false);
    mt19937 generator(42);
    int n;
    cin >> n;
    while(n != 0){
        long x,y,a,b;
        cin >> x >> y >> a >> b;
        Point origin(x,y);
        Ray ray(origin, Point(a,b));
        IK::Ray_2 approximate_ray(IK::Point_2(x,y), IK::Point_2(a,b));
        vector<IK::Segment_2> obstacles(n);
        for(int i = 0; i < n; ++i){
            long r, s, t, u;
            cin >> r >> s >> t >> u;
            obstacles[i] = IK::Segment_2(IK::Point_2(r,s), IK::Point_2(t,u));
        }
        // In a random order the closest hit so far improves only O(log n) times in expectation,
        // so only that many intersections are constructed; everything else is a cheap predicate.
        shuffle(obstacles.begin(), obstacles.end(), generator);
        bool collides = false;
        Point first_hit;
        Segment clipped;
        for(int i = 0; i < n; ++i){
            if(!do_intersect(approximate_ray, obstacles[i]))
                continue;
            // Input coordinates are exact as doubles, so this conversion is exact.
            Segment obstacle(Point(obstacles[i].source().x(), obstacles[i].source().y()),
                             Point(obstacles[i].target().x(), obstacles[i].target().y()));
            // Once something is hit, only obstacles crossing the segment origin--first_hit can be closer.
            if(collides && !do_intersect(clipped, obstacle))
                continue;
            // Intersect with the original ray so that constructions are never nested.
            Object result = intersection(ray, obstacle);
            Point hit;
            if(const Point *ipoint = object_cast<Point>(&result)){
                hit = *ipoint;
            } else if(const Segment *isegment = object_cast<Segment>(&result)){
                hit = has_smaller_distance_to_point(origin, isegment->source(), isegment->target()) ? isegment->source() : isegment->target();
            } else {
                continue;
            }
            if(!collides || has_smaller_distance_to_point(origin, hit, first_hit)){
                collides = true;
                first_hit = hit;
                if(first_hit == origin) break;
                clipped = Segment(origin, first_hit);
            }
        }
        if(!collides){
            cout << "no\n";
        } else {
            cout << (long) floor_to_double(first_hit.x()) << " " << (long) floor_to_double(first_hit.y()) << "\n";
        }
        cin >> n;
    }
}
