#include "signedarithmetic.hpp"

void testsignedaddsub(const signedarithmetic &sa) {
 bigint b1(bigint::size - 1, true), b2(bigint::size - 1, true), r1, r2;
 b1.sign = -1;
 cout << "b1 = ";
 b1.print();
 cout << "b2 = ";
 b2.print();
 cout << "testsignedaddsub signedadd -> " << sa.signedadd(b1, b2, r1) << endl;
 cout << "testsignedaddsub signedsubtract -> " << sa.signedsubtract(r1, b1, r2) << endl;
 cout << "testsignedaddsub: signedcompare -> " << (sa.signedcompare(r2, b2) == 0) << endl;
 cout << "sum = ";
 r1.print();
}

void testsignedmultiplydivide(const signedarithmetic &sa) {
 bigint n(bigint::size, true), d(bigint::size / 4, true), r1, r2, r3;
 n.sign = -1;
 cout << "n = ";
 n.print();
 cout << "d = ";
 d.print();
 cout << "testsignedmultiplydivide signeddivide -> " << sa.signeddivide(n, d, r1, r2) << endl;
 cout << "q = ";
 r1.print();
 cout << "r = ";
 r2.print();
 cout << "testsignedmultiplydivide signedmultiply -> " << sa.signedmultiply(d, r1, r3) << endl;
 cout << "n' = ";
 r3.print();
 cout << "testsignedmultiplydivide signedadd -> " << sa.signedadd(r3, r2, r3) << endl;
 cout << "n'' = ";
 r3.print();
 cout << "testsignedmultiplydivide: signedcompare -> " << (sa.signedcompare(r3, n) == 0) << endl;
}

void testegcdsimple(signedarithmetic &sa) {
 bigint b1(1, (unsigned short) 187), b2(1, (unsigned short) 102), rdr(0), r1(0), r2(0), 
  adr(1, (unsigned short) 17, 1), a1(1, (unsigned short) 1, -1), a2(1, (unsigned short) 2, 1);
 cout << "b1 = ";
 b1.print();
 cout << "b2 = ";
 b2.print();
 cout << "testegcdsimple: egcd -> " << sa.egcd(b1, b2, rdr, r1, r2) << endl;
 cout << "rdr = ";
 rdr.print();
 cout << "r1 = ";
 r1.print();
 cout << "r2 = ";
 r2.print();
 cout << "adr = ";
 adr.print();
 cout << "a1 = ";
 a1.print();
 cout << "a2 = ";
 a2.print();
 cout << "testegcdsimple: signedcompare -> " << ((sa.signedcompare(adr, rdr) == 0) && (sa.signedcompare(a1, r1) == 0) && (sa.signedcompare(a2, r2) == 0)) << endl;
}

int main() {
 signedarithmetic sa;
 //testsignedaddsub(sa);
 //testsignedmultiplydivide(sa);
 testegcdsimple(sa);
 return 0;
}
