#include "normal.h"
#include "Legendre.hpp"
#include <iostream>
using namespace std;

double ro(double t, double a, double b) {
 return (exp(-(b * b / t / 2.)) * fabs(b) / sqrt(2. * M_PI * t) / t * 2. * (1. - stdnormal_cdf(fabs(a) / sqrt(t))));
 //return 4. * sqrt(1. - t * t);
}

double integrate(double a, double b) {
 double d = 1e-3, t = d, f, pf = 0., sum = 0.; 
 do {
  f = ro(t, a, b);
  sum += ((f + pf) * d / 2.);
  pf = f;
  t += d;
//cerr << "sum = " << sum << " f = " << f << " t = " << t << endl;
 } while(t < 1e4); 
//cerr << "f = " << f << endl;
 return sum;
}

class Integral1 {
public:
 double b;
 double operator()(const double &t) const {
  return (exp(b * b / t / -2.) * fabs(b) / sqrt(2. * M_PI * t) / t);
  //return (4. * sqrt(1. - t * t));
 }
};

class Integral2 {
public:
 double a, b;
 double operator()(const double &t) const {
  Integral1 in;
  in.b = a;
  Legendre<Integral1> leg(in);
  return (exp(b * b / t / -2.) * fabs(b) / sqrt(2. * M_PI * t) / t * leg.Compute(0., t));
  //return (exp(b * b / t / -2.) * fabs(b) / sqrt(2. * M_PI * t) / t);
  //return (4. * sqrt(1. - t * t));
 }
};

double legendre(double a, double b) {
 Integral2 in;
 in.a = a;
 in.b = b;
 Legendre<Integral2> leg(in);
 return (1. - leg.Compute(0., 125));
}

double ro(double t, double b) {
 return (exp(-(b * b / t / 2.)) * fabs(b) / sqrt(2. * M_PI * t) / t);
}

double euler_tan(double a, double b) {
 double sum = 0., isum = 0., t, pf = 0., f, pfi = 0., fi, dt = 1e-1, count = 0.;
 for(t = dt; t < 1e5; t += dt) {
  fi = ro(t, a);
  isum += (pfi + fi) * dt / 2.; 
  pfi = fi;
  f = ro(t + dt, b) * (1. - isum);
  sum += (pf + f) * dt / 2.;
  pf = f;
if(t - count > 1e3) {
cerr << '\r' << isum << " " << sum << " " << t;
count = t;
}
 }
cerr << endl << "t = " << t << " isum = " << isum << endl;
 return sum;
}

double roh(double t, double a, double b) {
 double m, ia, ib, r;
 m = sqrt(2. * M_PI);
 ia = stdnormal_cdf(-a / sqrt(t)) * m;
 ib = stdnormal_cdf(-b / sqrt(t)) * m;
 r = (a * exp(a * a / t / -2.) * ib - b * exp(b * b / t / -2.) * ia) / pow((ia + ib), 2.) / sqrt(pow(t, 3.)) / m / 2.;
/*
if(isnan(r)) {
 r = 0.;
}
cerr << ia << " " << ib << " " << r << endl;
exit(1);
*/
 return r;
}

double euler_hyp(double a, double b) {
 double sum = 0., t, pf = 0., f, dt = 1e-0, count = 0.;
 a = fabs(a);
 b = fabs(b);
 for(t = dt; t < 1e5; t += dt) {
  f = roh(t, a, b);
  sum += (pf + f) * dt / 2.;
  pf = f; 
if(t - count > 1e3) {
cerr << '\r' << sum << " " << t;
count = t;
}
 }
 cerr << " " << sum << endl;
 return sum;
}

int main() {
 double a = 10, b = 10.;
 //cout << integrate(a, b) << endl;
 //cout << legendre(a, b) << endl;
 //cout << euler_tan(a, b) << endl;
 cout << euler_hyp(a, b) << endl;
 return 0;
}
