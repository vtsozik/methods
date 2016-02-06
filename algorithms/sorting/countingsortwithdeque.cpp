#include <iostream>
#include <vector>
#include <deque>
#include <stdlib.h>
using namespace std;

typedef vector<int> vint;
typedef deque<int> dint;

void countingsort(vint &v) {
 int min(v[0]), max(min), size(v.size()), val; 
 dint di(1, 1);
 vint r(size, 0);
 for(int i = 1; i < size; ++i) {
  val = v[i];
  if(val < min) {
   while(val < --min) {
    di.push_front(0);
   }
   di.push_front(0);
  } else if(val > max) {
   while(val > ++max) {
    di.push_back(0);
   }
   di.push_back(0);
  }
  ++di[val - min];
cout << "i = " << i << ", val = " << val << ", min = " << min << ", di[" << val - min << "] = " << di[val - min] << endl;
 }
 size = di.size();
cout << "min = " << min << ", max = " << max << endl;
cout << di[0] << ' ';
 for(int i = 1; i < size; ++i) {
cout << di[i] << ' ';
  di[i] += di[i - 1];
 }
cout << endl;
 for(int i = v.size() - 1; i >= 0; --i) {
  val = v[i];
  max = --di[val - min];
cout << "i = " << i << ", val = " << val << ", max = " << max << endl;
  r[max] = val; 
 } 
 v = r;
}

int main() {
 int n = 10, m = 100;
 srand(time(NULL)); 
 vint v(n, 0);
 cout << "Original sequence:" << endl;
 for(int i = 0; i < n; ++i) {
  v[i] = rand() % m;  
  cout << v[i] << ' ';
 }
 cout << endl;
 countingsort(v);
 cout << "Sorted sequence:" << endl;
 for(int i = 0; i < n; ++i) {
  cout << v[i] << ' ';
 }
 cout << endl;
}
