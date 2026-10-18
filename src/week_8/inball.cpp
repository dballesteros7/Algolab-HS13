#include <iostream>
#include <cmath>
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
    
    int n;
    cin >> n;
    while(n > 0){
        int d;
        cin >> d;
        // Maximize r s.t. a_i x + |a_i| r <= b_i, r >= 0, x free.
        Program ball_finder(CGAL::SMALLER, false, 0, false, 0);
        ball_finder.set_c(d, -1);
        ball_finder.set_l(d, true, 0);
        for(int i = 0; i < n; ++i){
            long squared_sum = 0;
            for(int j = 0; j < d; ++j){
                long a;
                cin >> a;
                squared_sum += a*a;
                ball_finder.set_a(j, i, a);
            }
            // The norms are guaranteed to be integral; fix up any rounding of sqrt.
            long a_norm = sqrt((double) squared_sum);
            while(a_norm * a_norm > squared_sum) a_norm--;
            while((a_norm + 1) * (a_norm + 1) <= squared_sum) a_norm++;
            ball_finder.set_a(d, i, a_norm);
            long b;
            cin >> b;
            ball_finder.set_b(i, b);
        }
        Solution sol = CGAL::solve_linear_program(ball_finder, ET());
        if(sol.is_infeasible()){
            cout << "none\n";
        } else if (sol.is_unbounded()){
            cout << "inf\n";
        } else {
            long value = floor_to_double(-sol.objective_value());
            cout << value << "\n";
        }
        cin >> n;
    }
}
