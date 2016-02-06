#include "egvt.hpp"

void EigenVectors::InitArray(DArray &darr, int size) {
 darr.reserve(size);
 for(int i = 0; i < size; ++i) {
  darr.push_back(0.);
 }
}

void EigenVectors::InitMatrix(DMatrix &dmat, int size) {
 if(!dmat.empty()) {
  dmat.erase(dmat.begin(), dmat.end());
 }
 DArray darr;
 InitArray(darr, size);
 dmat.reserve(size);
 for(int i = 0; i < size; ++i) {
  dmat.push_back(darr);
 }
}

void EigenVectors::GetEigenVectors(DMatrix &eigvects) {
 DArray darrtmpl;
 DMatrix dmattmpl; 
 EigenValues egvl(input_);
 int size = input_.size(), tmplsize = size - 1;
 InitMatrix(eigvects, size);
 InitMatrix(dmattmpl, tmplsize);
 InitArray(darrtmpl, tmplsize);
 egvl.ComputeEigenValues();
 const DArray &darregv = egvl.GetRealEigenValues();
 for(int i = 0; i < size; ++i) {
  FindEigenVector(eigvects, darregv[i], i, dmattmpl, darrtmpl);
 }
}

void EigenVectors::FindEigenVector(DMatrix &eigvects, double eigvl, int ind, DMatrix &dmattmpl, DArray &darrtmpl) {
 PopulateTemplate(dmattmpl, darrtmpl, eigvl, ind);
 SLEQ sleq(dmattmpl, darrtmpl); 
 sleq.GetRoots(darrtmpl);
 double root = AssumeOrthogonal(darrtmpl);
 int size = eigvects.size();
 for(int i1 = 0, i2 = 0; i1 < size; ++i1) {
  eigvects[i1][ind] = ((i1 != ind) ? (root * darrtmpl[i2++]) : root);
 }
}

void EigenVectors::PopulateTemplate(DMatrix &dmattmpl, DArray &darrtmpl, double eigvl, int ind) {
 int size = input_.size();
 for(int i1 = 0, i2 = 0; i1 < size; ++i1) {
  if(i1 != ind) {
   darrtmpl[i2] = -1. * input_[i1][ind];
   for(int j1 = 0, j2 = 0; j1 < size; ++j1) {
    if(j1 != ind) {
     dmattmpl[i2][j2] = ((i1 == j1) ? (input_[i1][j1] - eigvl) : input_[i1][j1]);
     ++j2;
    }
   }
   ++i2;
  }
 }
}

double EigenVectors::AssumeOrthogonal(DArray &darrtmpl) {
 double sum = 1.;
 int size = darrtmpl.size();
 for(int i = 0; i < size; ++i) {
  sum += (darrtmpl[i] * darrtmpl[i]);
 }
 return sqrt(1. / sum);
}

#if 1
#include <fstream>
#include <sstream>

int main() {
 DMatrix dmat, deigvt;
 ifstream ifs("data.dat");
 string line;
 while(getline(ifs, line)) {
  DArray darr;
  double val;
  stringstream iss(line);
  while(iss >> val) {
   darr.push_back(val);
  }
  dmat.push_back(darr);
 }
 EigenVectors egvt(dmat);
 egvt.GetEigenVectors(deigvt);
 PrintMatrix(deigvt);
 return 0;
}
#endif
