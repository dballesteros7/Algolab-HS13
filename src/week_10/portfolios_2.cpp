#include <iostream>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <cmath>

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
    int n,m;
    cin >> n >> m;
    while(n != 0 && m != 0){
        Program no_risk_portfolio(CGAL::SMALLER, true, 0, false, 0);
        Program portfolio(CGAL::SMALLER, true, 0, false, 0);
        portfolio.set_r(1, CGAL::LARGER);
        for(int i = 0; i < n; ++i){
            int c,r;
            cin >> c >> r;
            portfolio.set_a(i, 0 , c);
            portfolio.set_a(i, 1, r);
            no_risk_portfolio.set_c(i, -r);
            no_risk_portfolio.set_a(i, 0, c);
        }

        for(int i = 0; i < n; ++i){
            for(int j = 0; j < n; ++j){
                int cv;
                cin >> cv;
                if(j <= i){
                    portfolio.set_d(i, j, 2*cv);
                }
            }
        }
        for(int i = 0; i < m; ++i){
            int C, V;
            cin >> C >> V;
            no_risk_portfolio.set_b(0, C);
            Solution no_risk_sol = CGAL::solve_quadratic_program(no_risk_portfolio, ET());
            CGAL::Quotient<ET> no_risk_sol_exact = -no_risk_sol.objective_value();
            int no_risk_sol_approx = -floor(CGAL::to_double(no_risk_sol_exact));
            while(no_risk_sol_approx > no_risk_sol_exact) no_risk_sol_approx--;
            while(no_risk_sol_approx + 1 <= no_risk_sol_exact) no_risk_sol_approx++;
            if(no_risk_sol_approx == 0){
                cout << "0";
                continue;
            }
            int low_value = 0;
            int high_value = no_risk_sol_approx;
            int mid_value;
            if(high_value % 2 == 0){
                mid_value = high_value/2;
            } else {
                mid_value = (high_value + 1)/2;
            }
            do{
                portfolio.set_b(0, C);
                portfolio.set_b(1, mid_value);
                Solution sol = CGAL::solve_quadratic_program(portfolio, ET());
                if(sol.objective_value() <= V){
                    low_value = mid_value;
                } else {
                    high_value = mid_value - 1;
                }
                if((high_value + low_value) % 2 == 0){
                    mid_value = (high_value + low_value)/2;
                } else {
                    mid_value = (high_value + low_value + 1)/2;
                }
            } while(low_value != high_value);
            cout << low_value << "\n";
        }
        cin >> n >> m;
    }
}
