#include <iostream>
#include <stdlib.h>
#include "chinremtheor.hpp"
using namespace std;

/*
 * 380-750 nm visual spectrum
 * Prime wave lengths 383, 503, 619, 743
 */
int main() {
 cout.setf(ios::fixed,ios::floatfield);
#if 0
 long double a = 120, b = 23, r, x, y;
 r = egcd(a, b, x, y);
 cout << "r =  " << r << " x = " << x << " y = " << y << endl;
#else
 long double len;
 vector<long double> waveLengths, phaseShifts;
 waveLengths.reserve(4);
 waveLengths.push_back(383);
 waveLengths.push_back(503);
 waveLengths.push_back(619);
 waveLengths.push_back(743);
 phaseShifts.reserve(waveLengths.size());
#if 0
 for(int i = 0; i < waveLengths.size(); ++i) {
#if 1
  long double dist = 1e10;
  phaseShifts.push_back(dist - floorl(dist / waveLengths[i]) * waveLengths[i]); // Phase shifts for target 5 meters away.
#else
 unsigned rem = 100000 % (unsigned) waveLengths[i];
 rem = (rem * rem) % (unsigned) waveLengths[i];
 phaseShifts.push_back(rem); // Phase shifts for target 5 meters away.
#endif
cout << "wave length [" << i << "] = " << waveLengths[i] << ", phase shift [" << i << "] = " << phaseShifts[i] << endl;
 }
#else
 srand48(time(NULL));
 for(int i = 0; i < waveLengths.size(); ++i) {
  phaseShifts.push_back(truncl(drand48() * waveLengths[i]));
cout << "wave length [" << i << "] = " << waveLengths[i] << ", phase shift [" << i << "] = " << phaseShifts[i] << endl;
 }
#endif
 len = ComputeLength(waveLengths, phaseShifts) / 2e9;
 cout << "target is " << len << " meters away!" << endl;
#endif
 return 0;
}
