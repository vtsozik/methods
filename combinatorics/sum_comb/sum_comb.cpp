/* 
Problem: How many combinations of 20 marbles distributed among 5 buckets are possible.
Solution: Let's fisrt assign unique number (from 1 to 20) to each marble. Then we have 
5 possible combinations to distribute first marble, 6 - second (consider unique ordering 
of numbered marbles in a bucket, we can put first marble before second or second 
before first), 7 - third marble, or (3+5-1) => (i+b-1), where i - marble number, 
b - number of buckets. Distributing m marbles among 5 buckets we can have total of 
(m+b-1)!/(b-1)! possible order combinations of numbered marbles. On another hand we can 
have m! order combinations of our numbered marbles (i,e m*(m-1)*(m-2)*...*(m-(m-1))=m!).
So without uniquenes of order of numbered marbles there are (m+b-1)!/(b-1)!/m! unique
combinations or (m+b-1)!/(b-1)!/m!=(m+b-1)!/((m+b-1)-m)!/m!=C((m+b+1),m).
*/

#include <iostream>
using namespace std;

void comb(int i, int l, int s, int &t, int *a) {
 if(i == l - 1) {
  a[i] = s;
  ++t;
  for(int j = 0; j < l; ++j) {
   cout << a[j] << " "; 
  } 
  cout << endl;
 } else {
  for(int j = 0; j <= s; ++j) {
   a[i] = j;
   comb(i + 1, l, s - j, t, a);
  }
 }
}

double fact(int i) {
 double p = 1.;
 for(int j = 2; j <= i; ++j) {
  p *= j;
 }
 return p;
}

double analytic(int l, int s) {
 int t = l + s - 1;
 return fact(t) / fact(t - s) / fact(s);
}

int main(int argn, char** argv) {
 int i = 0, l = 5, s = 20, t = 0, *a = new int[l];
 comb(i, l, s, t, a);
 cout << "Total: " << t << endl;
 cout << "Total analytic: " << analytic(l, s) << endl;
 delete[] a; 
 return 0;
}
