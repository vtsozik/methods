#ifndef CN_HPP
#define CN_HPP

#include<vector>
using namespace std;

class CN {
 double l, ti, tx0tin, tx0ti0, a;
 vector<double> b, d, t;
 int nti, nx;
 void Init(int size);
 void Reinit(int size);
 void TDMA(double dx, double dti);
public:
 CN();
 void SNCN();
 void SNFD();
 void SA();
 void PT();
};

#endif //CN_HPP
