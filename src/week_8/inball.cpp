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

typedef CGAL::Quadratic_program<int> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cout.setf(ios::fixed);
    
    int n;
    cin >> n;
    while(n > 0){
        int d;
        cin >> d;
        Program ball_finder(CGAL::SMALLER, false, 0, false, 0);
        ball_finder.set_c(d, -1);
        ball_finder.set_l(d, true, 0);
        for(int i = 0; i < n; ++i){
            double squared_sum = 0;
            for(int j = 0; j < d; ++j){
                int a;
                cin >> a;
                squared_sum += a*a;
                ball_finder.set_a(j, i, a);
            }
            int a_norm = sqrt(squared_sum);
            ball_finder.set_a(d, i, a_norm);
            int b;
            cin >> b;
            ball_finder.set_b(i, b);
        }
        Solution sol = CGAL::solve_quadratic_program(ball_finder, ET());
        if(sol.is_infeasible()){
            cout << "none\n";
        } else if (sol.is_unbounded()){
            cout << "inf\n";
        } else {
            in value = floor(-CGAL::to_double(sol.objective_value()));
            cout << value << "\n";
        }
        cin >> n;
    }
}
