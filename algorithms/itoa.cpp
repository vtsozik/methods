#include <iostream>
#include <math.h>
using namespace std;

char* itoa(int i, char *a, int alen) {
 int div = (int) pow(10, alen), c = 0, r;
 do {
  if(div > 0) {
   r = i / div;
   if(c > 0 || r > 0) {
    a[c++] = 0x30 + (char) r;
    i -= (r * div);
   }
   div /= 10;
  } else {
    a[c++] = '\0';
  }
 } while(c <= alen);
 return a;
}

int main() {
 int i = 302010;
 char a[10];
 cout << itoa(i, a, sizeof(a) / sizeof(a[0]) - 1) << endl;
 return 0;
}

/* Q: Writa an implementation of function that parses content of an integer to a string.
 * A1: Please see a simple implementation above.
 */
