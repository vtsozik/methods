#include <iostream>
using namespace std;

struct Coin {
 int denom, quant;
};

void coinchange(int amt, Coin *coins, int level, int *num) {
 coins[level].quant = amt / coins[level].denom;
 if(coins[level].denom > 1) {
  while(coins[level].quant >= 0) {
   coinchange(amt - coins[level].denom * coins[level].quant,
    coins, level + 1, num);
   --coins[level].quant;
  }
 } else {
  cout << ++*num << ' ';
  for(int i = 0;; ++i) {
   cout << coins[i].denom << ':' << coins[i].quant << ' ';
   if(coins[i].denom == 1) {
    break;
   }
  }
  cout << endl;
 }
}

int main() {
 int n = 0;
 Coin coins[] = { {25, 0}, {10, 0}, {5, 0}, {1, 0} };
 coinchange(99, coins, 0, &n);
}
