#ifndef HALFTRANSFORM_HPP
#define HALFTRANSFORM_HPP

#include <stdlib.h>
#include "hmatrix.hpp"

#define DEBUG

class HalfTransform {
 mutable IMatrix hm, nhm, tm;
 HMatrix hmg;
public:
 const static int hms;
 inline HalfTransform() : hm(0), nhm(0), tm(0) {
  hmg.GenHadMat(hms, hm);
  NegateMatrix(hm, nhm);
  tm.reserve(1);
  tm.push_back(IArray(hms, 0));
 }
 inline const IMatrix& GetHad() const { return hm; }
 inline const IMatrix& GetNegHad() const { return nhm; }
 inline short FillChar(const IArray &v) const {
  short o = 0, p = 1;
  int vs = v.size();
  for(int i = 0; i < vs; ++i) {
   if(v[i] > 0) {
    o |= p;
   }
   p *= 2;
  }
#ifdef DEBUG
cout << "Coded array: ";
Print(o, 8);
#endif
  return o;
 }
 inline short CodeHalf(char c) const {
  const static char m = 8;
#ifdef DEBUG
cout << "Half coded word: ";
Print(c, 8);
#endif
  if(c & m) {
   c &= ~m;
   return FillChar(nhm[c]);
  } else {
   return FillChar(hm[c]);
  }
 }
 inline short Code(char c) const {
  const static char l = 15, cshft = 4, sshft = 8;
#ifdef DEBUG
cout << "Coded word: ";
Print(c, 8);
#endif
  short s = CodeHalf((c >> cshft) & l);
#ifdef DEBUG
cout << "Resulted Upper Code: ";
Print(s, 16);
#endif
  s = s << sshft;
#ifdef DEBUG
cout << "Resulted Shifted Code: ";
Print(s, 16);
#endif
  s |= CodeHalf(c & l);
#ifdef DEBUG
cout << "Resulted Code: ";
Print(s, 16);
#endif
  return s;
 }
 inline const IMatrix& ScanChar(short s) const {
  IArray &v = tm[0];
  int vs = v.size();
  short p = 1;
  for(int i = 0; i < vs; ++i) {
   if(s & p) {
    v[i] = 1; 
   } else {
    v[i] = -1; 
   } 
   p *= 2;
  }
  return tm;
 }
 inline char FindChar(const IArray &v) const {
  char c = 0, max;
  int vs = v.size(), tr = vs / 2 + 2;
  for(int i = 0; i < vs; ++i) {
   if(abs(max = v[i]) >= tr) {
    c = i;
    if(max < 0) {
     c |= 8;
    } 
    break;
   }
  }
  return c;
 }
 inline char DecodeHalf(short s) const {
  IMatrix r;
#ifdef DEBUG
cout << "Half word to decode: ";
Print(s, 8);
#endif
  MatrixProduct(ScanChar(s), hm, r);
#ifdef DEBUG
cout << "Product Vector: ";
PrintArray(r[0]);
#endif
  return FindChar(r[0]); 
 }
 inline char Decode(short s) const {
  const static short l = 255, sshft = 8, cshft = 4; 
#ifdef DEBUG
cout << "Word to decode: ";
Print(s, 16);
#endif
  char c = DecodeHalf((s >> sshft) & l);
#ifdef DEBUG
cout << "Decoded Upper Half Word: ";
Print(c, 8);
#endif
  c = c << cshft;
#ifdef DEBUG
cout << "Decoded Shifted Half Word: ";
Print(c, 8);
#endif
  c |= DecodeHalf(s & l); 
#ifdef DEBUG
cout << "Decoded Word: ";
Print(c, 8);
#endif
  return c;
 }
 inline void Print(const int w, const int size) const {
  int p =  1 << (size - 1); 
  for(int i = 0; i < size; ++i) {
   cout << ((w & p) ? '1' : '0') << ' ';
   p /= 2;
  }
  cout << endl;
 }
};

const int HalfTransform::hms = 8;
#endif //HALFTRANSFORM_HPP
