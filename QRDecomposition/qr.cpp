#include "qr.hpp"

void QR::Normalize(int col) {
 double d = 0., p;
 DArray norm;
 if(col == 0) {
  coeffs_.reserve(r_.size());
  for(int i = 0; i < r_.size(); ++i) {
   coeffs_.push_back(0.);
  }
 }
 for(int i = col; i < coeffs_.size(); ++i) {
  d += (r_[i][col] * r_[i][col]);
 }
 p = sqrt(d);
 d = 0.;
 norm.reserve(coeffs_.size() - col);
 for(int i = col; i < coeffs_.size(); ++i) {
  norm.push_back(r_[i][col] / p);
  d += (norm[i - col] * norm[i - col]);
 }
 d = sqrt(d);
 if(norm[0] > 0) {
  d *= -1.;
 } 
 for(int i = 0; i < coeffs_.size(); ++i) {
  if(i < col) {
   coeffs_[i] = 0.;
  } else if(i == col) {
   coeffs_[i] = sqrt(.5 * (1 - (norm[i - col] / d)));
   p = -1. * coeffs_[i] * d;
  } else {
   coeffs_[i] = .5 * norm[i - col] / p; 
  }
 }
}

void QR::ConstructP(DMatrix &p) {
 if(p.size() == 0) {
  DArray row;
  row.reserve(r_.size());
  for(int i = 0; i < r_.size(); ++i) {
   row.push_back(0.);
  }
  p.reserve(r_.size());
  for(int i = 0; i < r_.size(); ++i) {
   p.push_back(row);
  }
 }
 for(int i = 0; i < r_.size(); ++i) {
  for(int j = 0; j < r_.size(); ++j) { 
   p[i][j] = ((i == j) ? 1. : 0.) - (2. * coeffs_[i] * coeffs_[j]);
  }
 }
}

void QR::ComputeQRMatrices() {
 DMatrix p;
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

void QR::Multiply(DMatrix &prod, DMatrix &mult) {
 DMatrix old = prod;
 for(int i = 0; i < prod.size(); ++i) {
  for(int j = 0; j < prod.size(); ++j) {
   prod[i][j] = 0.;
   for(int k = 0; k < prod.size(); ++k) {
    prod[i][j] += (mult[i][k] * old[k][j]);
   }
  }
 }
}

void QR::TransposeQ() {
 DMatrix old = q_; 
 for(int i = 0; i < q_.size(); ++i) {
  for(int j = 0; j < q_.size(); ++j) {
   q_[i][j] = old[j][i]; 
  }
 }
}

void QR::PrintArray(const DArray &arr) {
 for(int i = 0; i < arr.size(); ++i) {
  cout << " " << arr[i];
 }
 cout << endl;
}

void QR::PrintMatrix(const DMatrix &mat) {
 for(int i = 0; i < mat.size(); ++i) {
  PrintArray(mat[i]);
 }
 cout << endl;
}
