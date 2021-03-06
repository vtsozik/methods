#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <device_functions.h>
#include <cutil_inline.h>

//#define CUDAPRINT
#define SHRD 

#ifdef CUDAPRINT
#include <cuPrintf.cu>
#endif

#include "eratosthenes.hpp"

typedef unsigned short numtype;

//#define LENGTH 4096
//#define LENGTH 2048
//#define LENGTH 1024
#define LENGTH 512
#define BITS 8
#define BITSIZE __umul24(8, sizeof(numtype))
#define SIZE (LENGTH / BITS / sizeof(numtype))
#define WORDD (1 << (__umul24(sizeof(numtype), BITS)))
#define WORDH (1 << (sizeof(numtype) * BITS))
#define BIMAX (WORDD - 1)

#define NOT_FOUND -1

typedef struct __align__(sizeof(numtype)) {
 numtype num[SIZE];
 numtype sign;
} bigint;

__constant__ bigint one;
//__constant__ bigint two;

__device__ void init(bigint *b, int value = 0, int sgn = 1, int end = SIZE, int start = 0) {
 SHRD int i, size = SIZE;
 i = start;
 for(; i < end; ++i) {
  b->num[i] = value;
 }
 for(i = end; i < size; ++i) {
  b->num[i] = 0;
 }
 b->sign = sgn;
}

__device__ int compare(const bigint *b1, const bigint *b2) {
 SHRD int i;
 for(i = SIZE; i >= 0; --i) {
  if(b1->num[i] > b2->num[i]) {
   return 1;
  } else if(b1->num[i] < b2->num[i]) {
   return -1;
  }
 }
 return 0;
}

__device__ int compare(const bigint *b1, const bigint *b2, int end1, int end2) {
 SHRD int i, j;
 for(i = end1 - 1, j = end2 - 1; i >= 0 && j >= 0; --i, --j) {
  if(b1->num[i] > b2->num[j]) {
   return 1;
  } else if(b1->num[i] < b2->num[j]) {
   return -1;
  }
 }
 return 0;
}

__device__ int capacity(const bigint *b) {
 SHRD int i;
 i = SIZE - 1;
 for(; i >= 0 && b->num[i] == 0; --i);
 return i + 1;
}

__device__ void fixdiv(unsigned *n, 
 unsigned short *q, unsigned short *r) {
 const int bitsize = BITSIZE; 
 const unsigned max = 1 << bitsize;
 *q = *n >> bitsize;
 *r = *n - *q * max;
}

__device__ bool add(const bigint *b1, const bigint *b2, bigint *r) {
 SHRD int i, size = SIZE;
 SHRD unsigned acc;
 unsigned short ta;
 acc = 0;
 for(i = 0; i < size; ++i) {
  acc += (b1->num[i] + b2->num[i]); 
  fixdiv(&acc, &ta, &(r->num[i]));
  acc = ta;
/*
  r->num[i] = acc % WORDD;
  acc /= WORDD;
*/
 }
 return (acc == 0);
}

__device__ bool subtract(const bigint *b1, const bigint *b2, bigint *r) {
 SHRD int i, size = SIZE;
 SHRD unsigned acc;
 unsigned short ta;
 acc = 0;
 for(i = 0; i < size; ++i) {
  acc = (WORDD + b1->num[i] - b2->num[i] - acc);
  fixdiv(&acc, &ta, &(r->num[i]));
  acc = 1 - ta;
/*
  r->num[i] = acc % WORDD;
  acc = 1 - acc / WORDD;
*/
 }
 return (acc == 0);
}

__device__ bool multiply_by_num(const bigint *b1, int c1,
 numtype w, int wp, bigint *r) {
 SHRD int end, size = SIZE;
 unsigned short ta;
 end = c1 + wp;
 if(end <= size) {
  int i;
  unsigned acc;
  acc = 0;
  for(i = 0; i < c1; ++i) {
   acc += __umul24(b1->num[i], w);
   fixdiv(&acc, &ta, &(r->num[wp + i]));
   acc = ta;
/*
   r->num[wp + i] = acc % WORDD;
   acc /= WORDD;
*/
  } 
  if(acc > 0 && end < size) {
   r->num[end] = acc;
   return true;
  } 
  return (acc == 0);
 }
 return false;
}

