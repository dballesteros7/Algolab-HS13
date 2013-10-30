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
        for(int i = 0; i < n; ++i){
            double r, s, t, u;
            cin >> r;
            cin >> s;
            cin >> t;
            cin >> u;
            if(!collides){
                Point source(r,s);
                Point target(t,u);
                Segment obstacle(source,target);
                if(do_intersect(ray,obstacle))
                    collides = true;
            }
        }
        if(collides)
            cout << "yes\n";
        else
            cout << "no\n";
        cin >> n;
    }
}
