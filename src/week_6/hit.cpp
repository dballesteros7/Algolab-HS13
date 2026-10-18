#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <iostream>

using namespace CGAL;
using namespace std;

// Only a predicate (do_intersect) is needed, so the inexact constructions kernel is enough.
typedef Exact_predicates_inexact_constructions_kernel K;
typedef K::Ray_2 Ray;
typedef K::Segment_2 Segment;
typedef K::Point_2 Point;

int main(){
    ios_base::sync_with_stdio(false);
    int n;
    cin >> n;
    while(n != 0){
        // Coordinates are integers below 2^51, so they are exact as doubles.
        long x,y,a,b;
        cin >> x >> y >> a >> b;
        Ray ray(Point(x,y), Point(a,b));
        bool collides = false;
        for(int i = 0; i < n; ++i){
            long r, s, t, u;
            cin >> r >> s >> t >> u;
            if(!collides && do_intersect(ray, Segment(Point(r,s), Point(t,u))))
                collides = true;
        }
        if(collides)
            cout << "yes\n";
        else
            cout << "no\n";
        cin >> n;
    }
}
