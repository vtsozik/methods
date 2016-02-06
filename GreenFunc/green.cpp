#include <math.h>
#include <iostream>
using namespace std;

#define DEFAULT_PRECISION 1e3

inline double evaluate_pi(double stp) {
 return 4. * sqrt(1. - stp * stp);
}

double integrate_pi() {
 double stp = 0., f, pf = evaluate_pi(stp), sum = 0., ul = 1., res = DEFAULT_PRECISION * DEFAULT_PRECISION;
 for(stp = (ul / res); stp <= ul; stp += (ul / res)) {
  f = evaluate_pi(stp);
  sum += (1.5 * f - .5 * pf) * (ul / res);
  pf = f;
 }
 return sum;
}

inline double charge_density(double sr, double st, double tr, double tt) {
 return 1. / (1. + sr * sr + tr * tr - 2. * sr * tr * cos(tt - st));
}

double evaluate(double rstp, double tstp, double sr, double st, double tr, double tt) {
 double r1, r2, f;
 r1 = sqrt(tr * tr - 2. * tr * rstp * cos(tt - tstp) + rstp * rstp);
 r2 = sqrt(tr * tr * rstp * rstp - 2. * tr * rstp * cos(tt - tstp) + 1.);
 f = log(r2 / r1) * charge_density(sr, st, tr, tt) / 2. / M_PI;
 return f;
}

double integrate(double sr, double st, double tr, double tt, double rstp) {
 double res = DEFAULT_PRECISION, tstp = 0., tul = 2. * M_PI, f, pf, sum = 0.;
 pf = evaluate(rstp, tstp, sr, st, tr, tt);
 for(tstp = (tul / res); tstp <= tul; tstp += (tul / res)) {
  f = evaluate(rstp, tstp, sr, st, tr, tt);
  sum += (1.5 * f - .5 * pf) * (tul / res);
  pf = f;
 }
 return sum;
}

double integrate(double sr, double st, double tr, double tt) {
 double res = DEFAULT_PRECISION, rul = 1., sum = 0., rstp = 0., f, pf;
 pf = integrate(sr, st, tr, tt, rstp);
 for(rstp = (rul / res); rstp <= rul; rstp += (rul / res)) {
  f = integrate(sr, st, tr, tt, rstp);
  sum += (1.5 * f - .5 * pf) * (rul / res);
  pf = f;
 }
 return sum;
}

int main() {
 cout << "Integration calibration pi: " << integrate_pi() << endl;
 double sr = .4, st = 2. * M_PI * 45. / 360., tr = 1., tt = 2. * M_PI * 75. / 360.;
 cout << "Solution: " << integrate(sr, st, tr, tt) << endl;
 return 0;
}
