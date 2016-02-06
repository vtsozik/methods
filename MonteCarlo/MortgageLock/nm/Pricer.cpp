#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include "normal.h"
#include "Pricer.hpp"
using namespace std;

#define BLACK76

#define SCALER 5.

Pricer::Pricer(int days, double strike) : days_((int)(((double) days) * 5. / 7.)), strike_(strike) {
 srand(time(NULL)); 
}

bool Pricer::LoadCalib(const char *file) {
 bool res;
 ifstream ifs(file);
 if(ifs.is_open()) {
  ifs >> start_;
  ifs >> mean_;
  ifs >> sigma_;
cerr << "start_ " << start_ << " mean_ " << mean_ << " sigma_ " << sigma_ << endl;
cerr << "norm. conv. mean " << exp(mean_ + sigma_ * sigma_ / 2.) << " norm. conv. sigma " << exp(2 * mean_ + sigma_ * sigma_) * (exp(sigma_ * sigma_) - 1.) << endl;
  ln_mean_ = mean_;
  mean_ /= SCALER; 
  sigma_ /= sqrt((double) SCALER);
  ln_sigma_ = sigma_ / ln_mean_ / sqrt((double) DAYS_IN_SERIES);
cerr << "ln_mean_ " << ln_mean_ << " ln_sigma_ " << ln_sigma_ << endl;
cerr << "days " << days_ << " mean_ " << mean_ << " sigma_ " << sigma_ << endl;
 }
 ifs.close();
 return res;
}

double Pricer::GetBlack76Price() {
 double d1, d2, scs, lp;
 scs = ln_sigma_ * sqrt((double) days_);
 d1 = (log(start_ / strike_) + ln_sigma_ * ln_sigma_ * ((double) days_) / 2.) / scs;
 d2 = d1 - scs;
cerr << "vola " << ln_sigma_ * sqrt(360.) << " d1 " << d1 << " d2 " << d2 << endl;
cerr << "stdnormal_cdf(d1) " << stdnormal_cdf(d1) << " stdnormal_cdf(d2) " << stdnormal_cdf(d2) << endl;
 lp = start_ * stdnormal_cdf(d1) - strike_ * stdnormal_cdf(d2);
 return lp; 
}

double Pricer::GetLognormalPrice(int paths) {
 double sum = 0, path, *rates = new double[days_]; 
 for(int i = 0; i < paths; ++i) {
  GenerateLogPath(rates);
  path = rates[days_ - 1];
  if(path > strike_) {
   sum += (path - strike_);
  }
 }
 delete[] rates;
 return (sum / ((double) paths));
}

double Pricer::GetAnalyticalPrice() {
 double m = ((double) days_) * mean_, 
  s = sigma_ * sqrt((double) days_), 
  diff = strike_ - start_,
  z = (diff - m) / s, 
  cdf = stdnormal_cdf(z),
  mdr = s * exp((z * z) / -2.) / sqrt(2. * M_PI) + m * (1. - cdf);
 return (mdr - diff * (1. - cdf));
}

double Pricer::GetMonteCarloPrice(int paths) {
 double sum = 0, path, *rates = new double[days_];
 for(int i = 0; i < paths; ++i) {
  path = start_;
  GeneratePath(rates);
  for(int j = 0; j < days_; ++j) {
   path += rates[j]; 
  }
  if(path > strike_) {
   sum += (path - strike_);
  }
 }
 delete[] rates;
 return (sum / ((double) paths)); 
}

void Pricer::GeneratePath(double *rates) {
 double x, y;
 for(int i = 0; i < days_; ++i) {
  if(!(i % 2)) {
   BoxMuller(x, y); 
   rates[i] = x * sigma_ + mean_; 
  } else {
   rates[i] = y * sigma_ + mean_; 
  }
 }
}

void Pricer::GenerateLogPath(double *rates) {
 double x, y, v = ln_sigma_ * ln_mean_ * sqrt((double) days_);
 for(int i = 0; i < days_; ++i) {
  if(!(i % 2)) {
   BoxMuller(x, y);
   rates[i] = exp(x * v) * start_;
  } else {
   rates[i] = exp(y * v) * start_;
  }
 }
}

void Pricer::BoxMuller(double &x, double &y) {
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

double Pricer::PV(double diff, double loan = 1.) {
 double e = strike_ + diff, ce, cs, pv;
 ce = disc(e, 360.);
 cs = disc(strike_, 360.);
 pv = (ce * e / (ce - 1)) * ((cs - 1) / cs / strike_) - 1;
 return (pv * loan);
}

double Pricer::PMTDF(double diff, double loan = 1.) {
 double er, ce, cs, pmtdf; 
 er = strike_ + diff;
 ce = disc(er, 360.);
 cs = disc(strike_, 360.);
 pmtdf = (ce * er / (ce - 1.) - cs * strike_ / (cs - 1.)) / 1200.;
 return (pmtdf * loan);
}

int main() {
 int paths = 100000, days = 90; 
 double strike = 6.25, loan = 412.;
 cout << "Days: " << days << " Strike: " << strike << " Paths: " << paths << endl;
 Pricer p(days, strike);
 p.LoadCalib("calib_results.txt");
#ifdef BLACK76
 double bl76 = p.GetBlack76Price(), pmtdf = p.PMTDF(bl76);
 cout << "Black 76: pv " << bl76 << " to loan notional: " << p.PV(bl76, loan) << endl; 
 cout << "Black 76: pmt diff " << pmtdf << " to loan notional: " << pmtdf * loan << endl; 
 double mc = p.GetLognormalPrice(paths); 
 cout << "Monte Carlo: " << mc << " PV: " << p.PV(mc, loan) << endl;
#else
 double an = p.GetAnalyticalPrice();
 cout << "Analytical price: " << an << " PV: " << p.PV(an, loan) << endl;
 double mc = p.GetMonteCarloPrice(paths);
 cout << "Monte Carlo: " << mc << " PV: " << p.PV(mc, loan) << endl;
#endif
 return 0;
}
