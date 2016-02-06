#ifndef EGVL_HPP
#define EGVL_HPP

#include "qr.hpp"

class EigenValues {
 DArray eigvls_;
 DMatrix input_;
 void ShiftMatrix(DMatrix &mtoshift, double shift);
 void TrimMatrix(DMatrix &mtotrim);
public:
 EigenValues(const DMatrix& input) : input_(input) {}
 const DArray& GetEigenValues() const {return eigvls_;}
 const DArray& ComputeEigenValues(); 
 double GetTrace();
 double GetDeterminant();
};

#endif //EGVL_HPP