__device__ bool multiply(const bigint *b1, const bigint *b2, bigint *r) {
 SHRD bool scs;
 SHRD int c1;
 scs = true;
 c1 = capacity(b1); 
 if(c1 > 0) {
  SHRD int c2;
  c2 = capacity(b2);
  scs = (c1 + c2 - 1) <= SIZE;
  SHRD int i;
  for(i = 0; scs && i < c2; ++i) {
   SHRD bigint tr; 
   init(&tr);
   scs = multiply_by_num(b1, c1, b2->num[i], i, &tr) 
    && add(r, &tr, r);
  }
 }
 return scs;
}

//#define DEBUG_DIVIDE
//#define SHIFT_DIVIDE
#ifndef SHIFT_DIVIDE
__device__ bool divide(const bigint *b1, const bigint *b2, bigint *q, bigint *r) {
 SHRD int bc2, rc, offs;
 SHRD unsigned numer, denom, word;
 SHRD numtype min, mean, max, bimax;
 SHRD bool scs;
 SHRD bigint tr1, tr2, tr3; 
 bc2 = capacity(b2);
 word = WORDD;
 bimax = BIMAX;
 if((scs = (bc2 > 0))) {
  //*r = *b1;
  memcpy(r, b1, sizeof(*r));
  denom = b2->num[bc2 - 1];
 }
 while(scs && compare(r, b2) >= 0) {
  rc = capacity(r);
  numer = r->num[rc - 1];
  offs = rc - bc2;
  if(compare(r, b2, rc, bc2) < 0) {
   numer = __umul24(numer, word) + r->num[rc - 2];
   --offs;
  }
#ifdef DEBUG_DIVIDE
cout << "offs = " << offs << " numer = " << numer << " denom = " << denom << endl;
#endif
  min = numer / (denom + 1); 
  max = (numer + 1) / denom; 
  if(max < min) {
   max = bimax;
  }
  do {
   mean = (min + max) / 2;
#ifdef DEBUG_DIVIDE
cout << "min = " << min << " mean = " << mean << " max = " << max << endl;
#endif
   init(&tr1);
   if((scs = multiply_by_num(b2, bc2, mean, offs, &tr1))) {
#ifdef DEBUG_DIVIDE
cout << "r = ";
r->print();
cout << "tr1 = ";
tr1->print();
#endif
    if(compare(r, &tr1) >= 0) {
     init(&tr2);
     if((scs = subtract(r, &tr1, &tr2))) { 
      init(&tr3);
      if((scs = multiply_by_num(b2, bc2, 1, offs, &tr3))) {   
#ifdef DEBUG_DIVIDE
cout << "b2 = ";
b2->print();
cout << "tr3 = ";
tr3->print();
cout << "tr2 = ";
tr2->print();
#endif
       if(compare(&tr2, &tr3) < 0) {
        q->num[offs] = mean;
        //*r = tr2;
        memcpy(r, &tr2, sizeof(*r));
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
#endif

 typedef struct __align__(sizeof(int)) {
  int bytes, bits;
 } shift;

 __device__ bool dwnshft(bigint *b, int *bc, const shift *shs, bigint *r) {
  int i, nbc = *bc - shs->bytes, upbshft = BITSIZE - shs->bits;
  bool scs = upbshft > 0 && nbc >= 0 && shs->bits >= 0;
  if(scs) {
   if(shs->bytes > 0) {
    for(i = 0; i < shs->bytes; i++) {
     r->num[i] = b->num[i]; 
    }
    for(i = shs->bytes; i < *bc; i++) {
     b->num[i - shs->bytes] = b->num[i]; 
    }
    for(i = nbc; i < *bc; ++i) {
     b->num[i] = 0;
    }
    *bc = nbc;
   }
   if(shs->bits > 0) {
    r->num[shs->bytes] = (b->num[0] << upbshft) >> upbshft;
    nbc = *bc - 1;
    for(i = 0; i < nbc; i++) {
     b->num[i] = b->num[i] >> shs->bits; 
     b->num[i] = b->num[i] | (b->num[i + 1] << upbshft);
    }
    b->num[nbc] = b->num[nbc] >> shs->bits; 
    for(; b->num[nbc] == 0; --nbc);
    *bc = nbc + 1;
   } 
  }
#ifdef DEBUG_DIVIDE
cout << endl << dec << "dwnshft: bc " << *bc << " SIZE " << SIZE << " shs->bits " << shs->bits << " scs " << scs << endl;
#endif
  return scs;
 }

 __device__ bool upshft(bigint *b, int *bc, const shift *shs) {
  int i, nbc = *bc + shs->bytes, dnbshft = BITSIZE - shs->bits;
  bool scs = dnbshft > 0 && nbc <= SIZE && shs->bits >= 0;
  if(scs) {
   if(shs->bytes > 0) {
    for(i = *bc - 1; i >= 0; --i) {
     b->num[i + shs->bytes] = b->num[i];
     b->num[i] = 0;
    } 
    *bc = nbc;
   }
   if(shs->bits > 0) {
    nbc = *bc - 1;
    if(*bc < SIZE) {
     b->num[*bc] = b->num[*bc] | b->num[nbc] >> dnbshft;
     if(b->num[*bc] != 0) {
      ++(*bc);
     }
    }
    b->num[nbc] = b->num[nbc] << shs->bits;
    for(i = nbc; i >= 1; --i) {
     b->num[i] = b->num[i] | b->num[i - 1] >> dnbshft;
     b->num[i - 1] = b->num[i - 1] << shs->bits;
    }
    nbc = *bc - 1;
    for(; b->num[nbc] == 0; --nbc); 
    *bc = nbc + 1;
   } 
  }  
#ifdef DEBUG_DIVIDE
cout << endl << dec << "upshft: nbc " << nbc << " SIZE " << SIZE << " shs->bits " << shs->bits << " scs " << scs << endl;
#endif
  return scs;
 }

 __device__ int bitcap(const numtype *num) {
  int i = BITSIZE, msk = 1 << (i - 1); 
  for(; i > 0; --i) {
   if((*num & msk) != 0) {
    break;   
   } else {
    msk = msk >> 1;
   }
  }
  return i; 
 }
 
 __device__ bool reduce(bigint *b1, int *b1c, bigint *b2, int *b2c, bigint *r, shift *shs) {
  bool scs = (*b1c > *b2c) || (compare(b1, b2) >= 0);
  int msk = 1;
  if(scs) {
   shs->bytes = shs->bits = 0;
   init(r);
   for(int i = 0; i < *b2c; ++i) {
    if(b2->num[i] != 0) {
     shs->bytes = i;
     for(int j = 0; j < BITSIZE; ++i) {
      if((b2->num[i] & msk) != 0) { 
       shs->bits = j;    
       break;
      } else {
       msk = msk << 1;
      }
     }
     break;
    }
   }
   if(shs->bits != 0 || shs->bytes != 0) {
    scs = dwnshft(b2, b2c, shs, r) &&  
     dwnshft(b1, b1c, shs, r);  
   }
  }
  return scs;
 }

 __device__ bool adjustshift(shift *shs) {
#ifdef DEBUG_DIVIDE
cout << endl << dec <<  "before: bytes " << shs->bytes << " bits " << shs->bits << endl;
#endif
  if(shs->bits < 0) { 
   --shs->bytes;                                                        
   shs->bits += BITSIZE;                                     
  }   
#ifdef DEBUG_DIVIDE
cout << endl << dec <<  "after: bytes " << shs->bytes << " bits " << shs->bits << endl;
#endif
  return (shs->bytes >= 0 && shs->bits >= 0);
 }

#ifdef SHIFT_DIVIDE
 __device__ bool divide(const bigint *b1, const bigint *b2, bigint *q, bigint *r) {
  bool scs;
  shift shsr, shsd;
  const shift shsb = {0, 1};
  bigint tb1, tb2, tb3, tb4, tr1, tr2;
  int cmp, tb1c, tb2c = capacity(b2), tb3c, tb4c;
  if(scs = (tb2c > 0)) {
   cmp = compare(b1, b2);
   if(cmp > 0) {
    tb1 = *b1;
    tb1c = capacity(&tb1);
    tb2 = *b2;
    if(scs = reduce(&tb1, &tb1c, &tb2, &tb2c, &tr1, &shsr)) {    
     if(tb2c == 1 && tb2.num[0] == 1) {
      *q = tb1;    
      init(r);
     } else {
      init(q);
      do {
       tb3 = tb1;
       tb4 = tb2;
       tb3c = tb1c;
       tb4c = tb2c;
       shsd.bytes = tb3c - tb4c;
       shsd.bits = bitcap(&(tb3.num[tb3c - 1])) - bitcap(&(tb4.num[tb4c - 1]));
       if(scs = (adjustshift(&shsd) && upshft(&tb4, &tb4c, &shsd))) {
        if(compare(&tb4, &tb3) > 0) {
         --shsd.bits;
         scs = adjustshift(&shsd) && dwnshft(&tb4, &tb4c, &shsb, &tr2);
        }
        if(scs && (scs = subtract(&tb3, &tb4, &tb1))) {
         tb1c = capacity(&tb1);
         q->num[shsd.bytes] = q->num[shsd.bytes] | (1 << shsd.bits);         
        }
       }
      } while(scs && compare(&tb1, &tb2) >= 0);
      if(scs) {
       tb1c = capacity(&tb1);
       scs = upshft(&tb1, &tb1c, &shsr) &&
        add(&tb1, &tr1, r);
      }
     }
    }
   } else if(cmp < 0) {
    init(q);
    *r = *b1;
   } else {
    init(q);
    init(r);
    q->num[0] = 1;
   }  
  }
  return scs;
 }
#endif //SHIFT_DIVIDE

__device__ bool powandmod(const bigint *a, const bigint *e, const bigint *m, bigint *r) {
 SHRD bool scs;
 scs = (capacity(m) > 0);
 if(scs) {
  SHRD int ec, ws, i, j;
  SHRD bigint tr1, tr2, tr3;
  ec = capacity(e), ws = BITSIZE;
  memcpy(&tr1, a, sizeof(tr1));
  r->num[0] = 1;
  for(i = 0; scs && i < ec; ++i) {
   for(j = 0; scs && j < ws; ++j) {
    if(i > 0 || j > 0) {
     init(&tr2);
     if((scs = (multiply(&tr1, &tr1, &tr2) && divide(&tr2, m, &tr3, &tr1))) &&
      (compare(&tr1, &one) == 0)) {
      return scs;
     }
    }
    if(scs && __umul24(e->num[i], (1 << j))) {
     init(&tr2);
     if((scs = (multiply(r, &tr1, &tr2)) && divide(&tr2, m, &tr3, r))) {
      if(capacity(r) == 0) {
       return scs;
      }
     }
    }
   }
  }
 }
 return scs;
}

__global__ void addtest(const bigint *b1, const bigint *b2, bigint *r) {
 init(r);
 if(!add(b1, b2, r)) {
  init(r);
 }
}

__global__ void subtracttest(const bigint *b1, const bigint *b2, bigint *r) {
 init(r);
 if(!subtract(b1, b2, r)) {
  init(r);
 }
}

__global__ void multtest(const bigint *b1, const bigint *b2, bigint *r) {
 init(r);
 if(!multiply(b1, b2, r)) {
  init(r);
 }
}

__global__ void divtest(const bigint *b1, const bigint *b2, bigint *q, bigint *r) {
 init(q);
 init(r);
 if(!divide(b1, b2, q, r)) {
  init(r);
 }
}

__global__ void mtdiv(const bigint *pc, const numtype *sp, int *result) {
  bigint d, q, r;
  int i;
 if(*result == NOT_FOUND) {
  i = blockIdx.x;
  init(&d, sp[i], 1, 1);
  if(!divide(pc, &d, &q, &r)) {
   //atomicCAS(result, NOT_FOUND, NOT_FOUND - 1 - i);
  } else {
   if(capacity(&r) == 0) {
    //atomicCAS(result, NOT_FOUND, i); 
   }
  }
 } 
}

__global__ void powandmodtest(bigint *b, bigint *e, numtype *result) {
 bigint r, m, t;
 numtype i = 1;
 init(&t);
 t.num[0] = 7;
 if(subtract(b, &t, &m)) {
  ++i; // 2
  init(&r);
  if(powandmod(b, e, &m, &r)) {
   ++i; // 3
   if(compare(&r, &m) < 0) {
    ++i; // 4
    if(subtract(b, &one, &m)) {
     ++i; // 5
     init(&r);
     if(powandmod(b, e, &m, &r)) {
      ++i; // 6
      if(compare(&r, &one) == 0) {
       ++i; // 7
       init(b);
       if(multiply(&m, &m, b)) {
        ++i; // 8
        init(&r);
     	if(powandmod(b, e, &m, &r)) {
         ++i; // 9
         if(capacity(&r) == 0) {
          ++i; // 10
         }
        }
       }
      } 
     } 
    }
   }
  }
 }
 *result = i;
}

__global__ void powandmodtest2(bigint *b, bigint *e, bigint *m, bigint *r, bool *scs) {
 SHRD bigint sb, se, sm, sr;
 memcpy(&sb, b, sizeof(sb));
 memcpy(&se, e, sizeof(se));
 memcpy(&sm, m, sizeof(sm));
 init(&sr);
 *scs = powandmod(&sb, &se, &sm, &sr);
 memcpy(r, &sr, sizeof(sr));
}

__global__ void cuprintftest() {
#ifdef CUDAPRINT
 cuPrintf("Value is: %d\n", one.num[0]);
#endif
}

void print(const bigint *b) {
 if(b->sign == 1) {
  printf("%c", '+');
 } else if(b->sign == 0) {
  printf("%c", '-');
 } else {
  printf("%c(%u)", '?', b->sign);
 }
 for(int i = 0; i < SIZE; ++i) {
  printf(" 0x%x", b->num[i]);
 }
 printf("%c", '\n');
}

int scomp(const bigint *b1, const bigint *b2) {
 if(b1->sign > b2->sign) {
  return 1;
 } else if(b1->sign < b2->sign) {
  return -1;
 } else {
  for(int i = SIZE; i >= 0; --i) {
   if(b1->num[i] > b2->num[i]) {
    return 1;
   } else if(b1->num[i] < b2->num[i]) {
    return -1;
   }
  }
  return 0;
 }
}

void inithost(bigint *b, int value = 0, int sgn = 1, int end = SIZE, int start = 0) {
 int i = start;
 for(; i < end; ++i) {
  b->num[i] = value;
 }
 for(i = end; i < SIZE; ++i) {
  b->num[i] = 0;
 }
 b->sign = sgn;
}

void initrandhost(bigint *b, int end = SIZE, int start = 0) {
 int i = start;
 for(; i < end; ++i) {
  b->num[i] = lrand48() % WORDH;
 }
 for(i = end; i < SIZE; ++i) {
  b->num[i] = 0;
 }
 b->sign = 1;
}

void testaddsub() {
 bigint hb1, hb2, hb3, hr, *db1, *db2, *db3, *dr;
 initrandhost(&hb1, SIZE - 1);
 initrandhost(&hb2, SIZE - 1);
 printf("b1: ");
 print(&hb1);
 printf("b2: ");
 print(&hb2);
 cutilSafeCall(cudaMalloc((void**) &db1, sizeof(bigint)));
 cutilSafeCall(cudaMalloc((void**) &db2, sizeof(bigint)));
 cutilSafeCall(cudaMalloc((void**) &db3, sizeof(bigint)));
 cutilSafeCall(cudaMalloc((void**) &dr, sizeof(bigint)));
 cutilSafeCall(cudaMemcpy(db1, &hb1, sizeof(bigint),
                              cudaMemcpyHostToDevice));
 cutilSafeCall(cudaMemcpy(db2, &hb2, sizeof(bigint),
                              cudaMemcpyHostToDevice));
 addtest<<<1, 1>>>(db1, db2, dr);
 cudaThreadSynchronize();
 subtracttest<<<1, 1>>>(dr, db2, db3);
 cudaThreadSynchronize();
 cutilSafeCall(cudaMemcpy(&hb3, db3, sizeof(bigint),
				cudaMemcpyDeviceToHost));
 cutilSafeCall(cudaMemcpy(&hr, dr, sizeof(bigint),
				cudaMemcpyDeviceToHost));
 printf("b1: ");
 print(&hb3);
 printf("b2: ");
 print(&hb2);
 printf("r: ");
 print(&hr);

 printf("Add/Subtract test is completed with error: %d\n", scomp(&hb1, &hb3));
  
 cutilSafeCall(cudaFree(db1));
 cutilSafeCall(cudaFree(db2));
 cutilSafeCall(cudaFree(db3));
 cutilSafeCall(cudaFree(dr));
 cudaThreadExit();
}

void testmultdiv() {
 bigint hb1, hb2, hb3, hq, hr, *db1, *db2, *db3, *dq, *dr;
 initrandhost(&hb1, SIZE / 2);
 initrandhost(&hb2, SIZE / 2);
 printf("b1: ");
 print(&hb1);
 printf("b2: ");
 print(&hb2);
 cutilSafeCall(cudaMalloc((void**) &db1, sizeof(bigint)));
 cutilSafeCall(cudaMalloc((void**) &db2, sizeof(bigint)));
 cutilSafeCall(cudaMalloc((void**) &db3, sizeof(bigint)));
 cutilSafeCall(cudaMalloc((void**) &dq, sizeof(bigint)));
 cutilSafeCall(cudaMalloc((void**) &dr, sizeof(bigint)));
 cutilSafeCall(cudaMemcpy(db1, &hb1, sizeof(bigint),
                              cudaMemcpyHostToDevice));
 cutilSafeCall(cudaMemcpy(db2, &hb2, sizeof(bigint),
                              cudaMemcpyHostToDevice));
 multtest<<<1, 1>>>(db1, db2, dr);
 cudaThreadSynchronize();
 divtest<<<1, 1>>>(dr, db2, db3, dq);
 cudaThreadSynchronize();
 cutilSafeCall(cudaMemcpy(&hb3, db3, sizeof(bigint),
				cudaMemcpyDeviceToHost));
 cutilSafeCall(cudaMemcpy(&hq, dq, sizeof(bigint),
				cudaMemcpyDeviceToHost));
 cutilSafeCall(cudaMemcpy(&hr, dr, sizeof(bigint),
				cudaMemcpyDeviceToHost));
 printf("b1: ");
 print(&hb3);
 printf("b2: ");
 print(&hb2);
 printf("r: ");
 print(&hr);
 printf("reminder: ");
 print(&hq);

 printf("Multiply/Divide test is completed with error: %d\n", scomp(&hb1, &hb3));
  
 cutilSafeCall(cudaFree(db1));
 cutilSafeCall(cudaFree(db2));
 cutilSafeCall(cudaFree(db3));
 cutilSafeCall(cudaFree(dq));
 cutilSafeCall(cudaFree(dr));
 cudaThreadExit();
}

void testmtdiv() {
 bigint hpc, *dpc;
 numtype *dpd;
 //eratosthenes ert(1304);
 eratosthenes ert(65535);
 int hr, *dr, nd = ert.getprimes().size(), bs = sizeof(numtype) * nd;
 cutilSafeCall(cudaMalloc((void**) &dpc, sizeof(bigint)));
 cutilSafeCall(cudaMalloc((void**) &dpd, bs));
 cutilSafeCall(cudaMalloc((void**) &dr, sizeof(int)));
 cutilSafeCall(cudaMemcpy(dpd, &(ert.getprimes()[0]), bs, cudaMemcpyHostToDevice));
 printf("# of dividers equal to %d\n", nd);
 do {
  hr = NOT_FOUND; 
  initrandhost(&hpc, SIZE / 2);
  cutilSafeCall(cudaMemcpy(dpc, &hpc, sizeof(bigint), cudaMemcpyHostToDevice));
  cutilSafeCall(cudaMemcpy(dr, &hr, sizeof(int), cudaMemcpyHostToDevice));
  mtdiv<<<nd, 1>>>(dpc, dpd, dr);
  cudaThreadSynchronize();
  cutilSafeCall(cudaMemcpy(&hr, dr, sizeof(int), cudaMemcpyDeviceToHost));
  printf("hr = %d\n", hr);
 } while(hr != NOT_FOUND);
 printf("hpc: ");
 print(&hpc);
 cutilSafeCall(cudaFree(dpc));
 cutilSafeCall(cudaFree(dpd));
 cutilSafeCall(cudaFree(dr));
 cudaThreadExit();
}

void testpowandmod() {
 bigint *db, *de, hb, he, hone;
 numtype *dr, hr;
 initrandhost(&hb, SIZE / 2); 
 initrandhost(&he); 
 inithost(&hone, 1, 1, 1, 0);
 cutilSafeCall(cudaMalloc((void**) &db, sizeof(bigint)));
 cutilSafeCall(cudaMalloc((void**) &de, sizeof(bigint)));
 cutilSafeCall(cudaMalloc((void**) &dr, sizeof(numtype)));
 cutilSafeCall(cudaMemcpyToSymbol(one, &hone, sizeof(hone)));
 cutilSafeCall(cudaMemcpy(db, &hb, sizeof(bigint), cudaMemcpyHostToDevice));
 cutilSafeCall(cudaMemcpy(de, &he, sizeof(bigint), cudaMemcpyHostToDevice));
#ifdef CUDAPRINT
 cutilSafeCall(cudaPrintfInit());
#endif
 powandmodtest<<<1, 1>>>(db, de, dr);
 cudaThreadSynchronize();
 cutilSafeCall(cudaMemcpy(&hr, dr, sizeof(numtype), cudaMemcpyDeviceToHost));
 printf("10 ?=? %d\n", hr);
#ifdef CUDAPRINT
 cutilSafeCall(cudaPrintfDisplay(stdout, true));
 cudaPrintfEnd();
#endif
 cutilSafeCall(cudaFree(db));
 cutilSafeCall(cudaFree(de));
 cutilSafeCall(cudaFree(dr));
 cudaThreadExit();
}

void testpowandmod2() {
 bigint *db, *de, *dm, *dr, hb, he, hm, hr, hone;
 bool *dscs, hscs;
 initrandhost(&hb, SIZE / 2);
 initrandhost(&he); 
 initrandhost(&hm, SIZE / 2); 
 inithost(&hone, 1, 1, 1, 0);
 cutilSafeCall(cudaMalloc((void**) &db, sizeof(bigint)));
 cutilSafeCall(cudaMalloc((void**) &de, sizeof(bigint)));
 cutilSafeCall(cudaMalloc((void**) &dm, sizeof(bigint)));
 cutilSafeCall(cudaMalloc((void**) &dr, sizeof(bigint)));
 cutilSafeCall(cudaMalloc((void**) &dscs, sizeof(bool)));
 cutilSafeCall(cudaMemcpyToSymbol(one, &hone, sizeof(hone)));
 cutilSafeCall(cudaMemcpy(db, &hb, sizeof(bigint), cudaMemcpyHostToDevice));
 cutilSafeCall(cudaMemcpy(de, &he, sizeof(bigint), cudaMemcpyHostToDevice));
 cutilSafeCall(cudaMemcpy(dm, &hm, sizeof(bigint), cudaMemcpyHostToDevice));
#ifdef CUDAPRINT
 cutilSafeCall(cudaPrintfInit());
#endif
 powandmodtest2<<<1, 1>>>(db, de, dm, dr, dscs);
 cudaThreadSynchronize();
 cutilSafeCall(cudaMemcpy(&hr, dr, sizeof(bigint), cudaMemcpyDeviceToHost));
 cutilSafeCall(cudaMemcpy(&hscs, dscs, sizeof(bool), cudaMemcpyDeviceToHost));
 printf("scs = %d\n", hscs);
 printf("hr: ");
 print(&hr);
#ifdef CUDAPRINT
 cutilSafeCall(cudaPrintfDisplay(stdout, true));
 cudaPrintfEnd();
#endif
 cutilSafeCall(cudaFree(db));
 cutilSafeCall(cudaFree(de));
 cutilSafeCall(cudaFree(dm));
 cutilSafeCall(cudaFree(dr));
 cutilSafeCall(cudaFree(dscs));
 cudaThreadExit();
}

void testcuprintf() {
 bigint hone;
 inithost(&hone, 1, 1, 1, 0);
 printf("hone: ");
 print(&hone);
 cutilSafeCall(cudaMemcpyToSymbol(one, &hone, sizeof(hone)));
#ifdef CUDAPRINT
 cutilSafeCall(cudaPrintfInit());
#endif
 cuprintftest<<<1, 1>>>();
 cudaThreadSynchronize();
#ifdef CUDAPRINT
 cutilSafeCall(cudaPrintfDisplay(stdout, true));
 cudaPrintfEnd();
#endif
 cudaThreadExit();
}

int main() {
 srand48(time(NULL));
 //testaddsub();
 //testmultdiv();
 //testmtdiv();
 //testpowandmod();
 testpowandmod2();
 //testcuprintf();
 return 0;
}

#if 0
// setup execution parameters
   dim3 threads(BLOCK_SIZE, BLOCK_SIZE);
   dim3 grid(uiWC / threads.x, uiHC / threads.y);

   // kernel warmup
   matrixMul<<< grid, threads >>>(d_C, d_A, d_B, uiWA, uiWB);
   cudaThreadSynchronize();

__constant__ float constData[256];
float data[256];
cudaMemcpyToSymbol(constData, data, sizeof(data));
cudaMemcpyFromSymbol(data, constData, sizeof(data));

#endif
