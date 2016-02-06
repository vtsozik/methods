#include "cegvl.hpp"
#include "lpl.hpp"

EigenValues::EigenValues(const DMatrix& input) {
 int size = input.size();
 input_.reserve(size);
 CDArray cdarr(size);
 for(int i = 0; i < size; ++i) {
  for(int j = 0; j < size; ++j) {
   cdarr[j] = cd(input[i][j], 0.); 
  } 
  input_.push_back(cdarr);
 } 
}

const CDArray& EigenValues::ComputeEigenValues() {
 const int limit = 1000;
 CDMatrix trngl; 
 eigvls_.erase(eigvls_.begin(), eigvls_.end());
 reigvls_.erase(reigvls_.begin(), reigvls_.end());
 cd shift, last;
 CDMatrix seg = input_;
 for(int i = 0; i < input_.size() - 1; ++i) {
int count = 0;
  do {
   GetWilkinsonShift(seg, shift);
   last = seg[seg.size() - 1][seg.size() - 1];
   ShiftMatrix(seg, shift);
   QR qr(seg);
   qr.ComputeQRMatrices();
   seg = qr.GetQ();
   trngl = qr.GetR();
   qr.Multiply(seg, trngl);
   ShiftMatrix(seg, shift * mone);
cout << "Convergence for i = " << i <<  ", count=" << count << " is " << abs(shift - seg[seg.size() - 1][seg.size() - 1]) << " ,shift " << shift << ", remainder " << seg[seg.size() - 1][seg.size() - 1] << endl;
  } while(abs(last - seg[seg.size() - 1][seg.size() - 1]) > 0. && ++count <= limit);
  eigvls_.push_back(last);
  reigvls_.push_back(last.real());
  TrimMatrix(seg);
  if(count > limit) {
cout << "Complex eigen values?" << endl;
exit(1);
   break;
  }
 }
 eigvls_.push_back(seg[seg.size() - 1][seg.size() - 1]);
 reigvls_.push_back(seg[seg.size() - 1][seg.size() - 1].real());
 sort(eigvls_.begin(), eigvls_.end(), cless());
 sort(reigvls_.begin(), reigvls_.end());
cout << endl << "Eigen values: ";
for(int i = 0; i < eigvls_.size(); ++i) {
cout << eigvls_[i] << " ";
}
cout << endl;
 return eigvls_;
}

void EigenValues::GetWilkinsonShift(const CDMatrix& seg, cd& shift) {
 int size = seg.size();
 cd mb = (seg[size - 2][size - 2] + seg[size - 1][size - 1]) / two,
  c = seg[size - 2][size - 2] * seg[size - 1][size - 1] - seg[size - 2][size - 1] * seg[size - 1][size - 2],
  d = mb * mb - c, l1, l2;
 d = sqrt(d); 
 l1 = mb - d;
 l2 = mb + d;
 shift = ((abs(l1 - seg[size - 1][size - 1]) > abs(l2 - seg[size - 1][size - 1])) ? l2 : l1); 
}

void EigenValues::TrimMatrix(CDMatrix &mtotrim) {
 for(int i = 0; i < mtotrim.size() - 1; ++i) {
  mtotrim[i].pop_back();  
 }
 mtotrim.pop_back();
}

void EigenValues::ShiftMatrix(CDMatrix &mtoshift, const cd& shift) {
 for(int i = 0; i < mtoshift.size(); ++i) {
  mtoshift[i][i] -= shift;
 }
}

double EigenValues::GetDeterminant() {
 cd prod = one;
 for(int i = 0; i < eigvls_.size(); ++i) {
  prod *= eigvls_[i];
 }
cout << "Matrix determinant (diagonalized): " << prod << endl;
 return prod.real();
}

#if 0

#include <fstream>
#include <sstream>

int main() {
 DMatrix dmat;
#if 1
 ifstream ifs("data.dat");
 string line;
 while(getline(ifs, line)) {
  DArray darr;
  double val;
  stringstream iss(line);
  while(iss >> val) {
   darr.push_back(val);
  }
  dmat.push_back(darr);
 }
#else
 DArray darr;
 darr.push_back(2.);
 darr.push_back(7.);
 darr.push_back(1.);
 dmat.push_back(darr);
 darr[0] = 3.;
 darr[1] = 4.;
 darr[2] = 8.;
 dmat.push_back(darr);
 darr[0] = 4.;
 darr[1] = 3.;
 darr[2] = 4.;
 dmat.push_back(darr);
#endif
 EigenValues egv(dmat);
 egv.ComputeEigenValues();
 egv.GetDeterminant();
 Laplacian lpl(dmat);
 cout << "Matrix determinant (original): " << lpl.GetDeterminant() << endl << endl;
 return 0;
}

#endif
