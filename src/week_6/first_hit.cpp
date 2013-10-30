#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Ray_2.h>
#include <CGAL/Segment_2.h>
#include <iostream>

using namespace CGAL;
using namespace std;

typedef Exact_predicates_exact_constructions_kernel K;
typedef Ray_2<K> Ray;
typedef Segment_2<K> Segment;
typedef Point_2<K> Point;

int main(){
    ios_base::sync_with_stdio(false);
    cout.setf(ios::fixed);
    int n;
    cin >> n;
    while(n != 0){
        double x,y,a,b;
        cin >> x;
        cin >> y;
        cin >> a;
        cin >> b;
        Point origin(x,y);
        Point path(a,b);
        Ray ray(origin, path);
        bool collides = false;
        K::FT min_distance;
        Object min_intersection;
        Point first_hit;
        for(int i = 0; i < n; ++i){
            double r, s, t, u;
            cin >> r;
            cin >> s;
            cin >> t;
            cin >> u;
            Point source(r,s);
            Point target(t,u);
            Segment obstacle(source, target);
            Object result = intersection(ray, obstacle);
            K::FT distance;
            bool correct = true;
            if(const Point *ipoint = object_cast<Point>(&result)){
                distance = squared_distance(origin, *ipoint);
            } else if(const Segment *isegment = object_cast<Segment>(&result)){
                distance = squared_distance(origin, *isegment);
            } else {
                correct = false;
            }
            if(correct && (!collides || distance < min_distance)){
                collides = true;
                min_distance = distance;
                min_intersection = result;
            }
        }
        if(!collides){
            cout << "no\n";
        } else {
            if (const Segment *isegment = object_cast<Segment>(&min_intersection)){
                K::FT distance_1 = squared_distance(origin, isegment->source());
                if(distance_1 <= min_distance){
                    first_hit = isegment->source(); 
                } else {
                    first_hit = isegment->target();
                }
            } else {
                first_hit = *(object_cast<Point>(&min_intersection));
            }
            K::FT x = first_hit.x();
            K::FT y = first_hit.y();
            double x_d = floor(to_double(x));
            double y_d = floor(to_double(y));
            while(x_d > x) x_d--;
            while(x_d + 1 <= x) x_d++;
            while(y_d > y) y_d--;
            while(y_d + 1 <= y) y_d++;
            cout << setprecision(0) << x_d << " " << y_d << "\n";
        }
        cin >> n;
    }
}

