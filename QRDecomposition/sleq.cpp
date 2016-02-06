#include "sleq.hpp"
#include "lpl.hpp"

#define LAPL

double SLEQ::ComputeDeterminant(const DMatrix &dmat) {
#ifdef LAPL
 Laplacian lpl(dmat);
 return lpl.GetDeterminant();
#else
 EigenValues egv(dmat);
 egv.ComputeEigenValues();
 return egv.GetDeterminant();
#endif
} 

void SLEQ::GetRoots(DArray &roots) {
 int size = left_.size();
 if(!roots.empty()) {
  roots.erase(roots.begin(), roots.end());
 }
 roots.reserve(size);
 double determ = ComputeDeterminant(left_);
cout << "Matrix determinant: " << determ << endl;
#if 1
 for(int i = 0; i < size; ++i) {
  DMatrix dmat = left_; 
  for(int j = 0; j < size; ++j) {
   dmat[j][i] = right_[j];
  }
//cout << "Matrix: " << i << endl; 
//QR(dmat).PrintMatrix(dmat);
  roots.push_back(ComputeDeterminant(dmat) / determ);
cout << "Root determinant: " << roots.back() << endl;
 }
#endif
}

#if 0
int main() {
 DMatrix dmat;
 DArray darr;
 darr.push_back(2.); 
 darr.push_back(7.); 
 darr.push_back(1.); 
 dmat.push_back(darr);
 darr[0]=3.; 
 darr[1]=4.; 
 darr[2]=8.; 
 dmat.push_back(darr);
 darr[0]=4.; 
 darr[1]=3.; 
 darr[2]=4.; 
 dmat.push_back(darr);
 darr[0]=75.; 
 darr[1]=119.; 
 darr[2]=80.; 
 SLEQ sl(dmat, darr); 
 sl.GetRoots(darr);
 for(int i = 0; i < darr.size(); ++i) {
  cout << "x[" << i << "]=" << darr[i] << " ";
 }
 cout << endl;
 return 0;
}
#endif
