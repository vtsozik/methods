#include <algorithm>
#include <string>
#include <fstream>
#include <sstream>
#include "gssnwtn.hpp"
#include "formula.hpp"
#include "calibrator.hpp"

//#define RATES // - Distribution of rate differences if this macro is commented out, distribution of rates otherwise.

bool Calibrator::ReadData(const char *file) {
 bool res;
 DVec rates; 
 ifstream ifs(file);
 if(res = ifs.is_open()) {
  string line;
  while(getline(ifs, line)) {
   double val;
   stringstream iss(line);
   if(res = (iss >> val)) {
    rates.push_back(val);
   } else {
    break;
   }
  }
  ifs.close();
#ifdef RATES
  BuildRatesMap(rates);
#else
  BuildMap(rates);
#endif
  ComputeMeanAndSigma();
 }
 return res;
}

void Calibrator::BuildRatesMap(DVec &rates) {
 int i, size = rates.size(), j, ssize;
 double rate, step;
 start_ = rates[size - 1];
 for(i = 0; i < size; ++i) {
  rates[i] = log(rates[i]);
 }
 sort(rates.begin(), rates.end());
 ssize = size - 1;
 step = (double) (rates[ssize] - rates[0]) / ssize;
//cerr << "min: " << rates[0] << "\tmax: " << rates[ssize] << "\tstep: " << step << endl;
 for(i = 0, j = 0, rate = rates[0]; i < size; ++i, rate += step) {
  for(; j < size && rates[j] <= rate; ++j);
  distMap_.insert(make_pair(rate, (double) (j + 1) / (size + 1)));
//cerr << rates[i] << "\t" << rate << "\t" << rates[j] << "\t" << i << "\t" << j << endl;
 } 
/*
for(DDMapIter iter = distMap_.begin(); iter != distMap_.end(); ++iter) {
cerr << iter->first << "\t" << iter->second << endl;
}
*/
}

void Calibrator::BuildMap(DVec &rates) {
 int i, size = rates.size() - 1, j, ssize;
 double rate, step;
 for(i = 0; i < size; ++i) {
  rates[i] = rates[i + 1] - rates[i];
 }
 start_ = rates[size];
 rates.pop_back();
 sort(rates.begin(), rates.end());
 ssize = size - 1;
 step = (double) (rates[size - 1] - rates[0]) / ssize;
 for(i = 0, j = 0, rate = rates[0]; i < size; ++i, rate += step) {
  for(; j < size && rates[j] <= rate; ++j);
  if(i < ssize) {
   --j;
  }
  distMap_.insert(make_pair(rate, (double) (j + 1) / size));
//cerr << rates[i] << "\t" << rate << "\t" << rates[j] << "\t" << i << "\t" << j << endl;
 }
/*
cerr << "Rate Difference	Its Probability" << endl;
for(DDMapIter iter = distMap_.begin(); iter != distMap_.end(); ++iter) {
cerr << iter->first << "\t" << iter->second << endl;
}
*/
}

void Calibrator::ComputeMeanAndSigma() {
 mean_ = sigma_ = 0;
 for(DDMapIter iter = distMap_.begin(); iter != distMap_.end(); ++iter) {
  mean_ += iter->first;
 }
 mean_ = ((double) mean_ / distMap_.size());
 for(DDMapIter iter = distMap_.begin(); iter != distMap_.end(); ++iter) {
  sigma_ += pow(iter->first - mean_, 2.);
 }
 sigma_ = sqrt((double) sigma_ / distMap_.size());
//cerr << "Mean and Sigma Initially: " << mean_ << "\t" << sigma_ << endl;
}

void Calibrator::Calibrate() {
 DVec c(2), x, y;
 int size = distMap_.size();
 x.reserve(size);
 y.reserve(size);
 c[0] = mean_;
 c[1] = sigma_;
//c[0] = 2.16707;
//c[1] = 0.277982;
 for(DDMapIter iter = distMap_.begin(); iter != distMap_.end(); ++iter) {
  x.push_back(iter->first);
  y.push_back(iter->second);
 }
 GaussNewton<Formula> gn(x, y, c);
 cout << "Sum Of Least Squares: " << gn.Fit(c) << endl;
 cout << "Fitted parameters:" << endl;
 PrintArray(c);
 mean_ = c[0];
 sigma_ = c[1];
}

bool Calibrator::SaveResults(const char *file) {
 bool res;
 ofstream ofs(file);
 if(res = ofs.is_open()) {
  ofs << start_ << endl;
  ofs << mean_ << endl;
  ofs << sigma_ << endl;
 } 
 ofs.close();
 return res;
}

#if 1
int main() {
 Calibrator c;
 c.ReadData("data.dat");
 c.Calibrate();
 c.SaveResults("calib_results.txt");
 return 0;
}
#endif
