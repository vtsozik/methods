#include "eratosthenes.hpp"

int main() {
 eratosthenes er(100);
 const vus primes = er.getprimes();
 int size = primes.size(), j;
 for(int i = 0; i < size; ++i) {
  cout << primes[i];
  j = i + 1;
  if(j % 10 == 0 || j >= size) {
   cout << endl;
  } else {
   cout << ", ";
  }
 }
 return 0;
}
