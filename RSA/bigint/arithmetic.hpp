#ifndef ARITHMETIC_HPP
#define ARITHMETIC_HPP

#include "bigint.hpp"

class arithmetic {

public:

 const bigint one;
 const int bitsize;

 arithmetic() : one(1, (numtype)1), bitsize(BITS * sizeof(numtype)) { } 

 inline int compare(const bigint &b1, const bigint &b2) const {
  for(int i = bigint::size - 1; i >= 0; --i) {
   if(b1.num[i] > b2.num[i]) {
    return 1;
   } else if(b1.num[i] < b2.num[i]) {
    return -1;
   }
  }
  return 0;
 }

 inline int compare(const bigint &b1, const bigint &b2, int end1, int end2) const {
  for(int i = end1 - 1, j = end2 - 1; i >= 0 && j >= 0; --i, --j) {
   if(b1.num[i] > b2.num[j]) {
    return 1;
   } else if(b1.num[i] < b2.num[j]) {
    return -1;
   }
  }
  return 0;
 }

 inline bool add(const bigint &b1, const bigint &b2, bigint &r) const {
  unsigned acc = 0;
  for(int i = 0; i < bigint::size; ++i) {
   acc += (b1.num[i] + b2.num[i]); 
   r.num[i] = acc % bigint::word;
   acc /= bigint::word;
  }
  return (acc == 0);
 }

 inline bool subtract(const bigint &b1, const bigint &b2, bigint &r) const {
  unsigned acc = 0;
  for(int i = 0; i < bigint::size; ++i) {
   acc = (bigint::word + b1.num[i] - b2.num[i] - acc);
   r.num[i] = acc % bigint::word;
   acc = 1 - acc / bigint::word;
  }
  return (acc == 0);
 }

 inline bool multiply_by_num(const bigint &b1, int c1,
  numtype w, int wp, bigint &r) const {
  int end = c1 + wp;
  if(end <= bigint::size) {
   unsigned acc = 0;
   for(int i = 0; i < c1; ++i) {
    acc += b1.num[i] * w;
    r.num[wp + i] = acc % bigint::word;
    acc /= bigint::word;
   } 
   if(acc > 0 && end < bigint::size) {
    r.num[end] = acc;
    return true;
   } 
   return (acc == 0);
  }
  return false;
 }

 inline bool multiply(const bigint &b1, const bigint &b2, bigint &r) const {
  bool scs = true;
  int c1 = b1.capacity(); 
  if(c1 > 0) {
   int c2 = b2.capacity();
   scs = (c1 + c2 - 1) <= bigint::size;
   for(int i = 0; scs && i < c2; ++i) {
    bigint tr; 
    scs = multiply_by_num(b1, c1, b2.num[i], i, tr) 
     && add(r, tr, r);
   }
  }
  return scs;
 }

//#define DEBUG_DIVIDE
#define SHIFT_DIVIDE
#ifndef SHIFT_DIVIDE
 inline bool divide(const bigint &b1, const bigint &b2, bigint &q, bigint &r) const {
  int bc2 = b2.capacity(), rc, offs;
  unsigned numer, denom;
  numtype min, mean, max;
  bool scs;
  bigint tr1(0), tr2(0), tr3(0); 
  if((scs = (bc2 > 0))) {
   r = b1;
   denom = b2.num[bc2 - 1];
  }
  while(scs && compare(r, b2) >= 0) {
   rc = r.capacity();
   numer = r.num[rc - 1];
   offs = rc - bc2;
   if(compare(r, b2, rc, bc2) < 0) {
    numer = numer * bigint::word + r.num[rc - 2];
    --offs;
   }
#ifdef DEBUG_DIVIDE
cout << "offs = " << offs << " numer = " << numer << " denom = " << denom << endl;
#endif
   min = numer / (denom + 1); 
   max = (numer + 1) / denom; 
   if(max < min) {
    max = bigint::max;
   }
   do {
    mean = (min + max) / 2;
#ifdef DEBUG_DIVIDE
cout << "min = " << min << " mean = " << mean << " max = " << max << endl;
#endif
    tr1.init();
    if((scs = multiply_by_num(b2, bc2, mean, offs, tr1))) {
#ifdef DEBUG_DIVIDE
cout << "r = ";
r.print();
cout << "tr1 = ";
tr1.print();
#endif
     if(compare(r, tr1) >= 0) {
      tr2.init();
      if((scs = subtract(r, tr1, tr2))) { 
       tr3.init();
       if((scs = multiply_by_num(b2, bc2, 1, offs, tr3))) {   
#ifdef DEBUG_DIVIDE
cout << "b2 = ";
b2.print();
cout << "tr3 = ";
tr3.print();
cout << "tr2 = ";
tr2.print();
#endif
        if(compare(tr2, tr3) < 0) {
         q.num[offs] = mean;
         r = tr2;
#ifdef DEBUG_DIVIDE
cout << "BREAK" << endl;
#endif
         break; 
        } else {
#ifdef DEBUG_DIVIDE
cout << "MORE" << endl;
#endif
         min = (max - min > 2) ? mean : max;
        }  
       }
      } 
     } else {
#ifdef DEBUG_DIVIDE
cout << "LESS" << endl;
#endif
      max = mean; 
     }
    }
   } while(scs);
  }
  return scs; 
 }
#endif //SHIFT_DIVIDE

