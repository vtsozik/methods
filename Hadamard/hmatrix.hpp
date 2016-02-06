#ifndef HMATRIX_HPP
#define HMATRIX_HPP

#include "imatrix.hpp"

class HMatrix {
 mutable IMatrix gm;
public:
 inline HMatrix() : gm(0) {
  int gms = 2;
  IArray r(gms, 1);
  gm.reserve(gms);
  gm.push_back(r); 
  r[gms - 1] = -1;
  gm.push_back(r); 
 }
 inline const IMatrix& GetGen() const { return gm; }
 inline void GenHadMat(const int &size, IMatrix &r) const {
  r = gm;
  for(int i = r.size(); i < size;) {
   IMatrix t(0);
   i = KronProd(r, gm, t);
   r = t; 
  }
 }
};

#endif //HMATRIX_HPP
