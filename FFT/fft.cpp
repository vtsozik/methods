#include <complex>
#include <iostream>
#include <math.h>
using namespace std;

void my_dft(const int n, complex<double> *x) {
 complex<double> y[n], c(2./static_cast<double>(n), 0);
 double d = 2. * M_PIl / static_cast<double>(n), e;
 for(int i = 0; i < n / 2; ++i) {
  y[i] = complex<double>(0, 0);
  for(int j = 0; j < n; ++j) {
   e = d * static_cast<double>(i * j);
   y[i] += (x[j] * complex<double>(cos(e), sin(e)));
  }
  y[i] *= c; 
  x[i] = y[i] * conj(y[i]);
 } 
}

void my_fft(const int n, complex<double> *x) {
 complex<double> y[n], c(2./static_cast<double>(n), 0);
 double d = 2. * M_PIl / static_cast<double>(n), e;
 int i, j, k, l, p = 1;
 for(i = n / 2; i > 0; i /= 2, p *= 2) {
  for(j = 0; j < n; j += (2 * i)) {
   for(k = 0; k < i; ++k) {
    e = d * static_cast<double>(k * p);
    y[j + k] = x[j + k] + x[j + k + i]; 
    y[j + k + i] = (x[j + k] - x[j + k + i]) * complex<double>(cos(e), sin(e));
   }
  }
  for(j = 0; j < n; ++j) { 
   x[j] = y[j];
  }
 }
 for (l = n >> 1, j = 0, i = 0; i < n - 1; i++) {
  if (i < j) {
   swap(x[i], x[j]);
  }
  k = l;
  while (k <= j) {
   j -= k;
   k >>= 1;
  }
  j += k;
 }
 for(i = 0; i < n; ++i) {
  x[i] *= c;
  x[i] = (x[i] * conj(x[i]));
 }
}

#if 0
int main() {
 int m = 13, i, n = (1 << m);
 double step = 2. * M_PIl / (double)n, path, rnd;
 complex<double> x[n], y[n];
 srand(0);
 for(i = 0; i < n; ++i) {
  path = step * static_cast<double>(i); 
  rnd = static_cast<double>(rand()) / static_cast<double>(RAND_MAX);
  //rnd = 0.;
  //path = 0.;
  y[i] = x[i] = sin(path * 50) + sin(path * 120) + rnd;
 }
#if 0
 my_dft(n, y);
#else
 my_fft(n, y);
#endif
 for(i = 1; i < n/2; ++i) {
  cout << static_cast<double>(i) << "," << x[i].real() << "," << y[i].real() << endl;
 }
 return 0;
}

#else

#include <fstream>
#include <vector>
int main() {
 int i, n;
 string str;
 vector<double> vd;
 double max = 0., val;
 ifstream ifs("./data.dat");
 while(getline(ifs, str)) {
  val = atof(str.c_str());
  vd.push_back(val); 
  if(max < val) {
   max = val;
  } 
 }
 ifs.close();
 n = vd.size(); 
 complex<double> cd[n];
 for(i = 0; i < n; ++i) {
  cd[i] = complex<double>(vd[i] / max, 0.);
  //cout << cd[i] << endl;
 }
 my_dft(n, cd);
 for(i = 1; i < n/2; ++i) {
  cout << static_cast<double>(i) << "," << cd[i].real() << endl;
 }
 return 0;
}
#endif
