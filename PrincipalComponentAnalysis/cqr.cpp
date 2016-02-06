#include "cqr.hpp"

void QR::Normalize(int col) {
 cd d = zero, p;
 CDArray nrm;
 if(col == 0) {
  coeffs_.reserve(r_.size());
  for(int i = 0; i < r_.size(); ++i) {
   coeffs_.push_back(zero);
  }
 }
 for(int i = col; i < coeffs_.size(); ++i) {
  d += (r_[i][col] * r_[i][col]);
 }
 p = sqrt(d);
 d = zero;
 nrm.reserve(coeffs_.size() - col);
 for(int i = col; i < coeffs_.size(); ++i) {
  nrm.push_back(r_[i][col] / p);
  d += (nrm[i - col] * nrm[i - col]);
 }
 d = sqrt(d);
 if(nrm[0].real() > 0.) {
  d *= mone;
 } 
 for(int i = 0; i < coeffs_.size(); ++i) {
  if(i < col) {
   coeffs_[i] = zero;
  } else if(i == col) {
   coeffs_[i] = sqrt((one - (nrm[i - col] / d)) / two);
   p = mone * coeffs_[i] * d;
  } else {
   coeffs_[i] = nrm[i - col] / p / two; 
  }
 }
}

void QR::ConstructP(CDMatrix &p) {
 if(p.size() == 0) {
  CDArray row;
  row.reserve(r_.size());
  for(int i = 0; i < r_.size(); ++i) {
   row.push_back(zero);
  }
  p.reserve(r_.size());
  for(int i = 0; i < r_.size(); ++i) {
   p.push_back(row);
  }
 }
 for(int i = 0; i < r_.size(); ++i) {
  for(int j = 0; j < r_.size(); ++j) { 
   p[i][j] = ((i == j) ? one : zero) - (two * coeffs_[i] * coeffs_[j]);
  }
 }
}

void QR::ComputeQRMatrices() {
 CDMatrix p;
 for(int i = 0; i < r_.size() - 1; ++i) {
  Normalize(i);
  ConstructP(p);
  if(q_.size() == 0) {
   q_ = p;
  } else {
   Multiply(q_, p);
  }
  Multiply(r_, p);
 } 
 TransposeQ();
}

void QR::Multiply(CDMatrix &prod, CDMatrix &mult) {
 CDMatrix old = prod;
 for(int i = 0; i < prod.size(); ++i) {
  for(int j = 0; j < prod.size(); ++j) {
   prod[i][j] = zero;
   for(int k = 0; k < prod.size(); ++k) {
    prod[i][j] += (mult[i][k] * old[k][j]);
   }
  }
 }
}

void QR::TransposeQ() {
 CDMatrix old = q_; 
 for(int i = 0; i < q_.size(); ++i) {
  for(int j = 0; j < q_.size(); ++j) {
   q_[i][j] = old[j][i]; 
  }
 }
}

void QR::PrintArray(const CDArray &arr) {
 for(int i = 0; i < arr.size(); ++i) {
  cout << " " << arr[i];
 }
 cout << endl;
}

void QR::PrintMatrix(const CDMatrix &mat) {
 for(int i = 0; i < mat.size(); ++i) {
  PrintArray(mat[i]);
 }
 cout << endl;
}
