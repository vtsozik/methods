#include<iostream>
using namespace std;

void fixdiv(unsigned *n, 
 unsigned short *q, unsigned short *r) {
 static const unsigned max = 1 << 16;
 *q = *n >> 16;
 *r = *n - *q * max;
}

int main() {
 static const unsigned word = 1 << 16;
 unsigned i, n = -1;
 unsigned short q1, r1, q2, r2;
 for(i = 0; i <= n; ++i) {
  fixdiv(&i, &q1, &r1);
  q2 = i / word;   
  r2 = i % word;
  if(q1 != q2 || r1 != r2) {
   break;
  }   
 }
 cout << "i = " << i << " q1 = " << q1 << " r1 = " << r1 << endl;
 cout << "i = " << i << " q2 = " << q2 << " r2 = " << r2 << endl;
 return 0;
}
