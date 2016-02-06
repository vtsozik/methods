#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <iostream>
#include <vector>
using namespace std;

typedef vector<int> IArray;
typedef vector<IArray> IMatrix;

inline void PrintArray(const IArray &arr) {
 for(int i = 0; i < arr.size(); ++i) {
  cout << '\t' << arr[i];
 }
 cout << endl;
}

inline void PrintMatrix(const IMatrix &mat) {
 for(int i = 0; i < mat.size(); ++i) {
  PrintArray(mat[i]);
 }
 cout << endl;
}

inline void TransposeMatrix(const IMatrix &mat, IMatrix &res) {
 int rows = mat.size();
 res.clear();
 if(rows > 0) {
  int cols = mat[0].size();
  res.reserve(cols);
  IArray row(rows);
  for(int i = 0; i < cols; ++i) {
   for(int j = 0; j < rows; ++j) {
    row[j] = mat[j][i];
   }
   res.push_back(row);
  }
 }
}

inline void MatrixProduct(const IMatrix &mat1, const IMatrix &mat2, IMatrix &res) {
 int rows1 = mat1.size(), rows2 = mat2.size();
 res.clear();
 if(rows1 > 0 && rows2 > 0) {
  int cols1 = mat1[0].size(), cols2 = mat2[0].size();
  if(cols1 == rows2 && cols2 > 0) {
   res.reserve(rows1);
   IArray row(cols2);
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

inline int KronProd(const IMatrix &im1, const IMatrix &im2, IMatrix &om) {
 int sim1 = im1.size(), sim2 = im2.size(), som = sim1 * sim2;
 om.clear();
 om.reserve(som);
 for(int i = 0; i < som; ++i) {
  IArray r(0);
  r.reserve(som);
  for(int j = 0; j < som; ++j) {
   r.push_back(im1[i % sim1][j % sim1] * im2[i / sim1][j / sim1]);
  }
  om.push_back(r);
 }
 return som;
}

inline void NegateMatrix(const IMatrix &im, IMatrix &om) {
 int is = im.size();
 om = im;
 for(int i = 0; i < is; ++i) {
  for(int j = 0; j < is; ++j) {
   om[i][j] *= -1; 
  }
 }
}

#endif //MATRIX_HPP
