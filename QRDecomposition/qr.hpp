#ifndef QR_HPP
#define QR_HPP
#include <vector>
#include <iostream>
#include <math.h>
using namespace std;

typedef vector<double> DArray;
typedef vector<DArray> DMatrix;

class QR {
 DMatrix q_, r_;
 DArray eigenv_, coeffs_;
 QR() {};
 void Normalize(int col);
 void ConstructP(DMatrix &p);
 void TransposeQ();
public:
 QR(DMatrix &dm) : r_(dm) {}
 void ComputeQRMatrices();
 void Multiply(DMatrix &prod, DMatrix &mult);
 const DMatrix &GetQ() const {return q_;}
 const DMatrix &GetR() const {return r_;}
 void PrintArray(const DArray &arr);
 void PrintMatrix(const DMatrix &mat);
};
#endif //QR_HPP
