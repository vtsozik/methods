#ifndef FULLTRANSFORM_HPP
#define FULLTRANSFORM_HPP

#include <stdlib.h>
#include <string.h>
#include "hmatrix.hpp"

//#define DEBUG

#define bits 8
#define cws 16
typedef char CodeWord[cws];

class FullTransform {
 mutable IMatrix hm, nhm, tm;
 HMatrix hmg;
public:
 const static int hms;
 inline FullTransform() : hm(0), nhm(0) {
  hmg.GenHadMat(hms, hm);
  NegateMatrix(hm, nhm);
 }
 inline const IMatrix& GetHad() const { return hm; }
 inline const IMatrix& GetNegHad() const { return nhm; }
 inline void FillCodeWord(const IArray &v, CodeWord cw) const {
  unsigned char p;
  memset(cw, 0, cws);
  for(int i = 0; i < cws; ++i) {
   p = 1;
   for(int j = 0; j < bits; ++j) {
    if(v[i * bits + j] > 0) {
     cw[i] |= p;
    }
    p *= 2;
   }
  }
 }
 inline void Code(unsigned char c, CodeWord cw) const {
  if(c & hms) {
   c &= ~hms;
   FillCodeWord(nhm[c], cw);
  } else {
   FillCodeWord(hm[c], cw);
  }
 }
 inline void ScanCodeWord(const CodeWord cw, IMatrix &orm) const {
  unsigned char p;
  IArray v(0);
  v.reserve(hms);
  for(int i = 0; i < cws; ++i) {
   p = 1;
   for(int j = 0; j < bits; ++j) {
    if(cw[i] & p) {
     v.push_back(1); 
    } else {
     v.push_back(-1); 
    } 
    p *= 2;
   }
  }
  orm.clear();
  orm.reserve(1);
  orm.push_back(v);
 }
 inline unsigned char FindChar(const IArray &v) const {
  unsigned char c = 0;
  int max, tr = hms / 2 + 2;
  for(int i = 0; i < hms; ++i) {
   if(abs(max = v[i]) >= tr) {
    c = i;
    if(max < 0) {
     c |= hms;
    } 
    break;
   }
  }
  return c;
 }
 inline unsigned char Decode(const CodeWord cw) const {
  IMatrix orm(0), r(0);
  ScanCodeWord(cw, orm);
  MatrixProduct(orm, hm, r);
  return FindChar(r[0]); 
 }
#ifdef DEBUG
 inline void PrintCodeWord(const CodeWord cw) const {
  for(int i = cws - 1; i >= 0 ; --i) {
   for(unsigned char j = hms; j >= 1; j /= 2) {
    cout << ((cw[i] & j) ? '1' : '0') << ' ';
   }
  }
  cout << endl;
 }
#endif
};

const int FullTransform::hms = cws * bits;
#endif //FULLTRANSFORM_HPP
