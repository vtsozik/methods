#include "millerrabin.hpp"
#include "eratosthenes.hpp"

#define TRIALS 1000

//#define DPD
void testdiv(const arithmetic &a, const eratosthenes &e, int scaler) {
 int calls = TRIALS;
 unsigned long int ct = 0, start;
 const vus v = e.getprimes();
 for(int i = 0; i < calls; ++i) {
  bigint n(bigint::size / scaler, true), d(1, v[i]), q, r;
  a.divide(n, d, q, r);
  n.init();
  a.multiply(d, q, n);
  q.init();
  r.init();
#ifdef DPD
  cout << "n = ";
  n.print();
  cout << "d = ";
  d.print();
#endif
  start = clock(); 
  a.divide(n, d, q, r);
  ct += (clock() - start);
#ifdef DPD
  cout << "q = ";
  q.print();
  cout << "r = ";
  r.print();
#endif
 }
 cout << "TESTDIV: key = " << dec << LENGTH / scaler << endl;
 cout << "CUMMULATIVE TIME: " << dec << ct << endl;
 cout << "TIME PER CALL: " << dec << ct / calls << endl;
}

//#define DPMR
void testmr(const millerrabin &mr, const eratosthenes &e, int scaler) {
 int calls = TRIALS;
 const vus v = e.getprimes();
 unsigned long int ctc = 0, stc, ctt, startt = time(NULL);
 for(int i = 0; i < calls; ++i) {
  stc = clock();
  bigint n(bigint::size / scaler, true), d(1, v[i]), q, r;
  mr.divide(n, d, q, r);
  n.init();
  mr.multiply(d, q, n);
#ifdef DPMR
  cout << '\r' << dec << i;
#endif
  ctc += (clock() - stc);
  mr.testprime(n, 50);
 }
 ctt = ((long double) ((time(NULL) - startt) * 1000000 - ctc)) / calls;
#ifdef DPMR
 cout << endl;
#endif
 cout << "TESTMR: key = " << dec << LENGTH / scaler << endl;
 cout << "TIME PER CALL: " << dec << ctt << endl;
}

int main() {
 srand(time(NULL));
 eratosthenes e;
 int start = LENGTH / 32, stop = 4, i, size = TRIALS;
 for(i = 1; (int)e.getprimes().size() < size; ++i) {
  e.init(i * TRIALS * log(TRIALS));
 }
 cout << "resulted eratosthenes size: " << e.getprimes().size() << endl;
 for(i = start; i >= stop; i /= 2) {
  millerrabin mr(bigint::size / i, false);
  testdiv(mr, e, i);
  testmr(mr, e, i);
 }
 return 0;
}
