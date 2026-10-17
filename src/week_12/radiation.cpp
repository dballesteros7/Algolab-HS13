#include <iostream>
#include <vector>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>

#ifdef CGAL_USE_GMP
#include <CGAL/Gmpz.h>
typedef CGAL::Gmpz IT;
typedef CGAL::Gmpz ET;
#else
#include <CGAL/MP_Float.h>
typedef CGAL::MP_Float IT;
typedef CGAL::MP_Float ET;
#endif

using namespace std;

// The constraint matrix is dense, so the LP is handed to CGAL columnwise
// through iterators instead of entry by entry.
typedef vector<const IT*>::const_iterator A_it;
typedef vector<IT>::const_iterator B_it;
typedef vector<CGAL::Comparison_result>::const_iterator R_it;
typedef CGAL::Const_oneset_iterator<bool> F_it;
typedef CGAL::Const_oneset_iterator<IT> V_it;
typedef CGAL::Linear_program_from_iterators<A_it, B_it, R_it, F_it, V_it, F_it, V_it, V_it> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

const int MAX_DEGREE = 30;
const unsigned long long P = 2147483647ULL; // prime 2^31 - 1

// One column per monomial x^k1 y^k2 z^k3, ordered by total degree, so the
// polynomials of degree at most d use exactly the first (d+1)(d+2)(d+3)/6.
// The coefficients of a polynomial are only seen through A*c, so the LP only
// depends on the column space of A: it is enough to give the LP a set of
// columns spanning it, of which there are at most h + t. Such a set is chosen
// greedily by Gaussian elimination modulo P; columns that are independent
// modulo P are independent over the rationals, and an infeasibility verdict
// for the reduced LP is double checked exactly against all columns.
struct Monomials {
    int cells;
    vector< vector< vector<IT> > > powers;                   // [cell][coordinate][exponent]
    vector< vector< vector<unsigned long long> > > powers_p; // the same modulo P
    vector< vector<IT> > columns;
    vector<int> selected;
    vector< vector<unsigned long long> > echelon;           // reduced selected columns modulo P
    vector<int> echelon_pivot;
    int degree;

    Monomials(const vector< vector<int> > &coordinates) : cells(coordinates.size()), degree(-1){
        powers.assign(cells, vector< vector<IT> >(3, vector<IT>(MAX_DEGREE + 1)));
        powers_p.assign(cells, vector< vector<unsigned long long> >(3, vector<unsigned long long>(MAX_DEGREE + 1)));
        for(int k = 0; k < cells; ++k){
            for(int c = 0; c < 3; ++c){
                int coordinate = coordinates[k][c];
                unsigned long long coordinate_p = ((coordinate % (long long)P) + P) % P;
                powers[k][c][0] = 1;
                powers_p[k][c][0] = 1;
                for(int e = 1; e <= MAX_DEGREE; ++e){
                    powers[k][c][e] = powers[k][c][e - 1]*coordinate;
                    powers_p[k][c][e] = powers_p[k][c][e - 1]*coordinate_p % P;
                }
            }
        }
    }

    // Adds the monomials of the next total degree
    void add_degree(){
        ++degree;
        for(int k1 = 0; k1 <= degree; ++k1){
            for(int k2 = 0; k2 <= degree - k1; ++k2){
                int k3 = degree - k1 - k2;
                columns.push_back(vector<IT>(cells));
                vector<IT> &column = columns.back();
                for(int k = 0; k < cells; ++k){
                    column[k] = powers[k][0][k1]*powers[k][1][k2]*powers[k][2][k3];
                }
                if((int)selected.size() == cells){
                    continue; // the selected columns already span everything
                }
                vector<unsigned long long> v(cells);
                for(int k = 0; k < cells; ++k){
                    v[k] = powers_p[k][0][k1]*powers_p[k][1][k2] % P*powers_p[k][2][k3] % P;
                }
                for(size_t e = 0; e < echelon.size(); ++e){
                    unsigned long long factor = v[echelon_pivot[e]];
                    if(factor != 0){
                        const vector<unsigned long long> &row = echelon[e];
                        for(int k = 0; k < cells; ++k){
                            v[k] = (v[k] + (P - factor)*row[k]) % P;
                        }
                    }
                }
                int pivot = 0;
                while(pivot < cells && v[pivot] == 0){
                    ++pivot;
                }
                if(pivot == cells){
                    continue;
                }
                // Normalise the pivot to 1 (inverse by Fermat)
                unsigned long long inverse = 1, base = v[pivot];
                for(unsigned long long e = P - 2; e > 0; e >>= 1){
                    if(e & 1){
                        inverse = inverse*base % P;
                    }
                    base = base*base % P;
                }
                for(int k = 0; k < cells; ++k){
                    v[k] = v[k]*inverse % P;
                }
                echelon.push_back(v);
                echelon_pivot.push_back(pivot);
                selected.push_back(columns.size() - 1);
            }
        }
    }
};

