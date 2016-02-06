#ifndef DRV_HPP
#define DRV_HPP
#include <vector>
#include <iostream>
using namespace std;

typedef vector<double> dvect;
typedef vector<int> ivect;

struct Function {
 Function() {}
 Function(const dvect &pnt, const dvect &grad) : pnt(pnt), grad(grad) {}
 double operator()(const dvect &p) { 
  if(!pnt.empty() && !grad.empty() && p.size() == 1) {
   return line(p);
  } else {
   return function(p);
  }
 }
 inline double function(const dvect &p) {
#ifdef TSTDRV
  return ((p[0] * p[0] * p[1]) + (p[1] * p[1] * p[1] * p[0] * p[0])); 
#else
  return ((p[0] - p[1]) / (p[0] * p[0] + p[1] * p[1] + 2)); 
#endif
 }
 inline double line(const dvect &r) {
  dvect p(pnt);
  for(int i = 0; i < p.size(); ++i) {
//cout << "p[" << i << "] = " << p[i] << " grad[" << i << "] = " <<  grad[i] << " r[0] = " << r[0] << endl;
   p[i] += (r[0] * grad[i]);
  }
  return function(p);
 }
private:
 dvect pnt, grad;
};

struct Line : public Function {
 double operator()(const dvect &t) {
 }
};

class Derivative {
 Function f;
 double p;
public:
 Derivative(const Function &f, double p) : f(f), p(p) {};
 double Partial(const dvect &pnt, const ivect &ind, bool ab = false);
 double Partial(const dvect &pnt, const ivect &ind, int depth, double delta);
 double Partial(const dvect &pnt, int ind, int depth, double delta);
 inline double Combinations(int all, int by) { return (Factorial(all) / Factorial(all - by) / Factorial(by)); }
 double Factorial(int n);
};

#endif //DRV_HPP
