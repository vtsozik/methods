#include "Integral.hpp"

double NormalIntegral::operator()(const double &value) const {
 return (exp(pow((value - mean_) / sigma_, 2.) / -2.)) / sqrt(2. * M_PIl) / sigma_;
}

double SigmaIntegral::operator()(const double &value) const {
 double z = (value - mean_) / sigma_;
 return (z * exp(pow(z, 2.) / -2.)) / sqrt(2. * M_PIl) / sigma_;
}
