#include <iostream>
using namespace std;
#include <math.h>

#define Y(X) 4. * sqrt(1. - X * X)

double intgrt(double a, double b) {
 double dx = b - a, s = (Y(b) - Y(a)) * dx / 2., ps, pr = 1e-6;
 do {
  ps = s;
  s /= 2.;
  dx /= 2.;
  for(double st = a + dx; st < b; st += (2 * dx)) {
   s += (Y(st) * dx);
  }
 } while(fabs(s - ps) > pr);
cerr << "dx = " << dx << endl;
 return s;
}

int main() {
 cout << "sum = " << intgrt(0., 1.) << endl;
 return 0;
}

/* Output
dx = 2.38419e-07
sum = 3.14159
*/
