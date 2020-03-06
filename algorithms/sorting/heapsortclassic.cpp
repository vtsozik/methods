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

template <class A, class I> void heapsort(A *a, I s) {
 A t;
 I n = s, parent = s / 2, index, child;
 while(1) {
  if(parent > 0) {
   t = a[--parent];
  } else {
   n--;
   if(n == 0) {
    return;
   }
   t = a[n];
   a[n] = a[0];
   cout << "a[n = " << n << "] = a[0] ";
   print(a, s);
  }
  index = parent;
  child = 2 * index + 1;
  while(child < n) {
   if(child + 1 < n && a[child + 1] > a[child]) {
    ++child;
   } 
   if(a[child] > t) {
    a[index] = a[child];
    cout << "a[index = " << index << "] = a[child = " << child << "] ";
    print(a, s);
    index = child;
    child = index * 2 + 1;
   } else {
    break;
   }
  }
  a[index] = t;
  cout << "a[index = " << index << "] = (t = " << t << ") ";
  print(a, s);
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
 heapsort(a, s);
 cout << "Sorted, ";
 print(a, s);
 return 0; 
}
