#include "millerrabin.hpp"

int main() {
 bigint bp(0);
 millerrabin mr(bigint::size / 4, true); 
 for(int i = 0; i < 100; ++i) {
  mr.genprime(bp, 50); 
  bp.print();
 }
 return 0;
}
