#include <iostream>
using namespace std;

/*
Problem: if you have 2 coins, one of them is biased to tail. One coin is randomly 
selected, flipped and shows tail. What is the probability that this coin is biased.
Solution: Bayes Theorem: P(B|T) = (P(T|B) * P(B)) / P(T),
P(T) = 1/2 * 1/2 + 1/2 * 1 = 3/4 - probability that randomly selected coin shows tail,
P(T|B) = 1, probability that biased coin shows tail,
P(B) = 1/2, probability that randomly selected coin is biased, hence:
P(B|T) = (1 * 1/2) / (3/4) = 2/3
*/

int main() {
 srand(time(NULL));
 int b, t;
 b = t = 0;
 for(int i = 0; i < 1000000; i++) {
  if((double) rand() / RAND_MAX > .5) {
   ++t;
   ++b;
  } else {
   if((double) rand() / RAND_MAX > .5) {
    ++t;
   }
  }
  cout << (double) b / t << "\r";
 } 
 cout << endl;
 return 0;
}
