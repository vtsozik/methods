#include <fstream>
#include <sstream>
#include "gssnwtn.hpp"
#include "formula.hpp"
using namespace std;

int main() {
 DArray c;
 DMatrix in;
 ifstream ifs("data.dat");
 string line;
 while(getline(ifs, line)) {
  DArray darr;
  double val;
  stringstream iss(line);
  while(iss >> val) {
   darr.push_back(val);
  }
  in.push_back(darr);
 }
 cout << "Input Xs:" << endl;
 PrintArray(in[0]);
 cout << "Input Ys:" << endl;
 PrintArray(in[1]);
 cout << "Input Cs:" << endl;
 PrintArray(in[2]);
 GaussNewton<Formula> gn(in[0], in[1], in[2]);
 cout << "Sum Of Least Squares: " << gn.Fit(c) << endl;
 cout << "Fitted parameters:" << endl;
 PrintArray(c);
 return 0;
}
