#include <iostream>

using namespace std;

int main()
{
   int n, i, bitvalue;
   int pairs_count, sum_odd_ones, sum_even_ones, zeros, ones;
   
   cin >> n;
   
   i = 0;
   
   pairs_count = 0;
   sum_odd_ones = 0;
   sum_even_ones = 0;
   zeros = 0;
   ones = 0;
   while (i < n){
       cin >> bitvalue;
       if (bitvalue == 0){
           zeros++;
           pairs_count++;
           if (ones >= 2){
               if (ones % 2 == 0){
                   pairs_count += ones/2 + sum_odd_ones;
               } else {
                   pairs_count += (ones - 1)/2 + sum_even_ones;
               }
           }
       } else {
           pairs_count += (zeros)*(zeros-1)/2;
           ones++;
           if (ones % 2 == 0){
               sum_even_ones += zeros;
               pairs_count += (sum_odd_ones) + ones/2;
           } else {
               sum_odd_ones += zeros;
               pairs_count += (sum_even_ones) + (ones - 1)/2;
           }
           zeros = 0;
       }
       i++;
   }
   pairs_count += (zeros-1)*(zeros)/2;
   
   cout << pairs_count;
   
   return 0;
}
