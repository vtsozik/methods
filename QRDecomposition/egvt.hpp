#ifndef EGVT_HPP
#define EGVT_HPP

#include "sleq.hpp"

class EigenVectors {
 DMatrix input_;
 void InitArray(DArray &darr, int size);
 void InitMatrix(DMatrix &dmat, int size);
 void PopulateTemplate(DMatrix &dmattmpl, DArray &darrtmpl, double eigvl, int ind);
 double AssumeOrthogonal(DArray &darrtmpl);
 void FindEigenVector(DMatrix &eigvects, double eigvl, int ind, DMatrix &dmattmpl, DArray &darrtmpl);
public:
 EigenVectors(const DMatrix input) : input_(input) {}
 void GetEigenVectors(DMatrix &eigvects);
};
#endif //EGVT_HPP
