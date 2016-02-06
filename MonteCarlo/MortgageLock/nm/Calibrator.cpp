#include <algorithm>
#include <string>
#include <fstream>
#include <sstream>
#include "ZBrent.hpp"
#include "Calibrator.hpp"

#include <iostream>

//#define RATES // - Rate differences if this macro is commented out.

bool Calibrator::ReadData(const char *file) {
 bool res;
 vector<double> rates; 
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

void Calibrator::BuildRatesMap(vector<double> &rates) {
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

void Calibrator::BuildMap(vector<double> &rates) {
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
//cerr << mean_ << "\t" << sigma_ << endl;
}

void Calibrator::ComputeGradient(Formula &formula, double mean, double sigma) {
 double gradient_mean, gradient_sigma, aux_var;
cerr << "Initially: mean " << mean << ", sigma " << sigma << endl;
 formula.SetMean(mean);
 formula.SetSigma(sigma);
 formula.SetType(Formula::MEAN); 
 gradient_mean = formula(); 
 formula.SetType(Formula::SIGMA); 
 gradient_sigma = formula(); 
 formula.SetType(Formula::SQUARE); 
cerr << "SUM OF SQUARES " << sqrt(formula()) << " gradient_mean " << gradient_mean << " gradient_sigma " << gradient_sigma << endl;
 aux_var = -sqrt(pow(gradient_mean, 2) + pow(gradient_sigma, 2));
 gradient_mean /= aux_var;
 gradient_sigma /= aux_var;
cerr << "gradient_mean " << gradient_mean << " gradient_sigma " << gradient_sigma << " aux_var " << aux_var << endl;
 formula.SetGradientMean(gradient_mean);
 formula.SetGradientSigma(gradient_sigma); 
 formula.SetType(Formula::THETA); 
}

double Calibrator::FindMin(Formula &formula, double p) {
 ZBrent<Formula> zbrent(formula, p);
 double guess = -1., df = formula(0.), pddf, ddf;
 do {
    guess /= 2.;
cerr << "Narrow ddf: " << ddf << "\tguess: " << guess << endl;
  } while((ddf = ((formula(guess) - df) / guess)) > 0.);
cerr << "Final narrowed ddf: " << ddf << "\tguess: " << guess << endl;
  return zbrent.Solve(guess, 0.);
}

void Calibrator::Calibrate() {
 double p = 1e-6, mean, sigma, theta = -2.;
 Formula formula(distMap_);
do { 
 mean = mean_;
 sigma = sigma_;
 ComputeGradient(formula, mean, sigma);
 theta = FindMin(formula, p);
 mean_ = formula.GetMean() + theta * formula.GetGradientMean();
 sigma_ = formula.GetSigma() + theta * formula.GetGradientSigma();
cerr << "Computed mean: " << mean_  << ", sigma: "  << sigma_ << ", theta: " << theta << endl;
cerr << "Difference mean: " << fabs(mean_ - mean) << " sigma " << fabs(sigma_ - sigma) << endl;
} while(fabs(theta) > p);
formula.SetType(Formula::SQUARE);
cerr << "Mean: " << mean << ", Sigma: " << sigma << " Sum Of Least Squares: " << sqrt(formula(theta)) << endl;
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

int main() {
 Calibrator c;
 c.ReadData("data.dat");
 c.Calibrate();
 c.SaveResults("calib_results.txt");
 return 0;
}
