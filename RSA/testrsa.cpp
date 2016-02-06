#include "rsa.hpp"

bool testkeygen(const rsa &r, bigint &puk, bigint &prk, bigint &mlds) {
 bool scs;
 cout << "generatekeys -> " << (scs = r.generatekeys(puk, prk, mlds)) << endl;
 cout << "puk = ";
 puk.print();
 cout << "prk = ";
 prk.print();
 cout << "mlds = ";
 mlds.print();
 return scs;
}

void testencrdecr(const rsa &r, const bigint &puk, const bigint &prk, const bigint &mlds) {
 bigint m(rsa::keysize, true, 1), c(0), d(0);
 cout << "m = ";
 m.print();
 cout << "testencr -> " << r.processmessage(puk, mlds, m, c) << endl; 
 cout << "c = ";
 c.print();
 cout << "testdecr -> " << r.processmessage(prk, mlds, c, d) << endl; 
 cout << "d = ";
 d.print();
 cout << "testencrdecr -> " << (r.compare(m, d) == 0) << endl;
}

int main() {
 srand(time(NULL));
 rsa r;
 bigint puk(0), prk(0), mlds(0);
 if(testkeygen(r, puk, prk, mlds)) {
  testencrdecr(r, puk, prk, mlds);
 }
 return 0;
}
