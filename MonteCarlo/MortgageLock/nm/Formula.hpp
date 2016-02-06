#ifndef FORMULA_HPP
#define FORMULA_HPP

#include <map>
#include "Integral.hpp"
using namespace std;

typedef map<double, double> DDMap;
typedef DDMap::iterator DDMapIter;

class Formula {
 DDMap distMap_;
 NormalIntegral normInt_;
 SigmaIntegral sigInt_;
 double mean_, sigma_, gradient_mean_, gradient_sigma_, theta_;
public: 
 enum TYPE {MEAN, SIGMA, THETA, SQUARE};
 Formula(const DDMap &distMap) : type_(MEAN), distMap_(distMap) {}
 void SetMean(double mean) { mean_ = mean; }
 void SetSigma(double sigma) { sigma_ = sigma; }
 void SetTheta(double theta) { theta_ = theta; }
 void SetGradientMean(double gradient_mean) { gradient_mean_ = gradient_mean; }
 void SetGradientSigma(double gradient_sigma) { gradient_sigma_ = gradient_sigma; }
 void SetType(const TYPE& type) { type_ = type; }
 void SetParameters(double);
 double GetMean() { return mean_; }
 double GetSigma() { return sigma_; }
 double GetTheta() { return theta_; }
 double GetGradientMean() { return gradient_mean_; }
 double GetGradientSigma() { return gradient_sigma_; } 
 double operator()(double = 0.);
private:
 TYPE type_;
};

#endif //FORMULA_HPP
