#ifndef RSA_HPP
#define RSA_HPP

#include "bigint/signedarithmetic.hpp"
#include "millerrabin.hpp"

//#define GENKEYSDEBUG
class rsa : public signedarithmetic {
public:
 static const int keysize = bigint::size / 4, certainty = 50;
 static const bool regenerate = true;
 const millerrabin mr;
 rsa() : mr(keysize, regenerate) {}
 inline bool generatekeys(bigint &puk, bigint &prk, bigint &mdls) const {
  bool scs;
  bigint p1(0), p2(0), t(0), dr(0), rt(0);
  do {
   mr.genprime(p1, certainty);
   mr.genprime(p2, certainty);
   mdls.init();
   t.init();
   scs = multiply(p1, p2, mdls) && subtract(p1, one, p1) && subtract(p2, one, p2) && multiply(p1, p2, t);
#ifdef GENKEYSDEBUG
cout << "p1 = ";
p1.print();
cout << "p2 = ";
p2.print();
cout << "t = ";
t.print();
#endif
  } while(scs && t.capacity() == 0);
  while(scs) {
   mr.genrandodd(puk);
   if(compare(mdls, puk) > 0 && (scs = egcd(t, puk, dr, rt, prk)) && rt.sign < 0 && compare(dr, one) == 0) {
#ifdef GENKEYSDEBUG
cout << "puk = ";
puk.print();
cout << "prk = ";
prk.print();
cout << "rt = ";
rt.print();
#endif
     break;
    }
  }
#ifdef GENKEYSDEBUG
bigint tstm, tstq, tstr;
bool tscs = scs && multiply(puk, prk, tstm) && divide(tstm, t, tstq, tstr) &&
(compare(tstr, one) == 0);
cout << "testgeneratekeys -> " << tscs << endl;
if(!tscs) {
cout << "testgeneratekeys -> details:" << endl;
cout << "puk = ";
puk.print();
cout << "prk = ";
prk.print();
cout << "tstm = ";
tstm.print();
cout << "tstr = ";
tstr.print();
}
#endif
  return scs; 
 }    

 inline bool processmessage(const bigint &key, const bigint &mdls, const bigint &in, bigint &out) const {
  out.init();
  return powandmod(in, key, mdls, out);
 }
};

#endif //RSA_HPP
