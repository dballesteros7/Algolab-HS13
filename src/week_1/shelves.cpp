#include <iostream>
#include <algorithm>

using namespace std;

int main()
{
   std::ios_base::sync_with_stdio(false);
   cin.tie(0);
   int k, i;
   int l, m, n;
   int left;
   int n_used, n_min;
   int opt_left, opt_m, opt_n;
   cin >> k;
   i = 0;
   while (i < k){
       cin >> l;
       cin >> m;
       cin >> n;
       // The leftover (l - n*n_used) % m is periodic in n_used with period m,
       // so the m largest values of n_used already reach every possible
       // leftover, each with the most large shelves. This is
       // min(m, l/n + 1) <= sqrt(l) + 1 iterations since m < n.
       n_used = l/n;
       n_min = max(0, n_used - m + 1);
       opt_left = m;
       opt_m = 0;
       opt_n = 0;
       for (; n_used >= n_min; n_used--){
           left = (l - n*n_used) % m;
           if (left < opt_left){
               opt_left = left;
               opt_n = n_used;
               opt_m = (l - n*n_used)/m;
               if (opt_left == 0){
                   break;
               }
           }
       }
       cout << opt_m;
       cout << " ";
       cout << opt_n;
       cout << " ";
       cout << opt_left;
       cout << "\n";
       i++;
   }
   return 0;
}
