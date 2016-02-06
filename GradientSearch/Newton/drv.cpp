//#define TSTDRV

#include "drv.hpp"
#include <math.h>
using namespace std;

#if 0
int main() {
 Function f;
 Derivative d(f, 1e-9);
 dvect ipnt;
 ipnt.push_back(-3.);
 ipnt.push_back(-2.);
#ifdef TSTDRV
 int depth = 2;
 ivect ind;
 for(int j = 0; j < depth; ++j) {
  ind.push_back(j);
 }
 d.Partial(ipnt, ind);
#else
 int depth = 10;
for(int i = 0; i <= depth; ++i) {
 ivect ind;
 for(int j = 0; j < i; ++j) {
  ind.push_back(0);
 }
 d.Partial(ipnt, ind);
}
#endif
 return 0;
}
#endif

double Derivative::Partial(const dvect &pnt, const ivect &ind, bool ab/*= false*/) {
 double delta = 1., conv, min, previous, next;
 bool binom = false;
 int depth = ind.size();
 if(ab) {
  for(int i = 1; i < depth; ++i) {
   if(!(binom = (ind[i - 1] == ind[i]))) {
    break;
   }
  }
 }
 next = (binom) ? Partial(pnt, ind[0], depth, delta) : Partial(pnt, ind, depth, delta);
 do {
  previous = next;
  delta /= 10.;
  next = (binom) ? Partial(pnt, ind[0], depth, delta) : Partial(pnt, ind, depth, delta);
  conv = fabs(previous);
  if(conv > 0.) {
   conv = fabs(previous - next) / conv;
  }
  if(delta == .1 || conv < min) {
   min = conv;
   if(min < p) {
    break;
   }
  } else if((conv / min) > 10.) {
   next = previous;
   delta *= 10.;
   break;
  }
 } while(delta > p);
//cout << "depth: " << depth << " next: " << next << " delta: " << delta << " p = " << p << " min = " << min << endl;
 return next;
}

double Derivative::Partial(const dvect &pnt, const ivect &ind, int depth, double delta) {
 if(depth == 0) {
  return f(pnt);
 } else {
  dvect up(pnt), dn(pnt);
  int i = ind.size() - depth;
  up[ind[i]] += delta;
  dn[ind[i]] -= delta;
  --depth;
  return ((Partial(up, ind, depth, delta) - Partial(dn, ind, depth, delta)) / delta / 2.);
 }
}

double Derivative::Partial(const dvect &pnt, int ind, int depth, double delta) {
 double sum = 0., step = 2. * delta, mult = 1.;
 dvect shift = pnt;
 shift[ind] += (delta * static_cast<double>(depth));
 for(int i = 0; i <= depth; ++i) {
  sum += (mult * Combinations(depth, i) * f(shift));
  mult *= -1.;
  shift[ind] -= step;
 } 
 return (sum / pow(step, depth));
}

double Derivative::Factorial(int n) {
 double f = 1.;
 for(int i = 2; i <= n; ++i) {
  f *= static_cast<double>(i);
 }
 return f;
}
