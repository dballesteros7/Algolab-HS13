#include <iostream>

using namespace std;

int main()
{
    std::ios_base::sync_with_stdio(false);
    cin.tie(0);
    int t, m;
    int i, j;
    double current, sum;
    cin >> t;
    for (i = 0; i < t; i++){
        cin >> m;
        sum = 0;
        for (j = 0; j < m; j++){
            cin >> current;
            sum += current;
        }
        cout << sum << "\n";
    }
    return 0;
}
