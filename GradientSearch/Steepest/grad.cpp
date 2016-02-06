#include <iostream>
#include <math.h>
using namespace std;

inline double c(double x, double y) {
 return (x * x + y * y + 2);
}

inline double f(double x, double y) {
 return ((x - y) / c(x, y));
}

inline double f(double x, double y, double gx, double gy, double t) {
 x += (t * gx); 
 y += (t * gy); 
 return f(x, y);
}

inline double dfx(double x, double y) {
 return ((-(x * x) + 2 * x * y + y * y + 2) / c(x, y) / c(x, y));
}

inline double dfy(double x, double y) {
 return ((-(x * x) - 2 * x * y + y * y - 2) / c(x, y) / c(x, y));
}

inline double dft(double nx, double ny, double gx, double gy) {
 return (((gx + gy) * (ny * ny - nx * nx) + 2 * (gx - gy) * (1 + nx * ny)) / c(nx, ny) / c(nx, ny));
}

inline double dft(double x, double y, double gx, double gy, double t) {
 x += (t * gx); 
 y += (t * gy); 
 return dft(x, y, gx, gy);
}

#if 0
inline double ddft(double nx, double ny, double gx, double gy) {
 return (4 * ((ny * gx * gx - nx * gy * gy) / c(nx, ny) / c(nx, ny) - (nx * gx + ny * gy) * dft(nx, ny, gx, gy) / c(nx, ny)));
}
#endif

double zbrent(double x, double y, double gx, double gy, double ll, double ul, double precision) {
 double a = (ll + ul) / 2, na, yll, ya = dft(x, y, gx, gy, a), yul, r, s, t, p, q;
 while(fabs(ya) > precision) {
  yll = dft(x, y, gx, gy, ll);
  yul = dft(x, y, gx, gy, ul);
  r = ya / yul;
  s = ya / yll;
  t = yll / yul;
//cout << yll << " " << ya << " " << yul << endl;
  p = s * (t * (r - t) * (ul - a) - (1 - r) * (a - ll));
  q = (t - 1) * (r - 1) * (s - 1);
  na = a + p / q;
  if(na > a) {
   ll = a;
  } else if(na < a) {
   ul = a;
  } else {
//cout << ll << " / " << na << " / " << ul << " / " << ya << endl;
   return na;
  }
  a = na;
  ya = dft(x, y, gx, gy, a);
//cout << ll << " - " << a << " - " << ul << " - " << ya << endl;
 }
 return a;
}

double min(double x, double y, double gx, double gy, double p) {
 double ll = 1, ul, h = 2, m = 1, df, dfh; 
 df = dft(x, y, gx, gy, ll);
 if(df > 0) {
  m = -1;
 };
 dfh = df;
 while((df * dfh) > 0) {
  ul = ll + h * m;
  dfh = dft(x, y, gx, gy, ul);
  h *= h; 
 } 
cout << "ll " << ll << " ul " << ul << " df " << df << " dfh " << dfh << endl;
 if(df < 0) {
  return zbrent(x, y, gx, gy, ll, ul, p);  
 } else {
  return zbrent(x, y, gx, gy, ul, ll, p);  
 }
}

int main() {
 double p = 1e-9, x, y, nx = -3, ny = -2, g, gx, gy, t;
int i = 0;
 do {
  x = nx;
  y = ny;
  gx = dfx(x, y); 
  gy = dfy(x, y); 
  g = -sqrt(gx * gx + gy * gy);
  gx /= g; 
  gy /= g; 
  t = min(x, y, gx, gy, p);
  nx = x + t * gx;
  ny = y + t * gy;
cout << "x " << nx << " y " << ny << " t " << t << " gx " << gx << " gy " << gy << " g " << g << endl;
 } while(fabs(nx - x) > p && fabs(ny - y) > p && i++ < 100);
 return 0;
}
