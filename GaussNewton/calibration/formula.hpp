#ifndef FORMULA_HPP
#define FORMULA_HPP

#include "matrix.hpp"
#include "normal.h"

class Formula;
typedef double (Formula::*FormFunc)(const double) const;
typedef vector<FormFunc> fvect;

class Formula {
 fvect fv_; 
 DArray c_;
 double First(const double x) const { return exp(pow((x - c_[0]) / c_[1], 2.) / -2.) / sqrt(2. * M_PIl) / c_[1]; }
 double Second(const double x) const { 
  double z = (x - c_[0]) / c_[1];
  return z * exp(pow(z, 2.) / -2.) / sqrt(2. * M_PIl) / c_[1];
 }
 double Residual(const double x) const { return -stdnormal_cdf((x - c_[0]) / c_[1]); }
public: 
 Formula(const DArray& c) : c_(c) {
  fv_.reserve(c_.size() + 1); 
  fv_.push_back(&Formula::First);
  fv_.push_back(&Formula::Second);
  fv_.push_back(&Formula::Residual);
 } 
 void SetC(const DArray &c) { c_ = c; }
 const DArray& GetC() const { return c_; }
 double operator()(const double x, const int i) const {
  return (this->*fv_[i])(x);
 }
};

#endif //FORMULA_HPP
