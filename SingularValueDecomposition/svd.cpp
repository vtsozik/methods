#include "egvt.hpp"
#include "svd.hpp"

#define DEBUG

void SVD::operator()(const DMatrix &in, DMatrix &u, DMatrix &e, DMatrix &vt) const {
 u.clear();
 e.clear();
 vt.clear();
 if(in.size() > 0 && in[0].size() > 0) {
  DMatrix intr, prod, prodtr;
  DArray eigvll, eigvlr;
  TransposeMatrix(in, intr);
  MultiplyMatrices(in, intr, prod);
  MultiplyMatrices(intr, in, prodtr);
#ifdef DEBUG
cout << "Input Matrix: " << endl;
PrintMatrix(in);
cout << "Transposed Matrix: " << endl;
PrintMatrix(intr);
cout << "Product Matrix: " << endl;
PrintMatrix(prod);
cout << "Transposed Product Matrix: " << endl;
PrintMatrix(prodtr);
#endif
  EigenVectors egvtl(prod), egvtr(prodtr);
  egvtl.GetEigenVectors(u, eigvll);
  egvtr.GetEigenVectors(vt, eigvlr);
  DArray darr(in[0].size());
  e.reserve(in.size());
  for(int i = 0; i < in.size(); ++i) {
   for(int j = 0; j < in[0].size(); ++j) {
    darr[j] = (i == j) ? sqrt(eigvll[j]) : 0.;  
   }
   e.push_back(darr);
  }
 }
}

void SVD::ComputeTriProduct(const DMatrix &m1, const DMatrix &m2, const DMatrix &m3, DMatrix &trpr) const {
 DMatrix dpr;
 MultiplyMatrices(m1, m2, dpr);
 MultiplyMatrices(dpr, m3, trpr);
}

void SVD::ComputeOriginal(const DMatrix &u, const DMatrix &e, const DMatrix &vt, DMatrix &orig) const {
 DMatrix v;
 TransposeMatrix(vt, v);
 ComputeTriProduct(u, e, v, orig);
}

void SVD::ComputeInverse(const DMatrix &u, const DMatrix &e, const DMatrix &vt, DMatrix &inv) const {
 double eig;
 DMatrix ei = e, ut;
 for(int i = 0; i < e.size(); ++i) {
  eig = RoundToZero(e[i][i]);
  ei[i][i] = (eig != 0.) ? (1. / eig) : 0.;
 }
 TransposeMatrix(u, ut);
 ComputeTriProduct(vt, ei, ut, inv);
}

bool SVD::Verify(const DMatrix &in, const DMatrix &u, const DMatrix &e, const DMatrix &vt) const {
 bool r = (in.size() > 0 && u.size() > 0 && e.size() > 0 && vt.size() > 0);
 if(r) {
  DMatrix out;
  ComputeOriginal(u, e, vt, out);
  int s1 = in.size(), s2 = out.size(), s3, s4;
  r = (s1 == s2);
  if(r) {
   for(int i = 0; i < s1 && r; ++i) {
    s3 = in[i].size();
    s4 = out[i].size();
    r = (s3 == s4);
    for(int j = 0; j < s3 && r; ++j) {
     r = (RoundToZero(fabs(in[i][j]) - fabs(out[i][j])) == 0.);
    }
   } 
  }
#ifdef DEBUG
cout << endl << "Input Matrix: " << endl;
PrintMatrix(in);
cout << "U Matrix: " << endl;
PrintMatrix(u);
cout << "E Matrix: " << endl;
PrintMatrix(e);
cout << "VT Matrix: " << endl;
PrintMatrix(vt);
cout << "Output Matrix: " << endl;
PrintMatrix(out);
#endif
 }
#ifdef DEBUG
if(!r) {
cout << "SVD failed! Matrix sizes are:" << endl;
cout << " Input Matrix: " << in.size() << endl;
cout << " U Matrix: " << u.size() << endl;
cout << " E Matrix: " << e.size() << endl;
cout << " VT Matrix: " << vt.size() << endl;
cout << endl;
}
#endif
 return r;
}

#if 1
#include <fstream>
#include <sstream>
using namespace std;

int main() {
 SVD svd;
 DMatrix in, u, e, vt, v, prod, out;
 ifstream ifs("data.dat");
 string line;
 while(getline(ifs, line)) {
  DArray darr;
  double val;
  stringstream iss(line);
  while(iss >> val) {
   darr.push_back(val);
  }
  in.push_back(darr);
 }
 svd(in, u, e, vt); 
 if(svd.Verify(in, u, e , vt)) {
  cout << "SVD decomposition was successfully verified!" << endl;
 }
 return 0;
}
#endif
