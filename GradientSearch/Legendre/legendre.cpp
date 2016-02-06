#include "legendre.hpp"
#include "cegvl.hpp"

Legendre::Legendre(double ll, double ul, int nn) : ll(ll), ul(ul), nn(nn) {
 CalculateIntegrals();
 Create();
}

void Legendre::CalculateIntegrals() {
 dIntegrals.clear();
 dIntegrals.reserve(nn);
 for(int i = 0; i < nn; ++i) {
  double p = static_cast<double>(i), us, ls;
  Nom un(1., ul, p), ln;
  us = un.Integrate();
  ln = un;
  ln.x = ll;
  ls = ln.Y();
  dIntegrals.push_back(us - ls);
//cerr << "Integral " << i << ": " << dIntegrals[i] << endl;
 }
//exit(1);
}

void Legendre::Create() {
 int i, j;
 nJacobian.clear();
 nJacobian.reserve(nn);
 nFunction.clear();
 nFunction.reserve(nn);
 for(i = 0; i < nn; ++i) {
  NArray nAF, nAJ;
  nAF.reserve(nn);
  nAJ.reserve(nn / 2);
  for(j = 0; j < nn; ++j) {
   Nom nom;
   nom.pc = (j % 2) ? static_cast<double>(i) : 1.;
   nom.p = (j % 2) ? 1. : static_cast<double>(i);
   if(!(j % 2)) {
    nAF.push_back(nom);
   }
   nom.Differentiate();
   nAJ.push_back(nom); 
  }
  nFunction.push_back(nAF);
  nJacobian.push_back(nAJ);
 }
}

void Legendre::FindPolynomial(DArray &dArray) {
 DMatrix dMatrix;
 dMatrix.reserve(nn);
 dArray.clear();
 dArray.reserve(nn);
//double dbg[6] = { -.7746, .5556, 0., .8889, .7746, .5556 };
 for(int i = 0; i < nn; ++i) {
  double rat = static_cast<double>(i + 1) / static_cast<double>(nn);
  dArray.push_back((i % 2) ? 1. - fabs(dArray[i - 1]) / (ul - ll) * 2. : (ll + (ul - ll) * rat));
  //dArray.push_back(rat);
//dArray.push_back(dbg[i]);
cerr << "Initially " << i << ": " << dArray[i] << endl;
  dMatrix.push_back(dIntegrals);
 }
 do {
  Increment(dMatrix, dArray);
  SolveSytem(dMatrix, dArray);
 } while(ComputeError(dArray) > precision);  
 ExtractPolynomial(dArray);
}

void Legendre::ExtractPolynomial(DArray &dArray) {
 double val;
 int i, k;
 bool needAlloc = (dArray.size() != nn);
 if(needAlloc) {
  dArray.clear();
  dArray.reserve(nn);
 }
 for(i = 0; i < nn; ++i) {
  k = i / 2;
  val = (i % 2) ? nFunction[0][k].c : nFunction[0][k].x;
  if(needAlloc) {
   dArray.push_back(val);
  } else {
   dArray[i] = val;
  }
 } 
}

void Legendre::Increment(DMatrix &dMatrix, DArray &dArray) {
 DArray dTmp = dIntegrals;
 for(int i = 0; i < nn; ++i) {
  for(int j = 0; j < nn; ++j) {
   nJacobian[i][j].c += ((j % 2) ? dArray[j - 1] : dArray[j + 1]);
   nJacobian[i][j].x += dArray[j];
   dMatrix[i][j] = nJacobian[i][j].Y();
   if(!(j % 2)) {
    int k = j / 2;
    nFunction[i][k].c = nJacobian[i][j].c;
    nFunction[i][k].x = nJacobian[i][j].x;
//cerr << "i = " << i << " j = " << j << " k = " << k << " x = " << nFunction[i][k].x << " c = " << nFunction[i][k].c << endl;
    dTmp[i] -= nFunction[i][k].Y();
   }
  }
 }
 dArray = dTmp;
}

void Legendre::SolveSytem(DMatrix &dMatrix, DArray &dArray) {
 double det;
 DArray dATmp = dArray;
 det = GetDeterminant(dMatrix);
//cerr << "Determinant: " << det << endl;
 for(int i = 0; i < nn; ++i) {
  DMatrix dMTmp = dMatrix;
  for(int j = 0; j < nn; ++j) {
   dMTmp[j][i] = dATmp[j];
  }
  dArray[i] = GetDeterminant(dMTmp) / det;
//cerr << "Root " << i << ": " << dArray[i] << endl;
 }
}

double Legendre::GetDeterminant(DMatrix &dMatrix) {
 EigenValues eg(dMatrix);
 eg.ComputeEigenValues();
 return eg.GetDeterminant();
}

double Legendre::ComputeError(DArray &dArray) {
 double sum = 0.;
 for(int i = 0; i < nn; ++i) {
  sum += (dArray[i] * dArray[i]);
 } 
cerr << "Error: " << sqrt(sum) << endl;
 return sqrt(sum);
}

int main() {
 Legendre leg(-1., 1., 8);
 DArray darr;
 leg.FindPolynomial(darr);
cerr << "Polynomial:";
PrintArray(darr);
 return 0;
}