// Solves the LP with the given columns of A
Solution solve(const vector<const IT*> &column_starts, const vector<IT> &b, const vector<CGAL::Comparison_result> &r){
    Program radiator(column_starts.size(), b.size(), column_starts.begin(), b.begin(), r.begin(),
                     F_it(false), V_it(0), F_it(false), V_it(0), V_it(0));
    // Bland's rule cannot cycle (the other pricing strategies do on these
    // degenerate feasibility problems)
    CGAL::Quadratic_program_options options;
    options.set_pricing_strategy(CGAL::QP_BLAND);
    return CGAL::solve_linear_program(radiator, ET(), options);
}

// Is there a polynomial of degree <= degree that is >= b on the healthy
// cells and <= b (b negative) on the tumor cells?
bool separable(int degree, const Monomials &monomials, const vector<IT> &b, const vector<CGAL::Comparison_result> &r){
    int variables = (degree + 1)*(degree + 2)*(degree + 3)/6;
    vector<const IT*> column_starts;
    vector<bool> is_selected(variables, false);
    for(size_t j = 0; j < monomials.selected.size() && monomials.selected[j] < variables; ++j){
        column_starts.push_back(&monomials.columns[monomials.selected[j]][0]);
        is_selected[monomials.selected[j]] = true;
    }
    Solution sol = solve(column_starts, b, r);
    if(!sol.is_infeasible()){
        return true;
    }
    if((int)column_starts.size() == monomials.cells){
        return false; // the selected columns span the whole space
    }
    // The certificate lambda (lambda^T A_j = 0 for the free variables, sign
    // conditions and lambda^T b only involve b and r) proves the full LP
    // infeasible as well if lambda^T A_j = 0 also holds for the other columns.
    vector<ET> lambda(sol.infeasibility_certificate_begin(), sol.infeasibility_certificate_end());
    bool certified = true;
    for(int j = 0; j < variables && certified; ++j){
        if(is_selected[j]){
            continue;
        }
        ET product = 0;
        for(int k = 0; k < monomials.cells; ++k){
            product += lambda[k]*monomials.columns[j][k];
        }
        certified = (product == 0);
    }
    if(certified){
        return false;
    }
    // Modular rank was too small (practically never happens): use all columns
    column_starts.clear();
    for(int j = 0; j < variables; ++j){
        column_starts.push_back(&monomials.columns[j][0]);
    }
    return !solve(column_starts, b, r).is_infeasible();
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    int n;
    cin >> n;
    for(int i = 0; i < n; ++i){
        int h, t;
        cin >> h >> t;
        int cells = h + t;
        vector< vector<int> > coordinates(cells, vector<int>(3));
        for(int k = 0; k < cells; ++k){
            cin >> coordinates[k][0] >> coordinates[k][1] >> coordinates[k][2];
        }
        // The constraints are homogeneous in the coefficients, so any positive
        // margins give the same answer as P >= 1 / P <= -1. Distinct margins
        // avoid most degenerate pivots, which makes the simplex much faster.
        vector<IT> b(cells);
        vector<CGAL::Comparison_result> r(cells);
        for(int k = 0; k < cells; ++k){
            int margin = 1000 + (k*7919) % 997;
            if(k < h){
                b[k] = margin;
                r[k] = CGAL::LARGER;
            } else {
                b[k] = -margin;
                r[k] = CGAL::SMALLER;
            }
        }
        Monomials monomials(coordinates);
        // Small degrees are cheap to test one by one, larger ones are found by
        // binary search (separability is monotone in the degree)
        int solution = -1;
        for(int j = 0; j <= 9 && solution == -1; ++j){
            monomials.add_degree();
            if(separable(j, monomials, b, r)){
                solution = j;
            }
        }
        if(solution == -1){
            int lower_limit = 10;
            int upper_limit = MAX_DEGREE;
            while(lower_limit <= upper_limit){
                int j = (lower_limit + upper_limit)/2;
                while(monomials.degree < j){
                    monomials.add_degree();
                }
                if(separable(j, monomials, b, r)){
                    solution = j;
                    upper_limit = j - 1;
                } else {
                    lower_limit = j + 1;
                }
            }
        }
        if(solution == -1){
            cout << "Impossible!\n";
        } else {
            cout << solution << "\n";
        }
    }
}
