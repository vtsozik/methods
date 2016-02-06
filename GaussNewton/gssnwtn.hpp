#ifndef GSSNWTN
#define GSSNWTN

#include "matrix.hpp"
#include "svd.hpp"

template <class T> class GaussNewton {
 T t_;
 DArray x_, y_;
 void ComputeResiduals(DArray &res) const;
 void ComputeJacobian(DMatrix &jac) const;
 void ComputeDeltas(const DMatrix &jac, const DArray &res, DMatrix &dc) const;
 double UpdateParameters(const DMatrix &dc, DArray &c);
public:
 GaussNewton(const DArray &x, const DArray &y, const DArray &c) : x_(x), y_(y), t_(c) {};
 double Fit(DArray &c);
};

template <class T> void GaussNewton<T>::ComputeResiduals(DArray &res) const {
 int ind = t_.GetC().size();
 for(int i = 0; i < x_.size(); ++i) {
  res[i] = y_[i] + t_(x_[i], ind);
 }
//cerr << "Computed Residuals:" << endl;
//PrintArray(res);
}

template <class T> void GaussNewton<T>::ComputeJacobian(DMatrix &jac) const {
 int size = t_.GetC().size();
 for(int i = 0; i < x_.size(); ++i) {
  for(int j = 0; j < size; ++j) {
   jac[i][j] = t_(x_[i], j);
  }
 }
//cerr << "Computed Jacobian:" << endl;
//PrintMatrix(jac);
}

template <class T> void GaussNewton<T>::ComputeDeltas(const DMatrix &jac, const DArray &res, DMatrix &dc) const {
 SVD svd;
 double eig;
 DMatrix resm(1, res), resmt, jact, jacsq, u, e, vt, inv;
 TransposeMatrix(jac, jact);
 MultiplyMatrices(jact, jac, jacsq); 
//cerr << "SVD Input Matrix:" << endl;
//PrintMatrix(jacsq);
 svd(jacsq, u, e, vt);
 svd.ComputeInverse(u, e , vt, inv);
 TransposeMatrix(resm, resmt);
 svd.ComputeTriProduct(inv, jact, resmt, dc);
}

template <class T> double GaussNewton<T>::UpdateParameters(const DMatrix &dc, DArray &c) {
 double sum = 0.;
 c = t_.GetC();
 for(int i = 0; i < c.size(); ++i) {
  c[i] -= dc[i][0];
  sum += dc[i][0] * dc[i][0];
 }
 t_.SetC(c);
 return sqrt(sum);
}

template <class T> double GaussNewton<T>::Fit(DArray &c) {
 double sum = 0.;
 DArray res(x_.size(), 0.);
 DMatrix jac(x_.size(), t_.GetC()), dc(1, t_.GetC());
 do {
//break;
  ComputeResiduals(res);
  ComputeJacobian(jac);
  ComputeDeltas(jac, res, dc); 
cerr << "Deltas: " << endl;
PrintMatrix(dc); 
 } while(RoundToZero(UpdateParameters(dc, c)) != 0.);
 ComputeResiduals(res);
 for(int i = 0; i < res.size(); ++i) {
  sum += res[i] * res[i];
 }
 return sqrt(sum);
}

#endif //GSSNWTN
