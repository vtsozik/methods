#include "egvl.hpp"
#include "lpl.hpp"

const DArray& EigenValues::ComputeEigenValues() {
 const int limit = 1000;
 DMatrix trngl; 
 eigvls_.erase(eigvls_.begin(), eigvls_.end());
 double shift;
 DMatrix seg = input_;
 for(int i = 0; i < input_.size() - 1; ++i) {
int count = 0;
  do {
   shift = seg[seg.size() - 1][seg.size() - 1];
   ShiftMatrix(seg, shift);
   QR qr(seg);
   qr.ComputeQRMatrices();
   seg = qr.GetQ();
   trngl = qr.GetR();
   qr.Multiply(seg, trngl);
   ShiftMatrix(seg, shift * -1.);
cout << "Convergence for i = " << i <<  ", count=" << count << " is "<< fabs(shift - seg[seg.size() - 1][seg.size() - 1]) << endl;
  } while(fabs(shift - seg[seg.size() - 1][seg.size() - 1]) > 0. && ++count <= limit);
  eigvls_.push_back(shift);
  TrimMatrix(seg);
  if(count > limit) {
cout << "Complex eigen values?" << endl;
exit(1);
   break;
  }
 }
 eigvls_.push_back(seg[seg.size() - 1][seg.size() - 1]);
 sort(eigvls_.begin(), eigvls_.end());
cout << endl << "Eigen values: ";
for(int i = 0; i < eigvls_.size(); ++i) {
cout << eigvls_[i] << " ";
}
cout << endl;
 return eigvls_;
}

void EigenValues::TrimMatrix(DMatrix &mtotrim) {
 for(int i = 0; i < mtotrim.size() - 1; ++i) {
  mtotrim[i].pop_back();  
 }
 mtotrim.pop_back();
}

void EigenValues::ShiftMatrix(DMatrix &mtoshift, double shift) {
 for(int i = 0; i < mtoshift.size(); ++i) {
  mtoshift[i][i] -= shift;
 }
}

double EigenValues::GetTrace() {
 double sum = 0.;
 for(int i = 0; i < eigvls_.size(); ++i) {
  sum += eigvls_[i];
 }
cout << "Trace (diagonalized): " << sum << endl;
 return sum;
}

double EigenValues::GetDeterminant() {
 double prod = 1.;
 for(int i = 0; i < eigvls_.size(); ++i) {
  prod *= eigvls_[i];
 }
cout << "Matrix determinant (diagonalized): " << prod << endl;
 return prod;
}

#if 0

#include <fstream>
#include <sstream>

int main() {
 DMatrix dmat;
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
 EigenValues egv(dmat);
 egv.ComputeEigenValues();
#if 0
int s = 2;
double c = 1.;
DMatrix t;
for(int i = 0; i < s; ++i) {
 DArray a;
 for(int j = 0; j < s; ++j) {
  a.push_back(c);
  c += 1.;
 }
 t.push_back(a);
}
Laplacian lpl(t);
#else
Laplacian lpl(dmat);
#endif
egv.GetDeterminant();
cout << "Matrix determinant (original): " << lpl.GetDeterminant() << endl << endl;
 return 0;
}

#endif
