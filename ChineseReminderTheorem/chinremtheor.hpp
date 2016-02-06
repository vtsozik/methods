#include <vector>
#include <math.h>
using namespace std;

long double egcd(const long double &a, const long double &b, long double &x, long double &y) {
 long double ppx = 1, px = 0, ppy = 0, py = 1, ppr = a * ppx + b * ppy, pr = a * px + b * py, i, r;
 do {
  i = truncl(ppr / pr);
  x = ppx - i * px;
  y = ppy - i * py;
  r = a * x + b * y;
//cout << "x = " << x << " y = " << y << " ppr = " << ppr << " pr = " << pr << " i = " << i << " r = " << r << endl;
  ppx = px;
  px = x;
  ppy = py;
  py = y;
  ppr = pr;
  pr = r; 
 } while(fabsl(truncl(r)) > 1); 
 return r;
}

long double ComputeLength(vector<long double> &waveLengths, vector<long double> &phaseShifts) {
 int i;
 long double len = 0, maxlen = 1, num, x, y, r;
 for(i = 0; i < waveLengths.size(); ++i) {
  maxlen *= waveLengths[i];
 }
cout << "maxlen = " << maxlen * 1e-9 << " meters" << endl;
 for(i = 0; i < waveLengths.size(); ++i) {
  num = maxlen / waveLengths[i];
  r = egcd(num, waveLengths[i], x, y);
if(fabsl(r) != 1) {
cout << "Error: r = " << r << " != 1" << endl;
}
cout << "i = " << i << ": "  << num << " * " << x << " + " << waveLengths[i] << " * " << y << " = " << r << endl;
  len += (num * x * phaseShifts[i]);
 }
 len = len - floorl(len / maxlen) * maxlen;
 return len; 
}
