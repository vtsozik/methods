#ifndef FORMULA_HPP
#define FORMULA_HPP

#include "math.h"
#include <iostream>
using namespace std;

class formula {
 double cd_, cmr_;
public: 
 inline formula(double kl) {
  cd_ = 1.761949183 * kl - 0.348258706;
  cmr_ = pow(2., 1.910891689 * log(kl) / log(2.) + 6.689600985);
cerr << "cd_ = " << cd_ << " cmr_ = " << cmr_ << endl;
 }
 inline double operator()(double x) {
  double xmo = x - 1., lxmo = log(xmo);
  return cd_ * (3 * x * x / lxmo -
   x * x * x / xmo / lxmo / lxmo) - cmr_ * x * 2.;
 }
};

#endif //FORMULA_HPP
