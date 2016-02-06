#include "fulltransform.hpp"

int main() {
 FullTransform ft;
 IMatrix i(0);
 cout << "Generated Hadamard matrix of size " << FullTransform::hms  << ':' << endl;
 PrintMatrix(ft.GetHad()); 
 cout << "Generated negative Hadamard matrix of size " << FullTransform::hms << ':' << endl;
 PrintMatrix(ft.GetNegHad()); 
 MatrixProduct(ft.GetHad(), ft.GetHad(), i);
 cout << "Hadamard matrix product on its transpose:" << endl;
 PrintMatrix(i); 
 CodeWord cw;
 for(int c = -128; c < 128; ++c) {
  ft.Code(c, cw);
  char cc = ft.Decode(cw);
  if(c == cc) {
   cout << "Test passed for char: " << (int)c << ", coded word:" << endl;
   ft.PrintCodeWord(cw);
  } else {
   cout << "Test failed for char: " << (int)c << ", decoded char: " << (int)cc << ", coded word:" << endl;
   ft.PrintCodeWord(cw);
   break;
  }
 }
 return 0;
}