 struct shift {
  int bytes, bits;
 };

 inline bool dwnshft(bigint &b, int &bc, const shift &shs, bigint &r) const {
  int i, nbc = bc - shs.bytes, upbshft = bitsize - shs.bits;
  bool scs = upbshft > 0 && nbc >= 0 && shs.bits >= 0;
  if(scs) {
   if(shs.bytes > 0) {
    for(i = 0; i < shs.bytes; i++) {
     r.num[i] = b.num[i]; 
    }
    for(i = shs.bytes; i < bc; i++) {
     b.num[i - shs.bytes] = b.num[i]; 
    }
    for(i = nbc; i < bc; ++i) {
     b.num[i] = 0;
    }
    bc = nbc;
   }
   if(shs.bits > 0) {
    r.num[shs.bytes] = (b.num[0] << upbshft) >> upbshft;
    nbc = bc - 1;
    for(i = 0; i < nbc; i++) {
     b.num[i] = b.num[i] >> shs.bits; 
     b.num[i] = b.num[i] | (b.num[i + 1] << upbshft);
    }
    b.num[nbc] = b.num[nbc] >> shs.bits; 
    for(; b.num[nbc] == 0; --nbc);
    bc = nbc + 1;
   } 
  }
#ifdef DEBUG_DIVIDE
cout << endl << dec << "dwnshft: bc " << bc << " bigint::size " << bigint::size << " shs.bits " << shs.bits << " scs " << scs << endl;
#endif
  return scs;
 }

 inline bool upshft(bigint &b, int &bc, const shift &shs) const {
  int i, nbc = bc + shs.bytes, dnbshft = bitsize - shs.bits;
  bool scs = dnbshft > 0 && nbc <= bigint::size && shs.bits >= 0;
  if(scs) {
   if(shs.bytes > 0) {
    for(i = bc - 1; i >= 0; --i) {
     b.num[i + shs.bytes] = b.num[i];
     b.num[i] = 0;
    } 
    bc = nbc;
   }
   if(shs.bits > 0) {
    nbc = bc - 1;
    if(bc < bigint::size) {
     b.num[bc] = b.num[bc] | b.num[nbc] >> dnbshft;
     if(b.num[bc] != 0) {
      ++bc;
     }
    }
    b.num[nbc] = b.num[nbc] << shs.bits;
    for(i = nbc; i >= 1; --i) {
     b.num[i] = b.num[i] | b.num[i - 1] >> dnbshft;
     b.num[i - 1] = b.num[i - 1] << shs.bits;
    }
    nbc = bc - 1;
    for(; b.num[nbc] == 0; --nbc); 
    bc = nbc + 1;
   } 
  }  
#ifdef DEBUG_DIVIDE
cout << endl << dec << "upshft: nbc " << nbc << " bigint::size " << bigint::size << " shs.bits " << shs.bits << " scs " << scs << endl;
#endif
  return scs;
 }

