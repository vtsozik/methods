#ifndef CALIBRATOR_HPP
#define CALIBRATOR_HPP

#include <vector>
#include "Formula.hpp"

using namespace std;

class Calibrator {
 DDMap distMap_;
 double start_, mean_, sigma_;
 double FindMin(Formula &, double);
 void ComputeGradient(Formula&, double, double);
 void ComputeMeanAndSigma();
 void BuildMap(vector<double> &);
 void BuildRatesMap(vector<double> &);
public:
 bool ReadData(const char *);
 void Calibrate();
 bool SaveResults(const char *);
};

#endif //CALIBRATOR_HPP
