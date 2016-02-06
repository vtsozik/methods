#include <iostream>
#include <math.h>
using namespace std;

/*
Car has 2 headlights. Each bulb has expected life (h) equal to 2500 hours. 
1) What is probability that both bulbs fail in 2500 hours:
				2500				2500
Pcdf(1 bulb fails in 2500h) = int(1/h*exp(-t/h)dt=-exp(-t/h)| = -exp(-2500/2500) + exp(-0/2500) = 
= 1 - exp(-1) = 0.632120559
								0
Pcdf(both 2 bulb fail in 2500h) = Pcdf(1 bulb fails in 2500h) ^ 2 = 0.632120559 ^ 2 = 0.399576401
2) What will be expected life of both bulbs working together:
a) Suppose firts bulb will fail in t1b hours, second bulb in t2b. We need to find:
max(t1b, t2b) = min(t1b,t2b) + (max(t1b, t2b) - min(t1b,t2b)). Let's introduce two
variables tmin = min(t1b,t2b) and tmax = max(t1b, t2b).
b) In the worst case two bulbs will burn out at average in the same time as on one bulb,
i.e. two bulbs should be at least as reliable as one bulb. Hence survival function:
Fsurvival(1 bulb fails in 2500h) = 1 - Pcdf(1 bulb fails in 2500h) = exp(-t/h) =
= exp(-tmin/h) * exp(-tmin/h) = exp(-2*tmin/h); tmin = t / 2 = 2500 / 2 = 1250 hours.
c) Since exponential distribution is memoryless the probability of second bulb to fail
is not dependent on probability of first bulb to fail and at average it will fail after, i.e.
Fsurvival(tmax-tmin|tmin) = exp(-(tmax - tmin)/h) * exp(-tmin/h) = exp(-tmax/h) = exp(-t/h)
Hence tmax - tmin = t = 2500 hours, so in total it will take ttotal = tmin + (tmax - tmin) = 
= 1250 + 2500 = 3750 hours for both bulbs to fail.
d) Overall if we have n bulbs in a system then nth lamp will burn out in:
tn = h * (1 + 1/2 + 1/3 + 1/4 + ... + 1/n); tn = h * sum(i=1, i <= n)(1/i);
BTW: This is famous mathematical sequence since it never converges.
*/

int main() {
 const int np = 1000000;
 const int nl = 2;
 double h = 2500., sum = 0., r[nl], t[nl], tmax; 
 srand(time(NULL));
 for(int i = 0; i < np; i++) {
  tmax = 0;
  for(int j = 0; j < nl; j++) {
  // do {
    r[j] = (double) rand() / RAND_MAX;
  // } while(r[j] == 0. || r[j] == 1.);
   t[j] = log(1. - r[j]);
   if(t[j] < tmax) {
    tmax = t[j];
   }
  }
  sum += tmax;
 }
//cerr << "raw sum " << sum << endl;
 sum /= (double) np;
//cerr << "raw sum " << sum << endl;
 sum *= (-1 * h);
 cout << "Expected life: " << sum << endl;
 return 0;
}
