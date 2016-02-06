#ifndef MILLERRABIN_HPP
#define MILLERRABIN_HPP

#include "bigint/arithmetic.hpp"
#include "eratosthenes.hpp"

#define MILLERRABINDEBUG

//#define PARALLEL_TEST
//#define PARALLEL_GEN

#if defined(PARALLEL_TEST) || defined(PARALLEL_GEN)
#define PARALLEL
#include <pthread.h>
#endif

#ifdef PARALLEL_TEST
struct threadarg {
 const bigint &b, &bmo, &d;
 const unsigned s;
 const void *mr;
};
struct threadid {
 pthread_t pth;
 int tid;
 bool isprime;
 threadarg *targ;
};
void *testprimep(void *arg);
#endif

#ifdef PARALLEL_GEN
#define SCALE 256
struct threadarg {
 const int certainty;
 const void *mr;
};
struct threadid {
 pthread_t pth;
 int tid;
 bigint b;
 threadarg *targ;
 threadid() : b(0) {}
};
inline void *genprimep(void *arg);
#endif

class millerrabin : public arithmetic {
 const int size;
 const bool regenerate;
#ifdef PARALLEL
mutable pthread_mutex_t mtx; 
mutable pthread_mutex_t mtxc; 
mutable pthread_cond_t cnd;
mutable unsigned indprime;
friend void *genprimep(void *arg);
#endif
public:
 const bigint two, four;
 const eratosthenes er;
 inline millerrabin(int s, bool r) : size((s < bigint::size) ? s : bigint::size), 
  regenerate(r), two(1, (unsigned short)2), four(1, (unsigned short)4), er(5000/*17863*/) {
#ifdef PARALLEL
pthread_mutex_init(&mtx, NULL);
pthread_mutex_init(&mtxc, NULL);
pthread_cond_init(&cnd, NULL);
#endif
 }
 inline ~millerrabin() {
#ifdef PARALLEL
pthread_mutex_destroy(&mtx);
pthread_mutex_destroy(&mtxc);
pthread_cond_destroy(&cnd);
#endif
 } 
 inline void genrandodd(bigint &b) const {
  bool scs = true;
  do {
#ifdef PARALLEL
pthread_mutex_lock(&mtx);
#endif
   b.init_rand(0, size);
#ifdef PARALLEL
pthread_mutex_unlock(&mtx);
#endif
   if(!b.isodd()) {
    scs = add(b, one, b);
   }
  } while(!scs);
 }
 
 inline void genrandinrange(const bigint &l, const bigint &r, bigint &b) const {
  bool scs = true;
  do {
#ifdef PARALLEL
pthread_mutex_lock(&mtx);
#endif
   b.init_rand(0, size);
#ifdef PARALLEL
pthread_mutex_unlock(&mtx);
#endif
   scs = compare(l, b) <= 0 && compare(b, r) <= 0;
  } while(!scs);
 }

 inline bool factorodd(const bigint &b, unsigned &s, bigint &d) const {
  bool scs = true;
  bigint tr1(0), tr2(0); 
  d = b;
  for(s = 0; !d.isodd() && scs; ++s) {
   tr1.init();
   scs = divide(d, two, tr1, tr2);
   d = tr1;
  }
  return scs;
 }

 inline bool testprime(bigint &b, const int &certainty) const {
  bool isprime;
  if((isprime = testdivisionbytrial(b))) {
#ifdef MILLERRABINDEBUG
cout << "FOUND MILLERRABIN CANDIDATE AT: " << time(NULL) << endl;
#endif
   bigint bmo(0);
   isprime = subtract(b, one, bmo);
   if(isprime) {
    unsigned s;
    bigint d(0); 
    isprime = factorodd(bmo, s, d);
#ifdef PARALLEL_TEST
    int i, j;
    threadarg targ = { b, bmo, d, s, this };
    threadid thid[certainty];
    size_t mystacksize;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_getstacksize (&attr, &stacksize);
#ifdef MILLERRABINDEBUG
cout << "DEFAULT THREAD STACK SIZE: " << stacksize << endl;
#endif
    stacksize *= certainty;
    pthread_attr_setstacksize (&attr, stacksize);
#ifdef MILLERRABINDEBUG
cout << "NEW THREAD STACK SIZE: " << stacksize << endl;
#endif
    for(i = 0; i < certainty && isprime; ++i) {
     thid[i].tid = i + 1;
     thid[i].targ = &targ;
     isprime = (pthread_create(&(thid[i].pth), attr, testprimep, (void *) &thid[i]) == 0);
    }
    for(j = 0; j < certainty && isprime;) {
     for(i = 0; i < certainty; ++i) {
      if(thid[i].tid != 0 && pthread_tryjoin_np(thid[i].pth, NULL) == 0) {
       ++j;
       thid[i].tid = 0;
       if(!(isprime = thid[i].isprime)) {
        break;
       }
      }
     } 
    }
    if(0 < j && !isprime) {
     for(i = 0; i < certainty; ++i) { 
      if(thid[i].tid != 0) {
       pthread_cancel(thid[i].pth);
       pthread_join(thid[i].pth, NULL);
#ifdef MILLERRABINDEBUG
//cout << "CANCELLING THREAD: " << thid[i].tid << endl;
#endif
      }
     }
    }
#else
    for(int i = 0; i < certainty && isprime; ++i) {
     isprime = mrtest(b, bmo, d, s);
    }
#endif
   } 
  } 
#ifdef MILLERRABINDEBUG
else {
//cout << "TRIAL DIVISION REJECTED MILLERRABIN CANDIDATE AT: " << time(NULL) << endl;
}
//cout << "REJECTED MILLERRABIN CANDIDATE AT: " << time(NULL) << endl;
#endif
  return isprime;
 }

