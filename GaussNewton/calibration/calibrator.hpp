#ifndef CALIBRATOR_HPP
#define CALIBRATOR_HPP

#include <map>
#include <vector>
using namespace std;

typedef map<double, double> DDMap;
typedef DDMap::iterator DDMapIter;
typedef vector<double> DVec;

class Calibrator {
 DDMap distMap_;
 double start_, mean_, sigma_;
 void ComputeMeanAndSigma();
 void BuildMap(DVec &);
 void BuildRatesMap(DVec &);
public:
 bool ReadData(const char *);
 void Calibrate();
 bool SaveResults(const char *);
};

#endif //CALIBRATOR_HPP
