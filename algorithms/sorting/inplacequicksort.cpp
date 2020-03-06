#define DEBUG_PRINT
#define MAIN
#ifdef MAIN
#include <iostream>
using namespace std;
#endif


template <class T> void swap(T *r, T *l) {
 T t = *r;
 *r = *l;
 *l = t;
}

template <class T> int partition(T *t, int left, int right, int piv) {
 T pivVal = t[piv];

#ifdef DEBUG_PRINT
for(int j(left); j <= right; ++j) {
 cout << t[j] << ' ';
}
cout << endl;
#endif

 swap(&t[piv], &t[right]);

#ifdef DEBUG_PRINT
for(int j(left); j <= right; ++j) {
 cout << t[j] << ' ';
}
cout << endl;
#endif

 int si = left;
 for(int i(left); i < right; ++i) {
  if(t[i] < pivVal) {
   swap(&t[i], &t[si]);
   ++si;

#ifdef DEBUG_PRINT
for(int j(left); j <= right; ++j) {
 cout << t[j] << ' ';
}
cout << endl;
#endif

  }
 }
 swap(&t[si], &t[right]);

#ifdef DEBUG_PRINT
for(int j(left); j <= right; ++j) {
 cout << t[j] << ' ';
}
cout << "si = " << si << " piv = " << piv << " pivVal = " << pivVal << " left = " << left << " right = " << right << endl;
#endif

 return si; 
}

template <class T> void inplqs(T *t, int left, int right) {
 if(left < right) {
  int piv = (left + right) / 2;
  piv = partition(t, left, right, piv);
  inplqs(t, left, piv - 1);
  inplqs(t, piv + 1, right);
 }
}

#ifdef MAIN
int main() {
 static const int n(10);
 int a[n];
 for(int i(0); i < n; ++i) {
  a[i] = n - i; 
  cout << a[i] << ' ';
 }
 cout << endl;
 inplqs(a, 0, n - 1);
 for(int i(0); i < n; ++i) {
  cout << a[i] << ' ';
 }
 cout << endl;
}
#else
template void inplqs<int>(int *t, int left, int right);
#endif
