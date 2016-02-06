#include "bigint.hpp"

#define DEBUG_BIGINT
#ifdef DEBUG_BIGINT
void simpletests() {
 bigint i, u(0), r(bigint::size / 2, true), o(1, (unsigned short)1), 
  m(bigint::size, bigint::max);
 cout << "i = ";
 i.print();
 cout << "u = ";
 u.print();
 cout << "r = ";
 r.print();
 cout << "o = ";
 o.print();
 cout << "m = ";
 m.print();
}

int main() {
 srand(time(NULL));
 simpletests();
 return 0;
}
#endif
