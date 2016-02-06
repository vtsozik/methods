#include <iostream>
using namespace std;

void BoxMuller(double &x, double &y) {
 double w;
 do {
  x = (double) 2.0 * rand() / RAND_MAX - 1.0;
  y = (double) 2.0 * rand() / RAND_MAX - 1.0;
  w = x * x + y * y;
 } while (w >= 1.0);
 w = sqrt(-2.0 * log(w) / w);
 x *= w;
 y *= w;
}

void simulate(double &u, double &d, double &su, double &sd) {
 int i = 0;
 double sum = 0., s[2], a = 32., b = -10.;
 do {
  if(!(i % 2)) {
   BoxMuller(s[0], s[1]); 
  }
  sum += s[i % 2]; 
  ++i;
 } while(a > sum && sum > b);
 if(sum <= b) {
  d += 1.;
  sd += (double) i;
 } else if(sum >= a) {
  u += 1.;
  su += (double) i;
 } else {
  cerr << "POSSIBLE CODING ERROR!" << endl;
  exit(1);
 }
}

int main() {
 srand(time(NULL));
 int i, n = 1000000;
 double u = 0., d = 0., su = 0., sd = 0., t, a, b;
 for(i = 0; i < n; ++i) {
  simulate(u, d, su, sd);
 }
 t = u + d; 
 cout << "\ru = " << u / t << " d = " << d / t << " su = " << su / u << " sd = " << sd / d << " st = " << (su + sd) / t << endl;
 return 0;
}