 inline int bitcap(const numtype &num) const {
  int i = bitsize, msk = 1 << (i - 1); 
  for(; i > 0; --i) {
   if((num & msk) != 0) {
    break;   
   } else {
    msk = msk >> 1;
   }
  }
  return i; 
 }
 
 inline bool reduce(bigint &b1, int &b1c, bigint &b2, int &b2c, bigint &r, shift &shs) const {
  bool scs = (b1c > b2c) || (compare(b1, b2) >= 0);
  int msk = 1;
  if(scs) {
   shs.bytes = shs.bits = 0;
   r.init();
   for(int i = 0; i < b2c; ++i) {
    if(b2.num[i] != 0) {
     shs.bytes = i;
     for(int j = 0; j < bitsize; ++j) {
      if((b2.num[i] & msk) != 0) { 
       shs.bits = j;    
       break;
      } else {
       msk = msk << 1;
      }
     }
     break;
    }
   }
   if(shs.bits != 0 || shs.bytes != 0) {
    scs = dwnshft(b2, b2c, shs, r) &&  
     dwnshft(b1, b1c, shs, r);  
   }
  }
  return scs;
 }

 inline bool adjustshift(shift &shs) const {
#ifdef DEBUG_DIVIDE
cout << endl << dec <<  "before: bytes " << shs.bytes << " bits " << shs.bits << endl;
#endif
  if(shs.bits < 0) { 
   --shs.bytes;                                                        
   shs.bits += bitsize;                                     
  } else if(shs.bits >= bitsize) {
   ++shs.bytes;                                                        
   shs.bits -= bitsize;                                     
  } 
#ifdef DEBUG_DIVIDE
cout << endl << dec <<  "after: bytes " << shs.bytes << " bits " << shs.bits << endl;
#endif
  return (shs.bytes >= 0 && shs.bits >= 0 && shs.bytes < bigint::size && shs.bits < bitsize);
 }

#ifdef SHIFT_DIVIDE
//#define SHIFT_DOWN
#ifndef SHIFT_DOWN
 inline bool divide(const bigint &b1, const bigint &b2, bigint &q, bigint &r) const {
  bool scs;
  shift shsr, shsd;
  static const shift shsb = {0, 1};
  bigint tb1(0), tb2(0), tb3(0), tb4(0), tr1, tr2;
  int cmp, tb1c, tb2c = b2.capacity(), tb3c, tb4c;
  if(scs = (tb2c > 0)) {
   cmp = compare(b1, b2);
   if(cmp > 0) {
    tb1 = b1;
    tb1c = tb1.capacity();
    tb2 = b2;
    if(scs = reduce(tb1, tb1c, tb2, tb2c, tr1, shsr)) {    
     if(tb2c == 1 && tb2.num[0] == 1) {
      q = tb1;    
      r.init();
     } else {
      q.init();
      do {
       tb3 = tb1;
       tb4 = tb2;
       tb3c = tb1c;
       tb4c = tb2c;
       shsd.bytes = tb3c - tb4c;
       shsd.bits = bitcap(tb3.num[tb3c - 1]) - bitcap(tb4.num[tb4c - 1]);
       if(scs = (adjustshift(shsd) && upshft(tb4, tb4c, shsd))) {
        if(compare(tb4, tb3) > 0) {
         --shsd.bits;
         scs = adjustshift(shsd) && dwnshft(tb4, tb4c, shsb, tr2);
        }
        if(scs && (scs = subtract(tb3, tb4, tb1))) {
         tb1c = tb1.capacity();
         q.num[shsd.bytes] = q.num[shsd.bytes] | (1 << shsd.bits);         
        }
       }
      } while(scs && compare(tb1, tb2) >= 0);
      if(scs) {
       tb1c = tb1.capacity();
       scs = upshft(tb1, tb1c, shsr) &&
        add(tb1, tr1, r);
      }
     }
    }
   } else if(cmp < 0) {
    q.init();
    r = b1;
   } else {
    q.init();
    r.init();
    q.num[0] = 1;
   }  
  }
  return scs;
 }
#else
 inline bool divide(const bigint &b1, const bigint &b2, bigint &q, bigint &r) const {
  bool scs;
  shift shsr, shsu, shsd;
  static const shift shsb = {0, 1};
  bigint tb1(0), tb2(0), tb3(0), tr1, tr2;
  int cmp, tb1c, tb2c = b2.capacity(), tb3c;
  if(scs = (tb2c > 0)) {
   cmp = compare(b1, b2);
   if(cmp > 0) {
    tb1 = b1;
    tb1c = tb1.capacity();
    tb2 = b2;
    if(scs = reduce(tb1, tb1c, tb2, tb2c, tr1, shsr)) {    
     if(tb2c == 1 && tb2.num[0] == 1) {
      q = tb1;    
      r.init();
     } else {
      q.init();
      shsu.bytes = tb1c - tb2c;
      shsu.bits = bitcap(tb1.num[tb1c - 1]) - bitcap(tb2.num[tb2c - 1]);
      if(scs = (adjustshift(shsu) && upshft(tb2, tb2c, shsu))) {
       if(compare(tb2, tb1) > 0) {
        --shsu.bits;
        scs = adjustshift(shsu) && dwnshft(tb2, tb2c, shsb, tr2);
       }
       if(scs && (scs = subtract(tb1, tb2, tb3))) {
        q.num[shsu.bytes] = q.num[shsu.bytes] | (1 << shsu.bits);         
       }
       while(scs && compare(tb3, b2) >= 0) {
        tb1 = tb3;
        tb1c = tb1.capacity();
        shsd.bytes = tb2c - tb1c;
        shsd.bits = bitcap(tb2.num[tb2c - 1]) - bitcap(tb1.num[tb1c - 1]);
        if(adjustshift(shsd) && dwnshft(tb2, tb2c, shsd, tr2)) {
         if(compare(tb2, tb1) > 0) {
          ++shsd.bits;
          scs = adjustshift(shsd) && dwnshft(tb2, tb2c, shsb, tr2);
         }
         if(scs && (scs = subtract(tb1, tb2, tb3))) {
          shsu.bytes -= shsd.bytes;
          shsu.bits -= shsd.bits;
          if(scs && (scs = adjustshift(shsu))) {
           q.num[shsu.bytes] = q.num[shsu.bytes] | (1 << shsu.bits);         
          }
         }
        }        
       } 
       if(scs) {
        tb3c = tb3.capacity();                                                                                                          
        scs = upshft(tb3, tb3c, shsr) && add(tb3, tr1, r);                                                                                                              
       }    
      }
     } 
    }
   } else if(cmp < 0) {
    q.init();
    r = b1;
   } else {
    q.init();
    r.init();
    q.num[0] = 1;
   }  
  }
  return scs;
 }
#endif //SHIFT_DOWN
#endif //SHIFT_DIVIDE

 inline bool powandmod(const bigint &a, const bigint &e, const bigint &m, bigint &r) const {
  bool scs = (m.capacity() > 0);
  if(scs) {
   int ec = e.capacity(), ws = BITS * sizeof(e.num[0]);
   bigint tr1(a), tr2(0), tr3(0);
   r.num[0] = 1;
   for(int i = 0; scs && i < ec; ++i) {
    for(int j = 0; scs && j < ws; ++j) {
     if(i > 0 || j > 0) {
      tr2.init();
      if((scs = (multiply(tr1, tr1, tr2) && divide(tr2, m, tr3, tr1))) && 
       (compare(tr1, one) == 0)) {
       return scs;
      }
     }
     if(scs && (e.num[i] & (1 << j))) {
      tr2.init();
      if((scs = (multiply(r, tr1, tr2)) && divide(tr2, m, tr3, r))) {
       if(r.capacity() == 0) {
        return scs;
       }
      }
     }
    }
   }
  } 
  return scs;
 }

};

#endif //ARITHMETIC_HPP
