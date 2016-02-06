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

void simulate(double &b, double &sb) {
 int i = 0;
 double sum = 0., s[2];
 do {
  if(!(i % 2)) {
   BoxMuller(s[0], s[1]); 
  }
  sum += s[i % 2]; 
  ++i;
 } while(-b < sum && sum < b);
 sb += (double) i;
}

int main() {
 srand(time(NULL));
 double i, m = 1e6, j, n = 4., sb;
 for(j = 1.; j <= n; j+=1.) {
  sb = 0.;
  for(i = 1.; i <= m; i+=1.) {
   simulate(j, sb);
  }
  cout << "b = " << j << " sb = " << sb / m << endl;
 }
 return 0;
}
