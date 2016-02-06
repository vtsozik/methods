#include <iostream>
#include <iomanip>
#include <cmath>
#include <vector>
using namespace std;

typedef vector<int> row;
typedef vector<row> matrix;

void printMatrix(const matrix &m, const int p, const int q) {
 int w = log(m[p - 1][q - 1]) / log(10) + 1;
 for(int i(0); i < p; ++i) {
  const row &r = m[i];
  for(int j(0); j < q; ++j) {
   cout << ' ' << setfill('0') << setw(w) << r[j];
  }
  cout << endl;
 }
}

//#define DEBUG_PRINT
//#define NORMAL_PRINT
#if 0 //A lot of code, but works faster for 70 elements in matrix 
bool bsearch(const matrix &m, int si, int ei, int sj, int ej, int f, int &fi, int &fj) {
 int sii(si), eii(ei), sij(sj), eij(ej), sdi, sdj, edi, edj;
 do {
  sdi = (sii + eii) / 2;
  sdj = (sij + eij) / 2;
  if(m[sdi][sdj] < f) {
   edi = (sdi < ei) ? sdi + 1 : sdi;
   edj = (sdj < ej) ? sdj + 1 : sdj;
   if(f < m[edi][edj]) {
#ifdef DEBUG_PRINT
cout << "Found range (" << sdi << ',' << sdj << ')' << endl;
cout << "si = " << si << ", ei = " << ei << endl;
cout << "sj = " << sj << ", ej = " << ej << endl;
#endif
      return bsearch(m, edi, ei, sj, sdj, f, fi, fj)
          || bsearch(m, si, sdi, edj, ej, f, fi, fj);
     } else if(f > m[edi][edj]) {
#ifdef DEBUG_PRINT
cout << "Next inner since m[" << edi << "][" << edj << "] ( = " << m[edi][edj] << ") < " << f << endl;
#endif
      sii = edi;
      sij = edj;
     } else {
      fi = edi;
      fj = edj;
     return true;
    }
  } else if(m[sdi][sdj] > f) {
#ifdef DEBUG_PRINT
cout << "Next since m[" << sdi << "][" << sdj << "] > " << f << endl;
#endif
   eii = sdi;
   eij = sdj;
  } else {
   fi = sdi;
   fj = sdj;
   return true;
  }
 } while(sii < eii || sij < eij);
 return false;
}  
#else // O(n) complexity, where n is size of matrix dimension
bool bsearch(const matrix &m, int si, int ei, int sj, int ej, int f, int &fi, int &fj) {
 int sii(si), eii(ei), sij(sj), eij(ej), i, j, mes, mee;
 do {
  i = (sii + eii) / 2;
  j = (sij + eij) / 2;
  mes = m[i][j];
  if(mes < f) {
   sii = i;
   sij = j;
  } else if(mes > f) {
   eii = i;
   eij = j;
  } else {
   fi = i;
   fj = j;
   return true; 
  }
 } while(eii - sii > 1 || eij - sij > 1);
 mes = m[sii][sij];
 mee = m[eii][eij];
 if(mes < f && f < mee) {
#ifdef DEBUG_PRINT
cout << "Found range (" << sii << ',' << sij << ") - (" << eii << ',' << eij << ')' << endl;
cout << "si = " << si << ", ei = " << ei << endl;
cout << "sj = " << sj << ", ej = " << ej << endl;
#endif
  return bsearch(m, eii, ei, sj, sij, f, fi, fj)
      || bsearch(m, si, sii, eij, ej, f, fi, fj);
 } else if(mes == f) {
  fi = sii;
  fj = sij;
  return true;
 } else if(mee == f) {
  fi = eii;
  fj = eij;
  return true;
 } else {
#ifdef DEBUG_PRINT
cout << "Exit search with m[" << sii << "][" << sij << "] = " << mes << " and m[" << eii << "][" << eij << "] = " << mee << endl;
cout << "si = " << si << ", ei = " << ei << endl;
cout << "sj = " << sj << ", ej = " << ej << endl;
#endif
  return false;
 }
}
#endif

// O(n + m) complexity, where n and m are sizes of matrix dimensions
bool lsearch(const matrix &m, int si, int ei, int sj, int ej, int f, int &fi, int &fj) {
 int i(si), j(ej);
 do {
  if(m[i][j] > f) {
   --j;
  } else if(m[i][j] < f) { 
   ++i;
  } else {
   fi = i;
   fj = j;
   return true; 
  }
 } while(i <= ei && sj <= j);
 return false;
}


int main() {
 int i, j, k, l, p, q, n(5), o(n), v(n + 2), h(n + 3);
 row r(n, 0);
 matrix m(o, r);
 for(i = 0; i < n; ++i) { 
  for(j = 0, k = 1; j < o; ++j, k += h) {
   m[i][j] = k + i * v; 
  }
 }
#if 1
#ifdef NORMAL_PRINT
cout << "Binary search" << endl;
#endif
#define search bsearch
#else
#ifdef NORMAL_PRINT
cout << "Linear search" << endl;
#endif
#define search lsearch
#endif
 for(p = 1; p <= n; ++p) {
  for(q = 1; q <= o; ++q) {
#ifdef NORMAL_PRINT
   cout << "Test for p = " << p << " and q = " << q << endl;
   cout << "Initialized matrix: " << endl;
   printMatrix(m, p, q);
#endif
   for(i = 0; i < p; ++i) { 
    for(j = 0; j < q; ++j) { 
#ifdef NORMAL_PRINT
     cout << "Searching for (" << i << ',' << j << ") element: " << m[i][j] << endl;
#endif
     if(search(m, 0, p - 1, 0, q - 1, m[i][j], k, l)) {
#ifdef NORMAL_PRINT
      cout << "Found element (" << k << ',' << l << ") element: " << m[k][l] << endl;
#endif
     } else {
      cout << "Oops, wasn't able to find searched element!" << endl; 
      return 1;
     }
    }
   }
#if 1
   i = -1;
   j = n * (h + v);
#ifdef NORMAL_PRINT
   cout << "Searching for out of range elements: " << i << " and " << j << endl;
#endif
   if(search(m, 0, p - 1, 0, q - 1, i, k, l)
   || search(m, 0, p - 1, 0, q - 1, j, k, l)) {
    cout << "Opps, found element (" << k << ',' << l << ") element: " << m[k][l] << " that doesn't exist!" << endl;
    return 1;
   } else {
#ifdef NORMAL_PRINT
    cout << "Wasn't able to find elements that don't exist" << endl; 
#endif
   }
#endif
  }
 }
 return 0;
}
