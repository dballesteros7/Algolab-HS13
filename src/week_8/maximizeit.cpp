#include <iostream>
#include <cassert>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>


// choose exact integral type
#ifdef CGAL_USE_GMP
#include <CGAL/Gmpz.h>
typedef CGAL::Gmpz ET;
#else
#include <CGAL/MP_Float.h>
typedef CGAL::MP_Float ET;
#endif

using namespace std;

typedef CGAL::Quadratic_program<int> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

int main(){
    ios_base::sync_with_stdio(false);
    cout.setf(ios::fixed);
    int p;
    cin >> p;
    Program skeletonA(CGAL::SMALLER, true, 0, false, 0);
    Program skeletonB(CGAL::SMALLER, false, 0, true, 0);
    
    // SkeletonA
    // min ax^2 - by
    // s.t.
    // x,y >= 0
    // x + y <= 4
    // 4x + 2y <= ab
    // -x + y <= 1
    skeletonA.set_a(0, 0, 1);
    skeletonA.set_a(0, 1, 4);
    skeletonA.set_a(0, 2, -1);
    skeletonA.set_a(1, 0, 1);
    skeletonA.set_a(1, 1, 2);
    skeletonA.set_a(1, 2, 1);
    skeletonA.set_b(0, 4);
    skeletonA.set_b(2, 1);

    // SkeletonB
    // min ax^2 + by + z2
    // s.t.
    // x, y <= 0
    // z >= 0
    // - x - y <= 4
    // -4x - 2y - z <= ab
    // x - y <= 1
    skeletonB.set_l(2, true, 0);
    skeletonB.set_u(2, false, 0);
    skeletonB.set_a(0, 0, -1);
    skeletonB.set_a(0, 1, -4);
    skeletonB.set_a(0, 2, 1);
    skeletonB.set_a(1, 0, -1);
    skeletonB.set_a(1, 1, -2);
    skeletonB.set_a(1, 2, -1);
    skeletonB.set_a(2, 1, -1);
    skeletonB.set_b(0, 4);
    skeletonB.set_b(2, 1);
    skeletonB.set_d(2,2,2);
    while(p != 0){
        int a,b;
        cin >> a >> b;
        if(p == 1){
            skeletonA.set_b(1, a*b);
            skeletonA.set_c(1, -b);
            skeletonA.set_d(0,0, 2*a);
            Solution sol = solve_quadratic_program(skeletonA, ET());
            if(sol.is_infeasible()){
                cout << "no\n";
            } else if (sol.is_unbounded()){
                cout << "unbounded\n";
            } else {
                int val = floor(-CGAL::to_double(sol.objective_value()));
                cout << setprecision(0) << val << "\n";
            }
        } else {
            skeletonB.set_b(1, a*b);
            skeletonB.set_c(1, b);
            skeletonB.set_d(0, 0, 2*a);
            Solution sol = solve_quadratic_program(skeletonB, ET());
            if(sol.is_infeasible()){
                cout << "no\n";
            } else if (sol.is_unbounded()){
                cout << "unbounded\n";
            } else {
                int val = ceil(CGAL::to_double(sol.objective_value()));
                cout << setprecision(0) << val << "\n";
            }
        }
        cin >> p;
    }

}
