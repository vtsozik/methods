#ifndef LEGENDRE_HPP
#define LEGENDRE_HPP

#include <iostream>
using namespace std;

template <class T> class Legendre {
 const T& integral_;
 static const int coeffSize_ = 5;
 static const double xCoeffs[], wCoeffs[];
public:
 Legendre(const T& integral) : integral_(integral) {}
 double Compute(double ll, double ul) {
  double sum = 0., av = (ll + ul) / 2., hd = (ul - ll) / 2.;
//cerr << "|||| " << ll << " " << ul << " ||||" << endl;
  for(int i = 0; i < coeffSize_; ++i) {
   sum += (wCoeffs[i] * integral_(av + hd * xCoeffs[i]));
  }
//cerr << "|||| " << hd * sum << " ||||" << endl;
  return (hd * sum);
 }
};

template <class T> const double Legendre<T>::xCoeffs[coeffSize_] = { -0.906179846, -0.53846931, 0., 0.53846931, 0.906179846 };
template <class T> const double Legendre<T>::wCoeffs[coeffSize_] = { 0.236926885, 0.47862867, 0.568888889, 0.47862867, 0.236926885 };

#endif //LEGENDRE_HPP
