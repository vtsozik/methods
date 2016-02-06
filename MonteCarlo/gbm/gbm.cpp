#include <iostream>
#include <fstream>
#include <sstream>
#include "normal.h"
using namespace std;

#define days_in_year 365.25

class GBM {
 int days_;
 double t_, r_, b_, s0_, k_;
 void BoxMuller(double &x, double &y);
public:
 GBM(int days, double r, double b, double s0, double k);
 double GetBlack73Price();
 double GetMCEPrice(int paths);
 double GetMCE1SPrice(int paths);
 double GetMCAPrice(int paths);
 void CompareAverages(int paths);
};

GBM::GBM(int days, double r, double b, double s0, double k) : days_(days), t_((double) days/days_in_year), r_(r), b_(b), s0_(s0), k_(k) {
 srand(time(NULL)); 
}

double GBM::GetBlack73Price() {
 double d1, d2, scs, cp;
 scs = b_ * sqrt(t_);
 d1 = (log(s0_ / k_) + (r_ + (b_ * b_ / 2.)) * t_) / scs;
 d2 = d1 - scs;
cerr << "stdnormal_cdf(d1) " << stdnormal_cdf(d1) << " stdnormal_cdf(d2) " << stdnormal_cdf(d2) << endl;
 cp = s0_ * stdnormal_cdf(d1) - k_ / exp(r_ * t_) * stdnormal_cdf(d2);
 return cp; 
}

double GBM::GetMCEPrice(int paths) {
 double p = 0, bmr[2], s;
 for(int i = 0; i < paths; ++i) {
  s = s0_;
  for(int j = 0; j < days_; ++j) {
   if(!(j % 2)) {
    BoxMuller(bmr[0], bmr[1]);
   }
   s *= exp((r_ - b_ * b_ / 2.) / days_in_year + b_ * bmr[j % 2] / sqrt(days_in_year)); 
  }
  if(s > k_) {
   p += ((s - k_) / exp(r_ * t_) / (double) paths);
  }
 }
 return p;
}

double GBM::GetMCE1SPrice(int paths) {
 double p = 0, bmr[2], s;
 for(int i = 0; i < paths; ++i) {
  if(!(i % 2)) {
   BoxMuller(bmr[0], bmr[1]);
  }
  s = s0_ * exp((r_ - b_ * b_ / 2.) * t_ + b_ * bmr[i % 2] * sqrt(t_));
  if(s > k_) {
   p += ((s - k_) / exp(r_ * t_) / (double) paths);
  }
 }
 return p;
}

double GBM::GetMCAPrice(int paths) {
 double p = 0, bmr[2], s, a;
 for(int i = 0; i < paths; ++i) {
  s = s0_;
  a = 0.;
  for(int j = 0; j < days_; ++j) {
   if(!(j % 2)) {
    BoxMuller(bmr[0], bmr[1]);
   }
   s *= exp((r_ - b_ * b_ / 2.) / days_in_year + b_ * bmr[j % 2] / sqrt(days_in_year));
   a += (s / days_);
  }
  if(a > k_) {
   p += ((a - k_) / exp(r_ * t_) / (double) paths);
  }
 }
 return p;
}

void GBM::CompareAverages(int paths) {
 double e = 0, bmr[2], s;
 for(int i = 0; i < paths; ++i) {
  s = s0_;
  for(int j = 0; j < days_; ++j) {
   if(!(j % 2)) {
    BoxMuller(bmr[0], bmr[1]);
   }
   s *= exp((r_ - b_ * b_ / 2.) / days_in_year + b_ * bmr[j % 2] / sqrt(days_in_year));
  }
  e += (s / exp(r_ * t_) / (double) paths);
 }
 cout << "Expected: " << e << ", S0 " << s0_ << endl;
}

void GBM::BoxMuller(double &x, double &y) {
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

int main() {
 GBM gbm(40, 0.038, 0.3406, 42.63, 42.5);
#if 1
 cout << "Black73Price: " << gbm.GetBlack73Price() << ", GetMCEPrice: " << gbm.GetMCEPrice(100000) << ", GetMCE1SPrice: " << gbm.GetMCE1SPrice(1000000) << endl;
 cout << "GetMCAPrice: " << gbm.GetMCAPrice(100000) << endl;
#else
 gbm.CompareAverages(1000000);
#endif
 return 0;
}
