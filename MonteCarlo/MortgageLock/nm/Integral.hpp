#ifndef NORMAL_INTEGRAL_HPP
#define NORMAL_INTEGRAL_HPP

#include <math.h>

#include <iostream>
using namespace std;

class Integral {
protected:
 double mean_, sigma_; 
public:
 void SetMean(double mean) { mean_ = mean; }
 void SetSigma(double sigma) { sigma_ = sigma; }
 double GetMean() { return mean_; }
 double GetSigma() { return sigma_; }
 virtual double operator()(const double &value) const = 0;
};

class NormalIntegral : public virtual Integral {
public:
 double operator()(const double &value) const;
};

class SigmaIntegral : public virtual Integral {
public:
 double operator()(const double &value) const;
};

#endif // NORMAL_INTEGRAL_HPP
