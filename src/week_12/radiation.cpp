#include <iostream>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <cmath>

#ifdef CGAL_USE_GMP
#include <CGAL/Gmpz.h>
typedef CGAL::Gmpz IT;
typedef CGAL::Gmpz ET;
#else
#include <CGAL/MP_Float.h>
typedef CGAL::MP_Float IT;
typedef CGAL::MP_Float ET;
#endif

typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

using namespace std;

typedef pair<int, pair<int, int> > Polytuple;

int main(){
    ios_base::sync_with_stdio(false);
    int n;
    cin >> n;
    for(int i = 0; i < n; ++i){
        int h, t;
        cin >> h >> t;
        vector<Polytuple> cells(h + t);
        for(int j = 0; j < h + t; ++j){
            int x,y,z;
            cin >> x >> y >> z;
            cells[j] = Polytuple(x, pair<int,int>(y,z));
        }
        map<Polytuple, vector<IT> > polyterms;
        bool done = false;
        Program radiator(CGAL::LARGER, false, 0, false, 0);
        int coeff_index_1 = 0;
        for(int j = 0; j <= 9; ++j){
            for(int k = 0; k < h + t; ++k){
                for(int k1 = 0; k1 <= j; ++k1){
                    for(int k2 = 0; k2 <= j - k1; ++k2){
                        int k3 = j - k1 - k2;
                        IT value;
                        Polytuple current_tuple(k1, pair<int, int>(k2, k3));
                        if(k1 == 0 && k2 == 0 && k3 == 0){
                            value = 1;
                        }
                        else if(k3 >= k2 && k3 >= k1){
                            Polytuple search_tuple(k1, pair<int, int>(k2, k3 - 1));
                            value = polyterms[search_tuple][k]*cells[k].second.second;
                        } else if(k2 >= k1 && k2 >= k3){
                            Polytuple search_tuple(k1, pair<int, int>(k2 - 1, k3));
                            value = polyterms[search_tuple][k]*cells[k].second.first;
                        } else {
                            Polytuple search_tuple(k1 - 1, pair<int, int>(k2, k3));
                            value = polyterms[search_tuple][k]*cells[k].first;
                        }
                        if(k == 0){
                            polyterms.insert(pair<Polytuple, vector<IT> >(current_tuple, vector<IT>(h + t)));
                        }
                        polyterms[current_tuple][k] = value;
                    }
                }
            }
        }
        for(int j = 0; j <= 9; ++j){
            for(int k = 0; k < h + t; ++k){
                int coeff_index_2 = 0;
                for(int k1 = 0; k1 <= j; ++k1){
                    for(int k2 = 0; k2 <= j - k1; ++k2){
                        int k3 = j - k1 - k2;
                        Polytuple current_tuple(k1, pair<int, int>(k2, k3));
                        radiator.set_a(coeff_index_1 + coeff_index_2, k, polyterms[current_tuple][k]);
                        coeff_index_2++;
                    }
                }
                if(k >= h){
                    radiator.set_r(k, CGAL::SMALLER);
                    radiator.set_b(k, -1);
                } else {
                    radiator.set_b(k, 1);
                }
                if(k == (h + t - 1)){
                    coeff_index_1 += coeff_index_2;
                }
            }
            CGAL::Quadratic_program_options options;
            options.set_pricing_strategy(CGAL::QP_BLAND);
            Solution sol = CGAL::solve_linear_program(radiator, ET(), options);
            if(!sol.is_infeasible()){
                done = true;
                cout << j << "\n";
                break;
            }
        }
        if(done){
            continue;
        }
        for(int j = 10; j <= 30; ++j){
            for(int k = 0; k < h + t; ++k){
                for(int k1 = 0; k1 <= j; ++k1){
                    for(int k2 = 0; k2 <= j - k1; ++k2){
                        int k3 = j - k1 - k2;
                        IT value;
                        Polytuple current_tuple(k1, pair<int, int>(k2, k3));
                        if(k1 == 0 && k2 == 0 && k3 == 0){
                            value = 1;
                        }
                        else if(k3 >= k2 && k3 >= k1){
                            Polytuple search_tuple(k1, pair<int, int>(k2, k3 - 1));
                            value = polyterms[search_tuple][k]*cells[k].second.second;
                        } else if(k2 >= k1 && k2 >= k3){
                            Polytuple search_tuple(k1, pair<int, int>(k2 - 1, k3));
                            value = polyterms[search_tuple][k]*cells[k].second.first;
                        } else {
                            Polytuple search_tuple(k1 - 1, pair<int, int>(k2, k3));
                            value = polyterms[search_tuple][k]*cells[k].first;
                        }
                        if(k == 0){
                            polyterms.insert(pair<Polytuple, vector<IT> >(current_tuple, vector<IT>(h + t)));
                        }
                        polyterms[current_tuple][k] = value;
                    }
                }
            }
        }
        int lower_limit = 10;
        int upper_limit = 30;
        int j = 21;
        int solution = -1;
        while(!done){
            radiator = Program(CGAL::LARGER, false, 0, false, 0);
            for(int k = 0; k < h + t; ++k){
                int coeff_index = 0;
                for(int k1 = 0; k1 <= j; ++k1){
                    for(int k2 = 0; k2 <= j - k1; ++k2){
                        for(int k3 = 0; k3 <= j - k1 - k2; ++k3){
                            Polytuple current_tuple(k1, pair<int, int>(k2, k3));
                            radiator.set_a(coeff_index, k, polyterms[current_tuple][k]);
                            coeff_index++;
                        }
                    }
                }
                if(k >= h){
                    radiator.set_r(k, CGAL::SMALLER);
                    radiator.set_b(k, -1);
                } else {
                    radiator.set_b(k, 1);
                }
            }
            CGAL::Quadratic_program_options options;
            options.set_pricing_strategy(CGAL::QP_BLAND);
            Solution sol = CGAL::solve_linear_program(radiator, ET(), options);
            if(!sol.is_infeasible()){
                solution = j;
                upper_limit = j - 1;
            } else {
                lower_limit = j + 1;
            }
            j = (upper_limit + lower_limit)/2;
            if(lower_limit > upper_limit)
                done = true;
        }
        if(solution == -1){
            cout << "Impossible!\n";
        } else {
            cout << solution << "\n";
        }
    }
}
