#include "lpl.hpp"

Laplacian::Laplacian(const DMatrix &input) : input_(input) {
 int size = input_.size();
 empty_.reserve(size);
 for(int i = 0; i < size; ++i) {
  empty_.push_back(false);
 }
}

double Laplacian::GetDeterminant(int row /* = 0*/, BArray arr /* = BArray()*/) {
 int size = input_.size();
 double sum = 0., mult = 1.;
 int new_row = row + 1;
 if(arr.empty()) {
  arr = empty_;
 }
 for(int i = 0; i < size; ++i) {
  if(!arr[i]) {
   if(new_row < size) {
    if(input_[row][i] != 0.) {
     BArray new_arr = arr;
     new_arr[i] = true;  
     sum += (mult * input_[row][i] * GetDeterminant(new_row, new_arr));
    }
    mult *= -1.;
   } else {
    return input_[row][i];
   }
  }
 }
 return sum;
}
