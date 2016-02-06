#include <iostream>
#include <math.h>
using namespace std;

int main() {
 double s = 0;
 for(int i = 1; i <= 1000000000; ++i) {
  s += 1/(double)i;
 }
 cout << "s " << s << " ln(s) " << log(s) << endl;
 return 1;
}
