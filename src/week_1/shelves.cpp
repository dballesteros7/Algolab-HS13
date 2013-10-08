#include <iostream>
#include <math.h>

using namespace std;

int main()
{
   std::ios_base::sync_with_stdio(false);
   int k, i;
   int l, m, n;
   int covered;
   int n_used, m_used;
   int opt_covered, opt_m, opt_n;
   int limit;
   double limit_factor;   
   cin >> k;
   i = 0;
   while (i < k){
       cin >> l;
       cin >> m;
       cin >> n;
       limit_factor = sqrt(l);
       if (n < limit_factor){
           limit = 1;  
       }
       else{
           limit = 0;
       }
       covered = 0;
       n_used = 0;
       m_used = 0;
       n_used = l/n;
       m_used = (l - n*n_used)/m;
       covered = (n_used*n + m_used*m);
       if (covered == l || n == m){
           cout << m_used;
           cout << " ";
           cout << n_used;
           cout << " ";
           cout << (l - covered);
           cout << "\n";
       } else {
           opt_covered = covered;
           opt_m = m_used;
           opt_n = n_used;
           while(n_used > 0){
               if (limit == 1 && m_used > limit_factor) {
                   break;
               }
               n_used--;
               m_used = (l - n*n_used)/m;
               covered = (n_used*n + m_used*m);
               if (covered > opt_covered){
                   opt_covered = covered;
                   opt_m = m_used;
                   opt_n = n_used;
               }
               if (opt_covered == l){
                   break;
               }
           }
           cout << opt_m;
           cout << " ";
           cout << opt_n;
           cout << " ";
           cout << (l - opt_covered);
           cout << "\n";
       }
       i++;
   }
   return 0;
}
