#ifndef STPST_H
#define STPST_H
#include "drv.hpp"

class Steepest {
 double precision;
 void Gradient(const dvect &pnt, dvect &grad);
 double Minimum(const dvect &pnt, const dvect &grad);
 double ZBrent(Derivative &dl, dvect &rt, ivect &lvl, double ll, double ul);
 double Partial(Derivative &dl, dvect &r, ivect &lvl, double v); 
public:
 Steepest(double p) : precision(p) {}
 void Find(dvect &pnt);
};

#endif //STPST_H
