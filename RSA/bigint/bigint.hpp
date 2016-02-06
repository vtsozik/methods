#ifndef BIGINT_HPP
#define BIGINT_HPP

#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <stdlib.h>
#include <math.h>
//#include <omp.h>
using namespace std;

typedef unsigned short numtype;
//#define LENGTH 4096
#define LENGTH 1024
#define BITS 8
#define SIZE (LENGTH / BITS / sizeof(numtype)) 
#define WORD (1 << (sizeof(numtype) * BITS))

struct bigint {
 static const int size = SIZE, word = WORD; 
 static const numtype max = WORD - 1; 
 int sign;
 numtype num[size];
 inline bigint() { init(); }
 inline bigint(int end, bool is_rand = false, int sgn = 1) { 
  if(is_rand) {
   init_rand(0, end, sgn);
  } else {
   init(0, end, sgn);
  }
 }
 inline bigint(int end, numtype value, int sgn = 1) { init(end, value, sgn); } 
 bigint(const string &str) { init(str); }
 bigint(const string &str, int sgn) { init(str, sgn); }
 inline void init(int start = 0, int end = size, int sgn = 1) {
  for(int i = start; i < end; ++i) {
   num[i] = 0;
  }
  sign = sgn;
 } 
 inline void init_rand(int start, int end, int sgn = 1) {
  for(int i = start; i < end; ++i) {
   num[i] = rand() % word;
  }
  if(start < end) {
   int j = end - 1;
   while(num[j] == 0) {
    num[j] = rand() % word;
   }
  }
  init(end, size, sgn); 
 }
 inline void init(int end, numtype value, int sgn = 1) {
  for(int i = 0; i < end; ++i) {
   num[i] = value;
  }
  init(end, size, sgn); 
 }
 void init(const string &str) {
  stringstream ss(str);
  char csgn;
  int isgn;
  ss >> csgn;
  if(csgn == '+') {
   isgn = 1;
  } else if(csgn == '-') {
   isgn = -1;
  } else {
   isgn = 0;
  }
  init(ss, isgn); 
 }
 void init(const string &str, int sgn) {
  stringstream ss(str);
  init(ss, sgn);
 }
 void init(stringstream &ss, int sgn) {
  if(sgn != 0) {
   int i = size - 1, n;
   for(; i >= 0 && ss >> hex >> n && 0 <= n && n <= max; --i) {
    num[i] = n;
   }
   if(i >= 0) {
    sgn = 0;
   }
  }
  sign = sgn;
 }
 inline int capacity() const {
  int i = size - 1;
  for(; i >= 0 && num[i] == 0; --i);
  return i + 1;
 }
 inline void abs() {
  if(sign == -1) {
   sign = 1;
  }
 }
 inline bool isodd() { return (num[0] & 1) == 1; }
 inline bool isdivisibleby3() {
  int sum = 0, div, cap = capacity();
  for(int i = 0; i < cap; ++i) {
   div = 1;
   for(int j = 0; j < 5; ++j) {
    sum += ((num[i] / div) % 10);
    div *= 10;
   }
  }
  return (sum % 3) == 0;
 }
 inline bool isdivisibleby5() { return (num[0] % 5) == 0; }
 void print() const {
  int width = sizeof(numtype) * 2;
  if(sign < 0) {
   cout << '-';
  } else if(sign > 0) {
   cout << '+';
  } else {
   cout << '?';
  }
  for(int i = size - 1; i >= 0; --i) {
   cout << " 0x" << hex << setfill('0') << setw(width) << num[i]; 
  }
  cout << endl;
 }
};

#endif //BIGINT_HPP
