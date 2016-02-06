/* Trader has 0.6 probability to win 50% or (1 - 0.6) to loose 50%.
 * Out of 260 trading days to break even  he needs (1 + 0.5)^n * (1 - 0.5)^(260 - n) = 1.0, =>
 * n = 164 days. What is probability that he would have n > 164 successful days in 260 day year.
 * p = sum(from i = 165, to n = 260) of (combinations(i,n) * (0.6^i) * (1 - 0.6)^(n - i)) 
*/

#include <iostream>
#include <math.h>
using namespace std;

long double fact(long double ld) {
 if(ld < 1.) {
  return 1.; 
 } 
 long double f = ld; 
 while(ld > 2.) {
  ld -= 1.;
  f *= ld;
 }
 return f; 
}

int main() {
 long double td = 260., sp = .6, sum = 0.;
 for(long double sd = 165; sd <= td; sd += 1.) {
  sum += (fact(td) / fact(td - sd) / fact(sd) * pow(sp, sd) * pow(1 - sp, td - sd));
 }
 cout << sum << endl; 
 return 0;
}
