#include "egvt.hpp"

void EigenVectors::InitArray(DArray &darr, int size) const {
 if(!darr.empty()) {
  darr.clear();
 }
 darr.reserve(size);
 for(int i = 0; i < size; ++i) {
  darr.push_back(0.);
 }
}

void EigenVectors::InitMatrix(DMatrix &dmat, int size) const {
 if(!dmat.empty()) {
  dmat.clear();
 }
 DArray darr(size, 0.);
 dmat.reserve(size);
 for(int i = 0; i < size; ++i) {
  dmat.push_back(darr);
 }
}

void EigenVectors::GetEigenVectors(DMatrix &egvts, DArray &egvls) const {
 EigenValues egvl(input_);
 egvl.ComputeEigenValues();
 egvls = egvl.GetRealEigenValues();
 int size = egvls.size(); 
 InitMatrix(egvts, size);
 for(int i = 0; i < size; ++i) {
  egvls[i] = RoundToZero(egvls[i]);
  FindEigenVector(egvts, egvls[i], i);
 }
}

void EigenVectors::FindEigenVector(DMatrix &egvts, double egvl, int ind) const {
 DArray egvt, darr;
 DMatrix dmat = input_, dmatprev; 
 double det = 0.;
 int i1, i2, j1, size = egvts.size(), rs = size;
 InitArray(egvt, size);
 darr = egvt;
 for(i1 = 0; i1 < size; ++i1) {
  dmat[i1][i1] -= egvl;
 }
 for(i1 = 0; i1 < ind && det == 0.; ++i1) {
  for(j1 = 0; j1 < size; ++j1) {
   dmat[i1][j1] = egvts[j1][i1];
  }
  det = ComputeSystem(dmat, darr);
 }
 if(det == 0.) {
  egvt[ind] = 1.;
  dmatprev = dmat;
  rs = TrimSystem(dmat, darr);
  PopulateSystem(dmat, darr, dmatprev, ind);
  while(rs > 1) {
   det = ComputeSystem(dmat, darr);
   if(det != 0.) {
    break;
   }
   rs = TrimSystem(dmat, darr);
  }
  if(det == 0.) {
   darr[0] = (dmat[0][0] == 0.) ? 0. : (darr[0] / dmat[0][0]);
  }
  for(i1 = 0, i2 = 0; i2 < rs; ++i1) {
   if(i1 != ind) {
    egvt[i1] = darr[i2];
    ++i2;
   }
  }
 }
 if(rs < size) { 
  Normalise(egvt);
 }
 for(i1 = 0; i1 < size; ++i1) {
  egvts[i1][ind] = RoundToZero(egvt[i1]);
 }
}

inline double EigenVectors::ComputeSystem(DMatrix &dmat, DArray &darr) const {
 SLEQ sleq(dmat, darr);
 return sleq.GetRoots(darr, RoundToZero(sleq.ComputeDeterminant()));
}

void EigenVectors::PopulateSystem(DMatrix &dmat, DArray &darr, const DMatrix &appld, int ind) const {
 int size = appld.size();
 for(int i1 = 0, i2 = 0; i1 < size; ++i1) {
  if(i1 != ind) {
   darr[i2] -= appld[i1][ind];
   for(int j1 = 0, j2 = 0; j1 < size; ++j1) {
    if(j1 != ind) {
     dmat[i2][j2] = appld[i1][j1];
     ++j2;
    }
   }
   ++i2;
  }
 }
}

int EigenVectors::TrimSystem(DMatrix &dmat, DArray &darr) const {
 dmat.pop_back();
 darr.pop_back(); 
 int i, size = dmat.size();
 for(i = 0; i < size; ++i) {
  dmat[i].pop_back();
 }
 return size;
}

double EigenVectors::Normalise(DArray &darr) const {
 double sum = 0.;
 int i, size = darr.size();
 for(i = 0; i < size; ++i) {
  sum += (darr[i] * darr[i]);
 }
 sum = sqrt(sum);
 for(i = 0; i < size; ++i) {
  darr[i] /= sum;
 }
 return sum;
}

#if 0
#include <fstream>
#include <sstream>

int main() {
 DArray degvl;
 DMatrix dmat, degvt;
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
 egvt.GetEigenVectors(degvt, degvl);
 cout << "Input Matrix:" << endl;
 PrintMatrix(dmat);
 cout << "Eigen Vectors:" << endl;
 PrintMatrix(degvt);
 cout << "Eigen Values:" << endl;
 PrintArray(degvl);
 return 0;
}
#endif
