#include <iostream>
#include <vector>
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

struct Investor {
    long C, R;
    bool feasible;
    CGAL::Quotient<ET> variance;
};

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
        // For R > 0, substituting x = R y gives: minimum variance = R^2 f(C/R) with
        // f(t) = min{ y'Sy : r'y >= 1, c'y <= t, y >= 0 }, which is non-increasing in t.
        // Every solved investor fixes f at one point, which often decides later investors without a QP.
        vector<Investor> solved;
        for(int i = 0; i < m; ++i){
            long C, R, V;
            cin >> C >> R >> V;
            int answer = -1;
            if(R > 0){
                for(size_t k = 0; k < solved.size() && answer < 0; ++k){
                    const Investor &s = solved[k];
                    // Compare C'/R' with C/R and f(C'/R') = variance'/R'^2 with V/R^2.
                    long cost_order = s.C * R - C * s.R;
                    bool low_variance = s.feasible && s.variance * ET(R * R) <= ET(V) * ET(s.R * s.R);
                    if(cost_order <= 0 && low_variance){
                        answer = 1;
                    } else if(cost_order >= 0 && !low_variance){
                        answer = 0;
                    }
                }
            }
            if(answer < 0){
                portfolio.set_b(0, C);
                portfolio.set_b(1, R);
                // All amounts are >= 0, which the nonnegative solver exploits.
                Solution sol = CGAL::solve_nonnegative_quadratic_program(portfolio, ET());
                Investor investor = {C, R, !sol.is_infeasible(), 0};
                if(investor.feasible){
                    investor.variance = sol.objective_value();
                }
                answer = investor.feasible && investor.variance <= ET(V);
                if(R > 0){
                    solved.push_back(investor);
                }
            }
            if(answer == 1){
                cout << "Yes.\n";
            } else {
                cout << "No.\n";
            }
        }
        cin >> n >> m;
    }
}
