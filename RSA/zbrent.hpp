#ifndef ZBRENT_HPP
#define ZBRENT_HPP

#include <iostream>
using namespace std;

template <class T> class zbrent {
  T formula_;
  double precision_;
 public:
  zbrent(const T& formula, const double &precision) : formula_(formula), precision_(precision) {} 
  double Solve(double, double);
};

template <class T> double zbrent<T>::Solve(double ll, double ul) {
 double a = (ll + ul) / 2, na, yll, ya = formula_(a), yul, r, s, t, p, q; 
//cerr << ll << " | " << a << " | " << ul << " | " << ya << endl;
 while(fabs(ya) > precision_) {
  yll = formula_(ll);
  yul = formula_(ul);
  r = ya / yul;
  s = ya / yll;
  t = yll / yul;
//cerr << yll << " " << ya << " " << yul << endl;
  p = s * (t * (r - t) * (ul - a) - (1 - r) * (a - ll));
  q = (t - 1) * (r - 1) * (s - 1);
  na = a + p / q; 
  if(na > a) {
   ll = a;
  } else if(na < a) {
   ul = a;  
  } else {
//cerr << ll << " / " << na << " / " << ul << " / " << ya << endl;
   return na;
  }
  a = na;
  ya = formula_(a);
//cerr << ll << " - " << a << " - " << ul << " - " << ya << endl;
 }
 return a;
}

#endif // ZBRENT_HPP
