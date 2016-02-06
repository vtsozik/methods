#ifndef PRICER_HPP
#define PRICER_HPP
#include <math.h>

class Pricer {
 int days_;
 double start_, mean_, ln_mean_, sigma_, ln_sigma_, strike_;
 void GenerateLogPath(double *rates);
 void GeneratePath(double *rates);
 void BoxMuller(double &x, double &y);
 inline double disc(double r, double m) { return pow((1. + r / 1200.), m); }
public:
 Pricer(int days, double strike);
 bool LoadCalib(const char *file);
 double GetAnalyticalPrice();
 double GetMonteCarloPrice(int paths);
 double GetBlack76Price();
 double GetLognormalPrice(int paths);
 double PV(double diff, double loan);
 double PMTDF(double diff, double loan);
};

#endif //PRICER_HPP
