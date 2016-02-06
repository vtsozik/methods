#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <vector>
#include <iostream>
using namespace std;

typedef vector<double> DArray;
typedef vector<DArray> DMatrix;

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

#endif //MATRIX_HPP
