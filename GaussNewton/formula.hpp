#ifndef FORMULA_HPP
#define FORMULA_HPP

#include "matrix.hpp"

class Formula;
typedef double (Formula::*FormFunc)(const double) const;
typedef vector<FormFunc> fvect;

class Formula {
 fvect fv_; 
 DArray c_;
 double First(const double x) const { return (-x / (c_[1] + x)); }
 double Second(const double x) const { return ( c_[0] * x / (c_[1] + x) / (c_[1] + x)); }
 double Residual(const double x) const { return (-c_[0] * x / (c_[1] + x)); }
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
