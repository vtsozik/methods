#include "arithmetic.hpp"

void testaddsub(const arithmetic &a) {
 bigint b1(bigint::size - 1, true), b2(bigint::size - 1, true), r1, r2;
 cout << "b1 = ";
 b1.print();
 cout << "b2 = ";
 b2.print();
 cout << "testaddsub add -> " << a.add(b1, b2, r1) << endl; 
 cout << "testaddsub subtract -> " << a.subtract(r1, b1, r2) << endl; 
 cout << "testaddsub: compare -> " << (a.compare(r2, b2) == 0) << endl;
 cout << "sum = ";
 r1.print();
}

void testmultiplydivide(const arithmetic &a) {
 bigint n(bigint::size, true), d(bigint::size / 4, true), r1, r2, r3;
 cout << "n(" << dec << n.capacity() << ") = ";
 n.print();
 cout << "d(" << dec << d.capacity() << ") = ";
 d.print();
 cout << "testmultiplydivide divide -> " << a.divide(n, d, r1, r2) << endl;
 cout << "testmultiplydivide multiply -> " << a.multiply(d, r1, r3) << endl;
 cout << "testmultiplydivide add -> " << a.add(r3, r2, r3) << endl;
 cout << "testmultiplydivide: compare -> " << (a.compare(r3, n) == 0) << endl;
 cout << "q(" << dec << r1.capacity() << ") = ";
 r1.print();
 cout << "r(" << dec << r2.capacity() << ") = ";
 r2.print();
}

void testupdownshift(const arithmetic &a) {
 bigint n(bigint::size / 2, true), t1(n),t2(0), t3(0);
 int nc = n.capacity(), t1c = nc, t2c;
 arithmetic::shift shs;
 shs.bytes = 4;
 shs.bits = 4;
 cout << "n(" << nc << ") = ";
 n.print();
 cout << "testupdownshift upshft -> " << a.upshft(t1, t1c, shs) << endl;
 cout << "t1(" << t1c << ") = ";
 t1.print();
 t2 = t1;
 t2c = t1c;
 cout << "testupdownshift dwnshft -> " << a.dwnshft(t2, t2c, shs, t3) << endl;
 cout << "t2(" << t2c << ") = ";
 t2.print();
 cout << "testupdownshift compare -> " << (a.compare(t2, n) == 0) << endl;
}

void testdividecustom(const arithmetic &a) {
 string sn = "+ 0x0000 0x0000 0x0000 0x0000 0x0000 0x0000 0x0000 0x0000 0x0268 0x6783 0xc750 0xab7c 0x6f22 0x6032 0x5828 0x8b49", sd = "+ 0x0000 0x0000 0x0000 0x0000 0x0000 0x0000 0x0000 0x0000 0x0000 0x0000 0x0000 0x0000 0x438d 0xcde7 0xe9e8 0x231b";
 bigint n(sn), d(sd), q, r;
 cout << "testdividecustom: n = ";
 n.print();
 cout << "testdividecustom: d = ";
 d.print();
 cout << "testdividecustom: divide -> " << a.divide(n, d, q, r) << endl; 
 cout << "testdividecustom: q = ";
 q.print();
 cout << "testdividecustom: r = ";
 r.print();
}

void testdividecustom2(const arithmetic &a) {
 string sn = "+ 0x0000 0x0000 0x0000 0x0000 0x0000 0x0000 0x9454 0x0ef1", sd = "+ 0x0000 0x0000 0x0000 0x0000 0x0000 0x0000 0x0000 0x0005";
 bigint n(sn), d(sd), q, r;
 cout << "testdividecustom2: n = ";
 n.print();
 cout << "testdividecustom2: d = ";
 d.print();
 cout << "testdividecustom2: divide -> " << a.divide(n, d, q, r) << endl;
 cout << "testdividecustom2: q = ";
 q.print();
 cout << "testdividecustom2: r = ";
 r.print();
 cout << "testdividecustom2: is n  divisibale by 5: " << n.isdivisibleby5() << endl;
}

void testdividecustom3(const arithmetic &a) {
 string sn = "+ 0x0000 0x0000 0x0000 0x0000 0x0000 0x0000 0x9454 0x0eea", sd = "+ 0x0000 0x0000 0x0000 0x0000 0x0000 0x0000 0x0000 0x0003";
 bigint n(sn), d(sd), q, r;
 cout << "testdividecustom3: n = ";
 n.print();
 cout << "testdividecustom3: d = ";
 d.print();
 cout << "testdividecustom3: divide -> " << a.divide(n, d, q, r) << endl;
 cout << "testdividecustom3: q = ";
 q.print();
 cout << "testdividecustom3: r = ";
 r.print();
 cout << "testdividecustom3: is n  divisibale by 3: " << n.isdivisibleby3() << endl;
}

void testpowandmod(const arithmetic &a) {
 bigint b(bigint::size / 2, true), e(bigint::size, true), m(0), r;
 cout << "testpowandmod: subtract -> " << a.subtract(b, a.one, m) << endl;
 cout << "testpowandmod: powandmod -> " << a.powandmod(b, e, m, r) << endl;
 cout << "testpowandmod: compare -> " << (a.compare(r, a.one) == 0) << endl;
 m.init_rand(0, bigint::size / 4);
// m.init_rand(0, 1);
 cout << "b = ";
 b.print();
 cout << "e = ";
 e.print();
 cout << "m = ";
 m.print();
 cout << "testpowandmod: powandmod -> " << a.powandmod(b, e, m, r) << endl;
 cout << "testpowandmod: compare -> " << (a.compare(r, m) < 0) << endl;
 cout << "r = ";
 r.print();
 b.init();
 cout << "testpowandmod: multiply -> " << a.multiply(m, m, b) << endl;
 cout << "testpowandmod: powandmod -> " << a.powandmod(b, e, m, r) << endl;
 cout << "testpowandmod: capacity -> " << (r.capacity() == 0) << endl;
}

int main() {
 arithmetic a;
 srand(time(NULL)); 
 //testaddsub(a);
 //testmultiplydivide(a);
 //testupdownshift(a);
 //testdividecustom(a);
 //testdividecustom2(a);
 //testdividecustom3(a);
 testpowandmod(a);
 return 0;
}
