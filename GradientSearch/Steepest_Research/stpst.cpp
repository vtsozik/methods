#include "stpst.hpp"
#include <iostream>
using namespace std;

void Steepest::Find(dvect &pnt) {
 double r, d, max;
 int i, size = pnt.size();
 dvect grad, npnt;
 do {
  Gradient(pnt, grad);
  r = Minimum(pnt, grad);
  max = 0.;
  for(i = 0; i < size; ++i) {
   d = (r * grad[i]);
   pnt[i] += d;
   d = fabs(d);
   if(d > max) {
    max = d;
   }
  }
cout << "max = " << max << " r = " << r << endl;
 } while(max > precision);
}

void Steepest::Gradient(const dvect &pnt, dvect &grad) {
 int i, size = pnt.size();
 double cd = 0;
 Function f;
 Derivative df(f, precision);
 ivect lvl;
 grad.clear();
 grad.reserve(size);
 lvl.push_back(0);
 for(i = 0; i < size; ++i) {
  lvl[0] = i;
  grad.push_back(df.Partial(pnt, lvl));
  cd += (grad[i] * grad[i]);
 }
 cd = -sqrt(cd);
 for(i = 0; i < size; ++i) {
  grad[i] /= cd; 
 }
}

double Steepest::Minimum(const dvect &pnt, const dvect &grad) {
 double ll = 1., ul, h = 2., m = 1., df, dfh;
 Function l(pnt, grad);
 Derivative dl(l, precision);
 dvect r;
 ivect lvl;
 df = Partial(dl, r, lvl, ll);
 if(df > 0) {
  m = -1;
 }
 dfh = df;
 while((df * dfh) > 0) {
  ul = ll + h * m;
  dfh = Partial(dl, r, lvl, ul);
  h *= h;
 }
cout << "ll " << ll << " ul " << ul << " df " << df << " dfh " << dfh << endl;
 if(df < 0) {
  return ZBrent(dl, r, lvl, ll, ul);
 } else {
  return ZBrent(dl, r, lvl, ul, ll);
 }
}

double Steepest::ZBrent(Derivative &dl, dvect &rt, ivect &lvl, double ll, double ul) {
 double a = (ll + ul) / 2, na, yll, ya = Partial(dl, rt, lvl, a), yul, r, s, t, p, q;
//cout << "a = " << a << " ya = " << ya << " p = " << precision << endl;
 while(fabs(ya) > precision) {
  yll = Partial(dl, rt, lvl, ll);
  yul = Partial(dl, rt, lvl, ul);
  r = ya / yul;
  s = ya / yll;
  t = yll / yul;
//cout << yll << " " << ya << " " << yul << endl;
  p = s * (t * (r - t) * (ul - a) - (1 - r) * (a - ll));
  q = (t - 1) * (r - 1) * (s - 1);
  na = a + p / q;
  if(na > a) {
   ll = a;
  } else if(na < a) {
   ul = a;
  } else {
//cout << ll << " / " << na << " / " << ul << " / " << ya << endl;
   return na;
  }
  a = na;
  ya = Partial(dl, rt, lvl, a);
//cout << ll << " - " << a << " - " << ul << " - " << ya << endl;
 }
 return a;
}

double Steepest::Partial(Derivative &dl, dvect &r, ivect &lvl, double v) {
 if(r.empty()) {
  r.push_back(v);
 } else {
  r[0] = v;
 }
 if(lvl.empty()) {
  lvl.push_back(0);
 }
 return dl.Partial(r, lvl);
}

int main() {
 dvect ipnt;
 ipnt.push_back(-3.); 
 ipnt.push_back(-2.); 
 Steepest stpst(1e-9);
 stpst.Find(ipnt);
 for(int i = 0; i < ipnt.size(); ++i) {
  cout << "i = " << i << " p = " << ipnt[i] << endl;
 }
 return 0;
}
