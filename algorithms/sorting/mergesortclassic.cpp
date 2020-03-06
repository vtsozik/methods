#include <iostream>
#include <algorithm>
using namespace std;

template <class A, class I> void print(A *a, I s) {
 cout << "Array (size: " << s << "):";
 for(I i = 0; i < s; ++i) {
  cout << ' ' << a[i];
 }
 cout << endl;
}

template <class A, class I> void merge(A *la, I ls, A *ra, I rs, A *a, I s) {
 int li = 0, ri = 0, mi = 0;
 while(li < ls && ri < rs) {
  if(la[li] < ra[ri]) {
   a[mi++] = la[li++];
  } else {
   a[mi++] = ra[ri++];
  }
 }
 while(li < ls) {
  a[mi++] = la[li++];
 } 
 while(ri < rs) {
  a[mi++] = ra[ri++];
 } 
}

template <class A, class I> void merge_sort(A *a, I s) {
 if(s > 0) {
  const I m = s / 2, ls = m + 1, rs = s - ls; 
  A la[ls], ra[rs]; 
  copy(a, a + ls, la);  
  copy(a + ls, a + rs, ra);  
  merge_sort(la, ls);
  merge_sort(ra, rs);
  merge(la, ls, ra, rs, a, s);
 }
}

template <class A, class I> void fill_random(A *a, I s) {
 srand(time(NULL));
 I f = 1;
 for(I i = 0; i < s; ++i) {
  a[i] = i;
  f *= i + 1;
 }
 I p = static_cast<I>(static_cast<double>(rand()) / RAND_MAX * f);
 for(I i = 0; i < p; ++i) {
  next_permutation(a, a + s);
 }
}

int main() {
 int a[10], s = sizeof(a) / sizeof(a[0]);
 fill_random(a, s);
 cout << "Initially, ";
 print(a, s);
 merge_sort(a, s);
 cout << "Sorted, ";
 print(a, s);
 return 0; 
}
