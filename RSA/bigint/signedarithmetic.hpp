#ifndef SIGNEDARITHMETIC_HPP
#define SIGNEDARITHMETIC_HPP

#include "arithmetic.hpp"

class signedarithmetic : public arithmetic {

public:

 //const bigint mone;

 signedarithmetic() /*: mone(1, (unsigned short)1, -1)*/ {}

 int signedcompare(bigint &b1, bigint &b2) const {
  int r = b1.sign * b2.sign;
  if(r < 0) {
   r = (b1.sign > b2.sign) ? 1 : -1;
  } else if(r > 0) {
   r = compare(b1, b2) * b1.sign;
  }
  return r;
 }

 bool signedadd(const bigint &b1, const bigint &b2, bigint &r) const {
  bool scs = false;
  int sign = b1.sign * b2.sign;
  if(sign > 0) {
   r.sign = ((scs = add(b1, b2, r))) ? b1.sign : 0;
  } else if(sign < 0) {
   if(b1.sign > b2.sign) {
    bigint bt(b2);
    bt.sign = 1;
    scs = signedsubtract(b1, bt, r);
   } else {
    bigint bt(b1);
    bt.sign = 1;
    scs = signedsubtract(b2, bt, r);
   }
  } else {
   r.sign = sign;
  }
  return scs;
 }

 bool signedsubtract(const bigint &b1, const bigint &b2, bigint &r) const {
  bool scs = false;
  int sign = b1.sign * b2.sign;
  if(sign < 0) {
   r.sign = ((scs = add(b1, b2, r))) ? ((b1.sign > b2.sign) ? 1 : -1) : 0;
  } else if(sign > 0) {
   if(compare(b1, b2) > 0) {
    r.sign = ((scs = subtract(b1, b2, r))) ? b1.sign : 0;
   } else {
    r.sign = ((scs = subtract(b2, b1, r))) ? b1.sign * -1 : 0;
   }
  } else {
   r.sign = sign;
  }
  return scs;
 }

 bool signedmultiply(const bigint &b1, const bigint &b2, bigint &r) const {
  bool scs = false;
  int sign = r.sign = b1.sign * b2.sign;
  if(r.sign != 0) {
   r.init();
   if((scs = multiply(b1, b2, r))) {
    r.sign = (sign > 0 || r.capacity() > 0) ? sign : 1;
   } else {
    r.sign = 0;
   }
  }
  return scs;
 }

 bool signeddivide(const bigint &b1, const bigint &b2, bigint &q, bigint &r) const {
  bool scs = false;
  int sign = q.sign = r.sign = b1.sign * b2.sign;
  if(sign != 0) {
   q.init();
   r.init();
   if((scs = divide(b1, b2, q, r))) {
    q.sign = (sign > 0 || q.capacity() > 0) ? sign : 1;
    r.sign = (b1.sign > 0 || r.capacity() > 0) ? b1.sign : 1;
   } else {
    q.sign = r.sign = 0;
   }
  }
  return scs;
 }

 bool egcd(const bigint &b1, const bigint &b2, bigint &dr, bigint &r1, bigint &r2) const {
  bool scs;
  bigint dd(0), ppr1(0), ppr2(0), pr1(0), pr2(0), q, r(0), tr1(0), tr2(0);
  if(compare(b1, b2) > 0) {
   dd = b1;
   dr = b2;
   pr1 = one;
   pr2.init();
  } else {
   dd = b2;
   dr = b1;
   pr1.init();
   pr2 = one;
  }
  r1 = pr2;
  r2 = pr1;
  while((scs = divide(dd, dr, q, r)) && r.capacity() > 0) {
   ppr1 = pr1;
   ppr2 = pr2;
   pr1 = r1;
   pr2 = r2;
   tr1.init();
   tr2.init();
   if(!(scs = (signedmultiply(q, pr1, tr1) && signedsubtract(ppr1, tr1, r1) &&
    signedmultiply(q, pr2, tr2) && signedsubtract(ppr2, tr2, r2)))) {
    break; 
   } 
   dd = dr;
   dr = r; 
   q.init();
   r.init();
  } 
  return scs;
 }
}; 

#endif //SIGNEDARITHMETIC_HPP
