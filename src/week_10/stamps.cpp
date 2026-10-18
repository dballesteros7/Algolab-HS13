#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpzf.h>
#include <iostream>
#include <vector>

typedef CGAL::Gmpzf ET;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 Point;
typedef K::Segment_2 Segment;
typedef CGAL::Quadratic_program<double> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    for(int i = 0; i < t; ++i){
        // Lamp powers lie in [1, 2^12]; stamp j needs an intensity in [1, M_j].
        Program illumination(CGAL::SMALLER, true, 1, true, 4096);
        int l, s, w;
        cin >> l >> s >> w;
        vector<Point> lamps(l);
        for(int j = 0; j < l; ++j){
            int x,y;
            cin >> x >> y;
            lamps[j] = Point(x,y);
        }
        vector<Point> stamps(s);
        vector<int> limits(s);
        for(int j = 0; j < s; ++j){
            int x,y;
            cin >> x >> y >> limits[j];
            stamps[j] = Point(x,y);
        }
        vector<Segment> walls(w);
        for(int j = 0; j < w; ++j){
            int a,b,c,d;
            cin >> a >> b >> c >> d;
            walls[j] = Segment(Point(a,b), Point(c,d));
        }
        for(int j = 0; j < s; ++j){
            for(int k = 0; k < l; ++k){
                Segment light_segment(stamps[j], lamps[k]);
                bool blocked = false;
                for(int z = 0; z < w && !blocked; ++z){
                    blocked = CGAL::do_intersect(walls[z], light_segment);
                }
                if(!blocked){
                    double intensity = 1/light_segment.squared_length();
                    illumination.set_a(k, j, intensity);
                    illumination.set_a(k, j + s, intensity);
                }
            }
            illumination.set_b(j, limits[j]);
            illumination.set_b(j + s, 1);
            illumination.set_r(j + s, CGAL::LARGER);
        }
        // Only feasibility matters, so there is no objective.
        Solution sol = CGAL::solve_linear_program(illumination, ET());
        if(sol.is_infeasible()){
            cout << "no\n";
        } else {
            cout << "yes\n";
        }
    }
}
