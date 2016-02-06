#include <iostream>
#include <vector>
using namespace std;

typedef vector<double> dvec;

class BM {
 void BoxMuller(double &x, double &y);
 double ComputeMoment(dvec &ds, double origin/* = 0.*/, double power/* = 1.*/);
 double ComputeCovariance(dvec &dsx, dvec &dsy, double ox, double oy);
public:
 BM();
 double operator()(int draws); 
};

BM::BM() {
 srand(time(NULL));
}

void BM::BoxMuller(double &x, double &y) {
 double w;
 do {
  x = (double) 2.0 * rand() / RAND_MAX - 1.0;
  y = (double) 2.0 * rand() / RAND_MAX - 1.0;
  w = x * x + y * y;
 } while (w >= 1.0);
 w = sqrt(-2.0 * log(w) / w);
 x *= w;
 y *= w;
}

double BM::ComputeMoment(dvec &ds, double origin = 0., double power = 1.) {
 double sum = 0.;
 int i, size = ds.size();
 for(i = 0; i < size; ++i) {
  sum += (pow((ds[i] - origin), power) / static_cast<double>(size));
 }
 return sum;
}

double BM::ComputeCovariance(dvec &dsx, dvec &dsy, double ox, double oy) {
 double sum = 0.;
 int i, size = dsx.size();
 for(i = 0; i < size; ++i) {
  sum += ((dsx[i] - ox) * (dsy[i] - oy) / static_cast<double>(size));
 }
 return sum;
}

double BM::operator()(int draws) {
 dvec dsx, dsy;
 double x, y, xy;
 dsx.reserve(draws);
 dsy.reserve(draws);
 for(int i = 0; i < draws; ++i) {
  BoxMuller(x, y);
  dsx.push_back(x); 
  dsy.push_back(y); 
 }
 x = ComputeMoment(dsx);
 y = ComputeMoment(dsy);
 cout << "Means, x: " << x << ", y: " << y << endl;
 xy = ComputeCovariance(dsx, dsy, x, y);
 cout << "Covariance: " << xy << endl;
 x = ComputeMoment(dsx, x, 2.);
 y = ComputeMoment(dsx, y, 2.);
 cout << "Variances, x: " << x << ", y: " << y << endl;
 return xy;
}

int main() {
 BM bm;
 bm(1000000);
 return 0;
}
