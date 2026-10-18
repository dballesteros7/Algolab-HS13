#include <iostream>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>

#ifdef CGAL_USE_GMP
#include <CGAL/Gmpz.h>
typedef CGAL::Gmpz ET;
#else
#include <CGAL/MP_Float.h>
typedef CGAL::MP_Float ET;
#endif

typedef CGAL::Quadratic_program<long> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

using namespace std;

// Exact floor (to_double alone may round an integral value down to k - 1 + eps).
double floor_to_double(const CGAL::Quotient<ET> &x){
    double a = floor(CGAL::to_double(x));
    while(a > x) a -= 1;
    while(a + 1 <= x) a += 1;
    return a;
}

int main(){
    ios_base::sync_with_stdio(false);
    cout.setf(ios::fixed);
    int n,m;
    cin >> n >> m;
    while(n != 0 && m != 0){
        Program dietPlan(CGAL::SMALLER, true, 0, false, 0);
        for(int i = 0; i < n; ++i){
            long l, u;
            cin >> l >> u;
            dietPlan.set_b(i, l);
            dietPlan.set_r(i, CGAL::LARGER);
            dietPlan.set_b(i + n, u);
        }
        for(int i = 0; i < m; ++i){
            long p;
            cin >> p;
            dietPlan.set_c(i, p);
            for(int j = 0; j < n; ++j){
                long c;
                cin >> c;
                dietPlan.set_a(i, j, c);
                dietPlan.set_a(i, j + n, c);
            }
        }
        // Linear objective and x >= 0: the specialised LP solver is much faster.
        Solution sol = CGAL::solve_nonnegative_linear_program(dietPlan, ET());
        if(sol.is_infeasible()){
            cout << "No such diet.\n";
        } else {
            long val = floor_to_double(sol.objective_value());
            cout << setprecision(0) << val << "\n";
        }
        cin >> n >> m;
    }

}