 inline bool mrtest(const bigint &b, const bigint &bmo, const bigint &d, const unsigned s) const {
  bool isprime;
  unsigned j = 1;
  bigint a, x;
  genrandinrange(two, bmo, a);
  if((isprime = powandmod(a, d, b, x))) {
   if(compare(x, one) != 0 || compare(x, bmo) != 0) {
    bigint tx(0);
    for(; j < s && isprime; ++j) {
     tx.init();
     if((isprime = powandmod(x, two, b, tx))) {
      x = tx;
      if((isprime = !(compare(x, one) == 0))) {
       if(compare(x, bmo) == 0) {
        break;
       }
      }
     }
    }
    isprime = !(j == s);
   }
  }
  return isprime; 
 }

 inline bool testdivisionbytrial(bigint &b) const {
  bool isprime = b.isodd() && !b.isdivisibleby3() && !b.isdivisibleby5();
  if(isprime) {
   bigint d(0), q(0), r(0);
   const vus primes = er.getprimes();
   int tsize = primes.size();
   for(int i = 3; i < tsize; ++i) {
    d.init(1, primes[i]);
/*#ifdef MILLERRABINDEBUG
cout << "i = " << i << endl;
#endif*/
    if(!(isprime = (divide(b, d, q, r) && r.capacity() > 0))) {
/*#ifdef MILLERRABINDEBUG
cout << "b = ";
b.print();
cout << "d = ";
d.print();
cout << "REJECTED BY: " << primes[i] << " REMAINDER: CAPACITY: " << r.capacity() << " VALUE: " << endl;
r.print();
#endif*/
     break;
    }
   }
  }
  return isprime;
 }

 inline void genprime(bigint &b, const int certainty) const {
#ifdef PARALLEL_GEN
  unsigned i;
  threadarg targ = { certainty, this };
  threadid thid[SCALE];
  size_t stacksize;
  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_attr_getstacksize (&attr, &stacksize);
#ifdef MILLERRABINDEBUG
cout << "DEFAULT THREAD STACK SIZE: " << stacksize << endl;
#endif
  stacksize *= SCALE;
  pthread_attr_setstacksize (&attr, stacksize);
#ifdef MILLERRABINDEBUG
cout << "NEW THREAD STACK SIZE: " << stacksize << endl;
#endif
  for(i = 0; i < SCALE; ++i) {
   thid[i].tid = i;
   thid[i].targ = &targ;
   pthread_create(&(thid[i].pth), &attr, genprimep, (void *) &thid[i]);
  }
  pthread_mutex_lock(&mtxc);
  pthread_cond_wait(&cnd, &mtxc);
  b = thid[indprime].b;
  pthread_mutex_unlock(&mtxc);
  for(i = 0; i < SCALE; ++i) {
   pthread_cancel(thid[i].pth);
   pthread_join(thid[i].pth, NULL);
  } 
#else
  genprimei(b, certainty); 
#endif
#ifdef MILLERRABINDEBUG
cout << "ACCEPTED AT: " << time(NULL) << endl;
#endif
 }

 inline void genprimei(bigint &b, const int &certainty) const {
  genrandodd(b);
  while(!testprime(b, certainty)) { 
   if(regenerate || !add(b, (((b.num[0] % 10) == 3) ? four :  two), b)) {
    genrandodd(b);
   }
  }
 }
};

#ifdef PARALLEL_TEST
void *testprimep(void *arg) {
 threadid *thid = (threadid *) arg;
 threadarg *targ = thid->targ;
 thid->isprime = (pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL) == 0) &&
  (pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL) == 0) &&
  ((const millerrabin *)(targ->mr))->mrtest(targ->b, targ->bmo, targ->d, targ->s);
#ifdef MILLERRABINDEBUG
//cout << "THREAD: " << thid->tid << " ISPRIME: " << thid->isprime << endl;
#endif
 return arg;
}
#endif

#ifdef PARALLEL_GEN
inline void *genprimep(void *arg) {
 threadid *thid = (threadid *) arg;
 threadarg *targ = thid->targ;
 pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
 //pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);
 pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
 ((const millerrabin *)(targ->mr))->genprimei(thid->b, targ->certainty);
 pthread_mutex_lock(&(((const millerrabin *)(targ->mr))->mtxc));
 ((const millerrabin *)(targ->mr))->indprime = thid->tid;
 pthread_cond_signal(&(((const millerrabin *)(targ->mr))->cnd));
 pthread_mutex_unlock(&(((const millerrabin *)(targ->mr))->mtxc));
 return arg;
}
#endif

#endif //MILLERRABIN_HPP
