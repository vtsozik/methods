#ifndef LEGENDRE_HPP
#define LEGENDRE_HPP

#include <math.h>
#include "matrix.hpp"

class Nom {
 double m;
public:
 double c, x, pc, p;
 Nom() : c(0.), x(0.), pc(1.), p(1.), m(1.) {}
 Nom(double c, double x, double p) : c(c), x(x), pc(1.), p(p), m(1.) {}
 inline double Differentiate() {
  m *= p;
  if(p != 0.) {
   p -= 1.;
  }
  return Y();
 }
 inline double Integrate() {
  if(p == -1.) {
   return m * pow(c, pc) * log(x);
  } else {
   p += 1.;
   m /= p;
   return Y(); 
  }
 }
 inline double Y() {
  return m * pow(c, pc) * pow(x, p);
 }
};

typedef vector<Nom> NArray;
typedef vector<NArray> NMatrix;

class Legendre {
 const static double precision = 1e-6;
 double ll, ul;
 int nn;
 DArray dIntegrals;
 NMatrix nJacobian, nFunction;
 void CalculateIntegrals();
 void Create();
 void Increment(DMatrix &dMatrix, DArray &dArray);
 void SolveSytem(DMatrix &dMatrix, DArray &dArray);
 double GetDeterminant(DMatrix &dMatrix);
 double ComputeError(DArray &dArray);
 void ExtractPolynomial(DArray &dArray);
public:
 Legendre(double ll, double ul, int nn);
 void FindPolynomial(DArray &dArray);
};

#endif //LEGENDRE_HPP
