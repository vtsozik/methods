#include <iostream>
#include <algorithm>
using namespace std;

template <class A, class I> void print(A *a, I lo, I hi) {
 cout << "Array (size: " << hi - lo + 1 << "):";
 for(I i = lo; i <= hi; ++i) {
  cout << ' ' << a[i];
 }
 cout << endl;
}

template <class A, class I> I partition(A *a, I lo, I hi) {
 A p = a[hi];
 I i = lo;
 cout << "Before partitioning, first at " << lo << ", last at " << hi << ", partition a[" << hi << "] = " << p << ", "; 
 print(a, lo, hi);
 for(I j =  lo; j < hi; ++j) {
  if(a[j] <= p) {
   swap(a[i], a[j]);
   cout << "Swapped a[j = " << i << "] = " << a[i]  << " with a[i = " << j << "] = " << a[j] << ", ";
   print(a, lo, hi);
   ++i;
  }
 }
 swap(a[i], a[hi]);
 cout << "After partitioning, swapped a[partiotion = " << i << "] = " << a[i] << " with a[" << hi << "] = " << a[hi] << ", "; 
 print(a, lo, hi);
 return i;
}

template <class A, class I> void quicksort(A *a, I lo, I hi) {
 if(lo < hi) {
  A p = partition(a, lo, hi);
  quicksort(a, lo, p - 1);
  quicksort(a, p + 1, hi);
 }
}

template <class A, class I> void fill_random(A *a, I lo, I hi) {
 srand(time(NULL));
 I f = 1;
 for(I i = lo, j = 1; i <= hi; ++i, ++j) {
  a[i] = i;
  f *= j;
 }
 I p = static_cast<I>(static_cast<double>(rand()) / RAND_MAX * f);
 for(I i = 0; i < p; ++i) {
  next_permutation(a + lo, a + hi + 1);
 }
}

int main() {
 int a[10], lo = 0, hi = sizeof(a) / sizeof(a[0]) - 1;
 fill_random(a, lo, hi);
 cout << "Initially, ";
 print(a, lo, hi);
 quicksort(a, lo, hi);
 cout << "Sorted, ";
 print(a, lo, hi);
 return 0; 
}
