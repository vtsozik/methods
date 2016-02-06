#ifndef EGVT_HPP
#define EGVT_HPP

#include "sleq.hpp"

class EigenVectors {
 DMatrix input_;
 void InitArray(DArray &darr, int size) const;
 void InitMatrix(DMatrix &dmat, int size) const;
 int TrimSystem(DMatrix &dmat, DArray &darr) const;
 void PopulateSystem(DMatrix &dmat, DArray &darr, const DMatrix &appld, int ind) const;
 double Normalise(DArray &darr) const;
 void FindEigenVector(DMatrix &egvts, double eigvl, int ind) const;
 inline double ComputeSystem(DMatrix &dmat, DArray &darr) const;
public:
 EigenVectors(const DMatrix &input) : input_(input) {}
 void GetEigenVectors(DMatrix &egvts, DArray &egvls) const;
};
#endif //EGVT_HPP
