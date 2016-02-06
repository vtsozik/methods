#ifndef CQR_HPP
#define CQR_HPP

#include <math.h>
#include <complex>
#include <vector>
#include <iostream>
using namespace std;

typedef complex<double> cd;
typedef vector<cd> CDArray;
typedef vector<CDArray> CDMatrix;

const static cd zero(0.,0.);
const static cd minusone(-1.,0.);
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
};

inline double cval1(const cd& c) {
 return c.real() + c.imag();
}

struct cmore : binary_function<cd, cd, bool> {
 bool operator() (const cd& l, const cd& r) const {return cval1(l) > cval1(r);}
};

inline void PrintArray(const CDArray &arr) {
 for(int i = 0; i < arr.size(); ++i) {
  cout << " " << arr[i];
 }
 cout << endl;
}

inline void PrintMatrix(const CDMatrix &mat) {
 for(int i = 0; i < mat.size(); ++i) {
  PrintArray(mat[i]);
 }
 cout << endl;
}

inline int IsNAN(const CDArray &cdarr) {
 int i, r = 0, size = cdarr.size();
 for(i = 0; i < size && r == 0; ++i) {
  r = isnan(cdarr[i].real()) + isnan(cdarr[i].imag());
 }
 return r;
}

inline int IsNAN(const CDMatrix &cdmat) {
 int i, r = 0, size = cdmat.size();
 for(i = 0; i < size && r == 0; ++i) {
  r = IsNAN(cdmat[i]);
 }
 return r;
}

#endif //CQR_HPP
