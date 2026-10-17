#include <iostream>
#include <climits>

using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    int test_cases;
    int test_counter = 0;
    int i, j;
    int c, m;
    int max_coin;
    int * currencies, * coins_to_generate;
    int * minima;
    cin >> test_cases;

    while (test_counter < test_cases){
        cin >> c;
        cin >> m;
        
        currencies = new int[c];
        coins_to_generate = new int[m];
        
        i = 0;
        while (i < c){
            int tmp;
            cin >> tmp;
            currencies[i] = tmp;
            i++;
        }
        
        i = 0;
        max_coin = 0;
        while (i < m){
            int tmp;
            cin >> tmp;
            coins_to_generate[i] = tmp;
            if (tmp > max_coin){
                max_coin = tmp;
            }
            i++;
        }
        
        // INT_MAX - 1 marks "not possible", so +1 never overflows
        minima = new int[max_coin + 1];
        minima[0] = 0;
        for(i = 1; i <= max_coin; i++){
            minima[i] = INT_MAX - 1;
            for(j = 0; j < c; j++){
                if(currencies[j] <= i){
                    minima[i] = min(minima[i], 1 + minima[i - currencies[j]]);
                }
            }
        }
        
        for (i = 0; i < m; i++){
            if (minima[coins_to_generate[i]] >= INT_MAX - 1){
                cout << "not possible";
            } else {
                cout << minima[coins_to_generate[i]];
            }
            cout << "\n";
        }
        
        delete [] coins_to_generate;
        delete [] currencies;
        delete [] minima;
        test_counter++;
    }
    
    return 0;
}
