#ifndef SLEQ_HPP
#define SLEQ_HPP

#include "cegvl.hpp"

class SLEQ {
 DMatrix left_;
 DArray right_;
 double ComputeDeterminant(const DMatrix&);
public:
 SLEQ(const DMatrix& left, const DArray& right) : left_(left), right_(right) {}
 void GetRoots(DArray &);
};

#endif //SLEQ_HPP
