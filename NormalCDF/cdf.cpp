#include <math.h>
#include <iostream>
using namespace std;

double cdf(double d) { //Taylor
 double sum = 0., k = 1. / sqrt(2. * M_PIl), sign = 1., m = d;
 int p = 1;
 do {
  sum += m;
  sign *= -1.;
  p += 2;
  m = sign * pow(d, p) / (p - 2) / (p - 1) / p;
 } while(fabs(m) > 1.e-6 && fabs(m) < 1.e6);
 return (.5 + (k * sum)); 
}

double f(double x) {
 return (2. / sqrt(22. / 7.)) * exp(-pow(x, 2.));
}

double GL(double a, double b) {
 double y1 = 0, y2 = 0, y3 = 0, y4 = 0, y5 = 0;
 double x1 = -sqrt(245. + 14. * sqrt(70.)) / 21., x2 = -sqrt(245. - 14. * sqrt(70.)) / 21.;
 double x3 = 0, x4 = -x2, x5 = -x1;
 double w1 = (322. - 13. * sqrt(70.)) / 900., w2 = (322. + 13. * sqrt(70.)) / 900.;
 double w3 = 128. / 225., w4 = w2, w5 = w1;
 int n = 4800;
 double i = 0, s = 0, h = (b - a) / n;
 for(int i = 0; i <= n; ++i) {
  y1 = h * x1 / 2. + (h + 2. * (a + i * h)) / 2.;
  y2 = h * x2 / 2. + (h + 2. * (a + i * h)) / 2.;
  y3 = h * x3 / 2. + (h + 2. * (a + i * h)) / 2.;
  y4 = h * x4 / 2. + (h + 2. * (a + i * h)) / 2.;
  y5 = h * x5 / 2. + (h + 2. * (a + i * h)) / 2.;
  s += (h * (w1 * f(y1) + w2 * f(y2) + w3 * f(y3) + w4 * f(y4) + w5 * f(y5)) / 2.);
 }
 return s;
}

double cdf1(double d) { //Legendre
 if(d >= 0.) {
  return (1. + GL(0, d / sqrt(2.))) / 2.;
 } else {
  return (1. - GL(0, d / sqrt(2.))) / 2.;
 }
}

double cdf2(double d) { //Euler
 double sum = 0., step = 1.e-6, y = 1., x = step, k = 1. / sqrt(2. * M_PIl), ny;
 do {
  x += step; 
  ny = exp(pow(x, 2.) / -2.); 
  sum += step * (ny + y) / 2.;
  y = ny;
 } while(x < fabs(d));
  if(d < 0.) {
   sum *= -1.;
  }
  return (.5 + (k * sum));
}

double cdf3(double d) { //My Legendre
 /*
    To change limit from [b,a] to[1,-1] :
    if t=(b-a)/2*x+(b+a)/2, then dt=d((b-a)/2*x+(b+a)/2))=(b-a)/2*dx and
    int[b,a](f(t)*dt)=(b-a)/2*int[1,-1](f((b-a)/2*x+(b+a)/2)*dx)=
    =~(b-a)/2*sum[j=0,m](W(xj)*f(xj*(b-a)/2+(b+a)/2));
    To compute integral sum :
    if xj=1, then f(1*(b-a)/2+(b+a)/2)=f(b); if xj=-1, then f(-1*(b-a)/2+(b+a)/2)=f(a);
    Let's a=0, h=(b-a)/n=b/n, i=0,...,n then f((xj*(b-a)/2+(b+a)/2)/n+a+i*h)=
    =f((xj*(b-0)/2+(b+0)/2+a/2)/n+0+i*h)=f(xj/2*b/n+b/n/2+i*h)=
    =f(xj*h/2+h/2+i*h)=f(h*(xj/2+1/2+i))=f(h*(xj/2+i+0.5)), where
    f(x)=1/(2*PI)*exp((x^2)/2) and b=d.
 */
 double s = 0., step = 1e-6, x[5] = { -0.9061798459, -0.5384693101 , 0., 0.5384693101, 0.9061798459 },
  wx[5] = { 0.2369268851, 0.4786286705, 0.5688888888, 0.4786286705, 0.2369268851 };
 int n = abs((int)(d / step));
 for(int i = 0; i <= n; ++i) {
  for(int j = 0; j < 5; ++j) {
   s += wx[j] * exp(pow(step * (x[j] / 2. + (double)i + 0.5), 2.) / -2.);
  }
 }
 s *= (step / (2. * sqrt(2. * M_PIl)));
 if(d < 0.) {
  s *= -1;
 } 
 return 0.5 + s;
}

int main(int argc, char **argv) {
 double d;
 if(argc > 1) {
  d = atof(argv[1]);
  cout << cdf(d) << endl;
  cout << cdf1(d) << endl;
  cout << cdf2(d) << endl;
  cout << cdf3(d) << endl;
 }
 return 0;
}
