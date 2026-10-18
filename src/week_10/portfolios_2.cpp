#include <iostream>
#include <vector>
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

typedef CGAL::Quadratic_program<int> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;
typedef CGAL::Quotient<ET> Exact;

using namespace std;

// Largest integer not above value.
int floor_value(const Exact &value){
    int approx = floor(CGAL::to_double(value));
    while(approx > value) --approx;
    while(approx + 1 <= value) ++approx;
    return approx;
}

int main(){
    ios_base::sync_with_stdio(false);
    int n,m;
    cin >> n >> m;
    while(n != 0 && m != 0){
        // Minimise the variance subject to cost <= C (row 0) and return >= R (row 1).
        Program portfolio(CGAL::SMALLER, true, 0, false, 0);
        portfolio.set_r(1, CGAL::LARGER);
        vector<int> costs(n), returns(n);
        for(int i = 0; i < n; ++i){
            cin >> costs[i] >> returns[i];
            portfolio.set_a(i, 0, costs[i]);
            portfolio.set_a(i, 1, returns[i]);
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
            portfolio.set_b(0, C);
            // Without the variance limit the best return is to spend everything on
            // the asset with the highest return per cost.
            long long best_return = 0;
            for(int j = 0; j < n; ++j){
                best_return = max(best_return, (long long)C*returns[j]/costs[j]);
            }
            // The minimal variance v(R) for a return of at least R is convex and
            // nondecreasing in R; find the largest integer R with v(R) <= V.
            // lo/hi are evaluated returns with v(lo) <= V < v(hi).
            int lower = 0, upper = best_return;
            int lo = 0, hi = upper + 1;
            Exact lo_variance = 0, hi_variance = 0, hi_slope = 0;
            int R = upper;
            bool newton = true;
            while(lower < upper){
                int previous_gap = upper - lower;
                portfolio.set_b(1, R);
                Solution sol = CGAL::solve_nonnegative_quadratic_program(portfolio, ET());
                if(sol.objective_value() <= V){
                    lo = R;
                    lo_variance = sol.objective_value();
                    lower = R;
                } else {
                    hi = R;
                    hi_variance = sol.objective_value();
                    // The return constraint's multiplier is a subgradient of v at hi.
                    hi_slope = -*(sol.optimality_certificate_begin() + 1);
                    upper = min(upper, hi - 1);
                    // The tangent at hi bounds v from below...
                    if(hi_slope > 0){
                        upper = min(upper, floor_value(Exact(hi) - (hi_variance - V)/hi_slope));
                    }
                }
                if(hi <= best_return){
                    // ...and the chord from lo to hi bounds it from above.
                    lower = max(lower, floor_value(Exact(lo) + (Exact(V) - lo_variance)*(hi - lo)/(hi_variance - lo_variance)));
                }
                // Step to the largest return that is still possible (Newton from
                // above), but bisect when the bounds are far apart or the last
                // Newton step did not halve the gap.
                int gap = upper - lower;
                bool bisect = 2*gap > upper - lo || (newton && 2*gap > previous_gap);
                newton = !bisect;
                R = bisect ? lower + (gap + 1)/2 : upper;
            }
            cout << lower << "\n";
        }
        cin >> n >> m;
    }
}
