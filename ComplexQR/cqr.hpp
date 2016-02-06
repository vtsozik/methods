#ifndef CQR_HPP
#define CQR_HPP
#include <complex>
#include <vector>
#include <iostream>
#include <math.h>
using namespace std;

typedef complex<double> cd;
typedef vector<cd> CDArray;
typedef vector<CDArray> CDMatrix;

const static cd zero(0.,0.);
const static cd mone(-1.,0.);
const static cd one(1.,0.);
const static cd two(2.,0.);

class QR {
 CDMatrix q_, r_;
 CDArray eigenv_, coeffs_;
 QR() {};
 void Normalize(int col);
 void ConstructP(CDMatrix &p);
 void TransposeQ();
public:
 QR(CDMatrix &cdm) : r_(cdm) {}
 void ComputeQRMatrices();
 void Multiply(CDMatrix &prod, CDMatrix &mult);
 const CDMatrix &GetQ() const {return q_;}
 const CDMatrix &GetR() const {return r_;}
 void PrintArray(const CDArray &arr);
 void PrintMatrix(const CDMatrix &mat);
};

inline double cval1(const cd& c) {
 return c.real() + c.imag();
}

struct cless : binary_function<cd, cd, bool> {
 bool operator() (const cd& l, const cd& r) const {return cval1(l) < cval1(r);}
};
#endif //CQR_HPP
