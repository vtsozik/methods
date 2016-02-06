#include <iostream>
using namespace std;

int fib_cfs(int n) {
 double f = (1. + sqrt(5.)) / 2.;
 return ((int) ((pow(f, (double) n) - (-pow(f, (double) -n))) / sqrt(5.)));
}

int fib_lin(int n) {
 int prev = 0, curr = 1 , sum = 0;
 if(n < 2) {
  return n;
 }
 for(int i = 1; i < n; ++i) {
  sum = curr + prev;
  prev = curr; 
  curr = sum; 
 }
 return sum;
}

int fib_rec(int n) {
 if(n > 1) {
  return (fib_rec(n - 1) + fib_rec(n - 2));
 } else {
  return n;
 }
}

int main() {
 int n = 40;
 cout << fib_cfs(n) << endl; // O(1) complexity.
 cout << fib_lin(n) << endl; // O(n) complexity.
 cout << fib_rec(n) << endl; // O(2 ^ (n - 1)) complexity.
 return 0;
}
