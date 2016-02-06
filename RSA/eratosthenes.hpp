#ifndef ERATOSTHENES
#define ERATOSTHENES

#include <math.h>
#include <vector>
#include <iostream>
using namespace std;

typedef vector<unsigned short> vus;

class eratosthenes {
 vus primes;
public:
 eratosthenes() {}
 eratosthenes(int size) {
  init(size); 
 }
 void init(int size) {
  vus nums;
  primes.clear();
  if(size > 2) {
   nums.reserve(size - 1);
   for(int i = 2; i <= size; ++i) {
    nums.push_back(i);
   }
   primes.reserve(2 * size / log(size - 1));
   size = nums.size();
   for(int i = 0; i < size; ++i) {
    unsigned short num = nums[i];
    if(num != 0) {
     primes.push_back(num);
     for(int j = i + num; j < size; j += num) {
      nums[j] = 0;
     }
    } 
   }
  }
 }
 const vus& getprimes() const { return primes; }
};

#endif //ERATOSTHENES
