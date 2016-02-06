#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <math.h>
#include <vector>
#include <iostream>
using namespace std;

typedef vector<double> DArray;
typedef vector<DArray> DMatrix;

#define ROUND_ZERO_THRESHOLD 1e-6

inline void PrintArray(const DArray &arr) {
 for(int i = 0; i < arr.size(); ++i) {
  cout << " " << arr[i];
 }
 cout << endl;
}

inline void PrintMatrix(const DMatrix &mat) {
 for(int i = 0; i < mat.size(); ++i) {
  PrintArray(mat[i]);
 }
 cout << endl;
}

inline int SquareMatrix(const DMatrix &mat, DMatrix &res) {
 int rows = mat.size(), size = rows;
 res = mat;
 if(rows > 0) {
  int cols = mat[0].size();
  if(cols > rows) {
   size = cols;
   res.reserve(size);
   DArray row(size, 0.);
   for(int i = rows; i < size; ++i) {
    res.push_back(row); 
   } 
  } else if(rows > cols) {
   for(int i = 0; i < size; ++i) {
    res[i].reserve(size); 
    for(int j = cols; j < size; ++j) {
     res[i].push_back(0.); 
    }
   } 
  }
 }
 return size;
}

inline void TransposeMatrix(const DMatrix &mat, DMatrix &res) {
 int rows = mat.size();
 res.clear();
 if(rows > 0) {
  int cols = mat[0].size();
  res.reserve(cols);
  DArray row(rows);
  for(int i = 0; i < cols; ++i) {
   for(int j = 0; j < rows; ++j) {
    row[j] = mat[j][i]; 
   }
   res.push_back(row);
  }
 } 
}

inline int TransposeAndSquareMatrix(const DMatrix &mat, DMatrix &res) {
 DMatrix transp;
 TransposeMatrix(mat, transp);
 return SquareMatrix(transp, res);
}

inline void MultiplyMatrices(const DMatrix &mat1, const DMatrix &mat2, DMatrix &res) {
 res.clear();
 int rows1 = mat1.size(), rows2 = mat2.size();
 if(rows1 > 0 && rows2 > 0) {
  int cols1 = mat1[0].size(), cols2 = mat2[0].size();
  if(cols1 == rows2 && cols2 > 0) {
   res.reserve(rows1);
   DArray row(cols2);
   for(int i = 0; i < rows1; ++i) {
    for(int j = 0; j < cols2; ++j) {
     row[j] = 0.;
     for(int k = 0; k < rows2; ++k) {
      row[j] += mat1[i][k] * mat2[k][j];
     } 
    }
    res.push_back(row);
   }
  } 
 }
}

inline double RoundToZero(double det) { return (fabs(det) > ROUND_ZERO_THRESHOLD) ? det : 0.; }

inline int IsNAN(const DArray &darr) {
 int i, r = 0, size = darr.size();
 for(i = 0; i < size && r == 0; ++i) {
  r = isnan(darr[i]);
 }
 return r;
}

inline int IsNAN(const DMatrix &dmat) {
 int i, r = 0, size = dmat.size();
 for(i = 0; i < size && r == 0; ++i) {
  r = IsNAN(dmat[i]);
 }
 return r;
}

#endif //MATRIX_HPP
