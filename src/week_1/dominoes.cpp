#include <iostream>

using namespace std;

int main()
{
   std::ios_base::sync_with_stdio(false);
   int n, m;
   int i, j;
   int current_h;
   int current_toppled;
   i = 0;
   cin >> n;
   while(i < n){
       cin >> m;
       j = 0;
       current_toppled = 0;
       current_h = 0;
       while (j < m){
           cin >> current_h;
           if (j <= current_toppled){
               if ((current_h + j - 1) > current_toppled){
                current_toppled = current_h + j - 1;
               }
           }
           j++;
       }
       if (current_toppled >= m){
           cout << m;
           cout << "\n";
       }
       else{
           cout << current_toppled + 1;
           cout << "\n";
       }
       i++;
   }
   return 0;
}
