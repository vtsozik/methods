#include "Formula.hpp"
//#include "Legendre.hpp"
#include "normal.h"

#include <iostream>
using namespace std;

double Formula::operator()(double value) {
 double common, sum = 0.;
#ifdef LEGENDRE_HPP
 Legendre<Integral> nLeg(normInt_);
#endif
 SetParameters(value);
 for(DDMapIter iter = distMap_.begin(); iter != distMap_.end(); ++iter) {
#ifdef LEGENDRE_HPP
  common = iter->second - .5 - nLeg.Compute(normInt_.GetMean(), iter->first);
#else
  common = iter->second - stdnormal_cdf((iter->first - normInt_.GetMean()) / normInt_.GetSigma());
#endif
  if(type_ == MEAN) {
   sum -= (2. * common * normInt_(iter->first));
  } else if(type_ == SIGMA) {
   sum -= (2. * common * sigInt_(iter->first));
  } else if(type_ == THETA) {
   sum -= (2. * common * (gradient_mean_ * normInt_(iter->first) + gradient_sigma_ * sigInt_(iter->first)));
  } else if(type_ == SQUARE) {
   sum += pow(common, 2.); 
  }
 }
 return sum;
}

void Formula::SetParameters(double value) {
 double mean = mean_, sigma = sigma_;
 if(type_ == THETA) {
  theta_ = value;
  mean += (theta_ * gradient_mean_); 
  sigma += (theta_ * gradient_sigma_); 
 }
 normInt_.SetMean(mean); 
 sigInt_.SetMean(mean); 
 normInt_.SetSigma(sigma); 
 sigInt_.SetSigma(sigma);
}
