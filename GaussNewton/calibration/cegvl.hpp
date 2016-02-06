#ifndef EGVL_HPP
#define EGVL_HPP

#include "cqr.hpp"
#include "matrix.hpp"

class EigenValues {
 CDArray eigvls_;
 DArray reigvls_;
 CDMatrix input_;
 void ShiftMatrix(CDMatrix &mtoshift, const cd &shift);
 void TrimMatrix(CDMatrix &mtotrim);
 void GetWilkinsonShift(const CDMatrix& seg, cd& shift);
public:
 EigenValues(const DMatrix& input);
 const CDArray& GetEigenValues() const {return eigvls_;}
 const DArray& GetRealEigenValues() const {return reigvls_;}
 const CDArray& ComputeEigenValues(); 
 double GetDeterminant();
};

#endif //EGVL_HPP
