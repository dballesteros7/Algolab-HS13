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
    int n, m;
    cin >> n >> m;
    while(n != 0 && m != 0){
        Program portfolio(CGAL::SMALLER, true, 0, false, 0);
        portfolio.set_r(1, CGAL::LARGER);
        for(int i = 0; i < n; ++i){
            int c,r;
            cin >> c >> r;
            portfolio.set_a(i, 0, c);
            portfolio.set_a(i, 1, r);
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
            int C, R, V;
            cin >> C >> R >> V;
            portfolio.set_b(0, C);
            portfolio.set_b(1, R);
            Solution sol = CGAL::solve_quadratic_program(portfolio, ET());
            if(sol.is_infeasible()){
                cout << "No.\n";
            } else {
                if(sol.objective_value() <= V){
                    cout << "Yes.\n";
                } else {
                    cout << "No.\n";
                }
            }

        }
        cin >> n >> m;
    }
}
