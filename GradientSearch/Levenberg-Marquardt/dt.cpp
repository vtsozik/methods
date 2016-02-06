#include <iostream>
using namespace std;

inline double c(double x, double y) {
 return (x * x + y * y + 2);
}

inline double f(double x, double y) {
 return ((x - y) / c(x, y));
}

inline double dfx(double x, double y) {
 return ((-(x * x) + 2 * x * y + y * y + 2) / c(x, y) / c(x, y));
}

inline double dfy(double x, double y) {
 return ((-(x * x) - 2 * x * y + y * y - 2) / c(x, y) / c(x, y));
}

inline double dfx(double x, double y, double h) {
 return ((f(x + h, y) - f(x - h, y)) / 2 / h);
}

inline double dfy(double x, double y, double h) {
 return ((f(x, y + h) - f(x, y - h)) / 2 / h);
}

int main() {
 double p = 1e-9 , x = -3, y = -2, h, ph;
 h = 1;
 do {
  ph = h;
  h /= 10.;
 } while(fabs(dfx(x, y, h) - dfx(x, y, ph)) > p);
 cerr << "hx: " << h << " dfx " << dfx(x, y) << " dfxh " << dfx(x, y, h) << endl;
 h = 1;
 do {
  ph = h;
  h /= 10.;
 } while(fabs(dfy(x, y, h) - dfy(x, y, ph)) > p);
 cerr << "hy: " << h << " dfy " << dfy(x, y) << " dfyh " << dfy(x, y, h) << endl;
 return 0;
}
