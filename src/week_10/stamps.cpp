#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Segment_2.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <vector>

#ifdef CGAL_USE_GMP
#include <CGAL/Gmpz.h>
typedef CGAL::Gmpz ET;
#else
#include <CGAL/MP_Float.h>
typedef CGAL::MP_Float ET;
#endif

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Point_2<K> Point;
typedef CGAL::Segment_2<K> Segment;
typedef CGAL::Quadratic_program<K::FT> Program;
typedef CGAL::Quadratic_program_solution<K::FT> Solution;

using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    for(int i = 0; i < t; ++i){
        Program illumination(CGAL::SMALLER, true, 1, true, 4096);
        int l, s, w;
        cin >> l >> s >> w;
        vector<Point> lamps(l);
        for(int j = 0; j < l; ++j){
            int x,y;
            cin >> x >> y;
            Point lamp(x,y);
            lamps[j] = lamp;
            illumination.set_c(j, 1);
        }
        vector< vector<Segment> > light_segments(s, vector<Segment>());
        for(int j = 0; j < s; ++j){
            int x,y,m;
            cin >> x >> y >> m;
            Point stamp(x,y);
            for(int k = 0; k < l; ++k){
                Segment light_segment = Segment(stamp, lamps[k]);
                K::FT distance = light_segment.squared_length();
                illumination.set_a(k, j, 1/distance);
                illumination.set_a(k, j + s, 1/distance);
                light_segments[j].push_back(light_segment);
            }
            illumination.set_b(j, m);
            illumination.set_b(j + s, 1);
            illumination.set_r(j + s, CGAL::LARGER);
        }
        vector<Segment> walls(w);
        for(int j = 0; j < w; ++j){
            int a,b,c,d;
            cin >> a >> b >> c >> d;
            Point startPoint(a,b);
            Point endPoint(c,d);
            Segment wall(startPoint,endPoint);
            walls[j] = wall;
        }
        for(int j = 0; j < s; ++j){
            for(int k = 0; k < l; ++k){
                for(int z = 0; z < w; ++z){
                    if(CGAL::do_intersect(walls[z], light_segments[j][k])){
                        illumination.set_a(k, j, 0);
                        illumination.set_a(k, j + s, 0);
                        break;
                    }
                }
            }
        }
        //CGAL::print_quadratic_program(std::cout, illumination);
        Solution sol = CGAL::solve_quadratic_program(illumination, K::FT());
        if(sol.is_infeasible()){
            cout << "no\n";
        } else {
            cout << "yes\n";
        }
    }

}
