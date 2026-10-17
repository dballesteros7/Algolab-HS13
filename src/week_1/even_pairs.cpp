#include <iostream>

using namespace std;

int main()
{
   std::ios_base::sync_with_stdio(false);
   cin.tie(0);
   int n, i, bitvalue;
   // Number of prefixes (including the empty one) with even/odd sum.
   // x_i..x_j is even iff prefixes j and i-1 have the same parity.
   long long even_prefixes, odd_prefixes, pairs_count;
   int parity;

   cin >> n;

   even_prefixes = 1;
   odd_prefixes = 0;
   parity = 0;
   for (i = 0; i < n; i++){
       cin >> bitvalue;
       parity ^= bitvalue & 1;
       if (parity == 0){
           even_prefixes++;
       } else {
           odd_prefixes++;
       }
   }
   pairs_count = even_prefixes*(even_prefixes - 1)/2 + odd_prefixes*(odd_prefixes - 1)/2;

   cout << pairs_count;

   return 0;
}
