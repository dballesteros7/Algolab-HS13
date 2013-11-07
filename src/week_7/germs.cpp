#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;
using namespace CGAL;

typedef Exact_predicates_inexact_constructions_kernel K;
typedef Delaunay_triangulation_2<K> Triangulation;
typedef Segment_2<K> Segment;
typedef Point_2<K> Point;

// I think comments will help my coding!
// Formula for bacteria growth is 0.5 + t^2
int main(){
    ios_base::sync_with_stdio(false);
    cout.setf(ios::fixed);
    int n;
    cin >> n;
    while(n > 0){
        // Corners of the rectangle
        double l,b,r,t;
        cin >> l >> b >> r >> t;
        Point r1(l,b);
        Point r2(l,t);
        Point r3(r,b);
        Point r4(r,t);
        // Sides of the rectangle
        Segment r1r2 = Segment(r1,r2);
        Segment r2r3 = Segment(r2,r4);
        Segment r3r4 = Segment(r4,r3);
        Segment r4r1 = Segment(r3,r1);
        Segment rectangle[4] = {r1r2,r2r3,r3r4,r4r1};

        // Bacteria in the dish
        vector<Point> bacteria(n);
        vector<K::FT> minSquaredDistances(n);
        map<Point, int> bacteria_map;
        for(int i = 0; i < n; ++i){
            double x,y;
            cin >> x >> y;
            bacteria[i] = Point(x,y);
            bacteria_map.insert(pair<Point, int>(bacteria[i], i));
            minSquaredDistances[i] = squared_distance(rectangle[0], bacteria[i]);
            for(int j = 1; j < 4; ++j){
                K::FT distance = squared_distance(rectangle[j], bacteria[i]);
                if(distance < minSquaredDistances[i]){
                    minSquaredDistances[i] = distance;
                }
            }
        }

        // Triangulate the bacteria using Delaunay
        Triangulation bacteria_in_dish;
        bacteria_in_dish.insert(bacteria.begin(), bacteria.end());
        // Now for each bacteria find its nearest obstacle, put the distances in a vector
        for(Triangulation::Finite_edges_iterator it = bacteria_in_dish.finite_edges_begin(); it != bacteria_in_dish.finite_edges_end(); ++it){
            Segment edgeSegment = bacteria_in_dish.segment(*it);
            K::FT distance = edgeSegment.squared_length()/4;
            if(distance < minSquaredDistances[bacteria_map[edgeSegment.source()]]) {
                minSquaredDistances[bacteria_map[edgeSegment.source()]] = distance;
            }
            if(distance < minSquaredDistances[bacteria_map[edgeSegment.target()]]) {
                minSquaredDistances[bacteria_map[edgeSegment.target()]] = distance;
            }
        }
        // Sort the distances to find the 0%, 50% and 100% of deaths.
        sort(minSquaredDistances.begin(), minSquaredDistances.end());
        K::FT dexact = minSquaredDistances[0];
        // Do the approximations!
        if(dexact <= 0.25){
            cout << 0 << " ";
        } else {
            double t1 = ceil(std::sqrt(std::sqrt(to_double(dexact)) - 0.5));
            double d1 = (0.5 + t1*t1)*(0.5 + t1*t1);
            while(d1 < dexact){
                t1++;
                d1 = (0.5 + t1*t1)*(0.5 + t1*t1);
            }
            double t2 = t1 - 1;
            double d2 = (0.5 + t2*t2)*(0.5 + t2*t2);
            while(d2 >= dexact){
                t1--;
                t2--;
                d2 = (0.5 + t2*t2)*(0.5 + t2*t2);
            }
            cout << setprecision(0) << t1 << " ";
        }
        dexact = minSquaredDistances[ceil(n/2)];
        // Do the approximations!
        if(dexact <= 0.25){
            cout << 0 << " ";
        } else {
            double t1 = ceil(std::sqrt(std::sqrt(to_double(dexact)) - 0.5));
            double d1 = (0.5 + t1*t1)*(0.5 + t1*t1);
            while(d1 < dexact){
                t1++;
                d1 = (0.5 + t1*t1)*(0.5 + t1*t1);
            }
            double t2 = t1 - 1;
            double d2 = (0.5 + t2*t2)*(0.5 + t2*t2);
            while(d2 >= dexact){
                t1--;
                t2--;
                d2 = (0.5 + t2*t2)*(0.5 + t2*t2);
            }
            cout << setprecision(0) << t1 << " ";
        }
        dexact = minSquaredDistances[n - 1];
        // Do the approximations!
        if(dexact <= 0.25){
            cout << 0 << " ";
        } else {
            double t1 = ceil(std::sqrt(std::sqrt(to_double(dexact)) - 0.5));
            double d1 = (0.5 + t1*t1)*(0.5 + t1*t1);
            while(d1 < dexact){
                t1++;
                d1 = (0.5 + t1*t1)*(0.5 + t1*t1);
            }
            double t2 = t1 - 1;
            double d2 = (0.5 + t2*t2)*(0.5 + t2*t2);
            while(d2 >= dexact){
                t1--;
                t2--;
                d2 = (0.5 + t2*t2)*(0.5 + t2*t2);
            }
            cout << setprecision(0) << t1 << "\n";
        }
        cin >> n;
    }
}
