#ifndef SVD_HPP
#define SVD_HPP

#include "matrix.hpp"

class SVD {
public:
 void operator()(const DMatrix &in, DMatrix &u, DMatrix &e, DMatrix &vt) const;
 void ComputeTriProduct(const DMatrix &m1, const DMatrix &m2, const DMatrix &m3, DMatrix &trpr) const;
 void ComputeOriginal(const DMatrix &u, const DMatrix &e, const DMatrix &vt, DMatrix &orig) const;
 void ComputeInverse(const DMatrix &u, const DMatrix &e, const DMatrix &vt, DMatrix &inv) const;
 bool Verify(const DMatrix &in, const DMatrix &u, const DMatrix &e, const DMatrix &vt) const;
};

#endif //SVD_HPP
