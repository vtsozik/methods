#include "cn.hpp"
#include <iostream>
using namespace std;

CN::CN() {
 double k = 0.5; // W/(m·K) 
 double c = 1.2e6; // J/(m³·K)
 l = .1;
 ti = 3 * 3600;
 tx0ti0 = 253.15;
 tx0tin = 293.15;
 nti = 100;
 nx = 10;
 a = k / c;
}

void CN::Init(int size) {
 b.reserve(size);
 d.reserve(size);
 t.reserve(size);
 for(int i = 0; i < size; ++i) {
  b.push_back(0.);
  d.push_back(0.);
  t.push_back(tx0ti0 + (tx0tin - tx0ti0) / (double) (size - 1) * (double) i);
 }
}

void CN::Reinit(int size) {
 for(int i = 0; i < size; ++i) {
  b[i] = d[i] = 0.;
  t[i] = tx0ti0 + (tx0tin - tx0ti0) / (double) (size - 1) * (double) i;
 }
}

void CN::TDMA(double dx, double dti) {
 double m, xc = 2 * dx * dx, tic = a * dti, 
  b0on = xc + tic, bn0on = b0on + tic,
  dc0on = xc - tic, dcn0on = dc0on - tic; 
 int i, size = t.size() - 1;
 b[0] = b[size] = b0on;
 d[0] = dc0on * t[0] + tic * t[1];
 d[size] = tic * t[size - 1] + dc0on * t[size];
 for(i = 1; i < size; ++i) {
  b[i] = bn0on;
  d[i] = tic * t[i - 1] + dcn0on * t[i] + tic * t[i + 1];
  m = -tic / b[i - 1];
  b[i] += (m * tic);
  d[i] -= (m * d[i - 1]); 
 }
 m = -tic / b[size - 1];
 b[size] += (m * tic);
 d[size] -= (m * d[size - 1]); 
 t[size] = d[size] / b[size];
 --size;
 for(i = size; i >= 0; --i) {
  t[i] = (d[i] + tic * t[i + 1]) / b[i];
 }
}

void CN::SNCN() {
 double dx = l / (double) nx, dti = ti / (double) nti;
 Init(nx + 1);
 cout << "Numerical solution (Crank-Nicolson):," << endl;
 PT();
 for(int i = 0; i < nti; ++i) {
  TDMA(dx, dti);
  PT();
 }
}

void CN::SA() {
 double dx = l / (double) nx, dti = ti / (double) nti, sc = 1 / l, pisq = M_PIl * M_PIl, piscsq = pisq * sc * sc, la, fs;
 int o = 1000;
 cout << endl << "Analytical solution:," << endl;
 for(int i = 0; i <= nti; ++i) {
  for(int j = 0; j <= nx; ++j) {
   fs = 0.;
   for(int k = 1; k <= o; k += 2) {
    la = ((double) (k * k));
    fs -= (exp(-a * la * piscsq * dti * ((double) i)) * cos(((double) k) * sc * ((double) j) * dx * M_PIl) / la);
   }
   fs *= (4. / pisq * (tx0tin - tx0ti0));
   fs += (tx0tin + tx0ti0) / 2.;
   cout << fs << ",";
  }
  cout << endl;
 }
}

void CN::PT() {
 for(int i = 0; i <= nx; ++i) {
  cout << t[i] << ",";
 }
 cout << endl;
}

void CN::SNFD() {
 double dx = l / (double) nx, dti = ti / (double) nti, co = a * dti / dx / dx;
 int size = nx + 1;
 cout << endl << "Numerical solution (Finite Difference):," << endl;
 Reinit(size);
 PT();
 for(int i = 0; i < nti; ++i) {
  d[0] = t[0] + co * (t[1] - t[0]);
  for(int j = 1; j < nx; ++j) {
   d[j] = t[j] + co * (t[j + 1] - 2 * t[j] + t[j - 1]);
  } 
  d[nx] = t[nx] + co * (t[nx - 1] - t[nx]);
  t = d;
  PT();
 }
}

int main() {
 CN cn;
 cn.SNCN();
 cn.SA();
 cn.SNFD();
 return 0;
}
