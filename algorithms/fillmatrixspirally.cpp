#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <cmath>
using namespace std;

typedef vector<int> row;
typedef vector<row> matrix;

void printMatrix(const matrix &m) {
 int ms(m.size());
 int w = log(ms * ms) / log(10) + 1;
 for(int i(0); i < ms; ++i) {
  const row &r = m[i];
  int rs(r.size());
  for(int j(0); j < rs; ++j) {
   cout << ' ' << setfill('0') << setw(w) << r[j];
  }
  cout << endl; 
 } 
}

void nextStep(int &dx, int &dy, int &mx) {
 if(dx == mx && dy == mx) {
  ++dy;
  ++mx;
 } else if(dy == mx && dx > -mx) {
   --dx;
 } else if(dx == -mx && dy > -mx) {
   --dy;
 } else if(dy == -mx && dx < mx) {
   ++dx;
 } else if(dx == mx && dy < mx) {
   ++dy;
 }
}

void fillMatrixSpirally(matrix &m) {
 int rs(m.size());
 if(rs > 0) {
  int cs(m[0].size()), x0(cs / 2), y0(rs / 2), ms(cs * rs),
  x(x0), y(y0), dx(0), dy(0), mx(0);
  for(int i(1); i <= ms; ++i) {
   m[y][x] = i;
   nextStep(dx, dy, mx);
   x = x0 + dx;
   y = y0 + dy;
//cout << i << ' ' << dx << ' ' << dy << ' ' << x << ' ' << y << endl;
  }
 }
}

int main() {
 row r(5, 0);
 matrix m(5, r);
 cout << "Initialized matrix: " << endl;
 printMatrix(m);
 fillMatrixSpirally(m);
 cout << "Spirally filled matrix: " << endl;
 printMatrix(m);
 return 0;
}
