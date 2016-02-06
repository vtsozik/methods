#ifndef SLEQ_HPP
#define SLEQ_HPP

#include "cegvl.hpp"

class SLEQ {
 DMatrix left_;
 DArray right_;
public:
 SLEQ(const DMatrix& left, const DArray& right) : left_(left), right_(right) {}
 double ComputeDeterminant(const DMatrix&) const;
 double ComputeDeterminant() const { return ComputeDeterminant(left_); }
 double GetRoots(DArray &roots) const { return GetRoots(roots, ComputeDeterminant()); }
 double GetRoots(DArray &, double) const;
};

#endif //SLEQ_HPP
