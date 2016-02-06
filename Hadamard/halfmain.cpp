#include "halftransform.hpp"

int main() {
 HalfTransform ht;
 IMatrix i(0);
 cout << "Generated Hadamard matrix of size " << HalfTransform::hms  << ':' << endl;
 PrintMatrix(ht.GetHad()); 
 cout << "Generated negative Hadamard matrix of size " << HalfTransform::hms  << ':' << endl;
 PrintMatrix(ht.GetNegHad()); 
 MatrixProduct(ht.GetHad(), ht.GetHad(), i);
 cout << "Hadamard matrix product on its transpose:" << endl;
 PrintMatrix(i); 
#if 0
 for(int c = -128; c < 128; ++c) {
  short s = ht.Code(c);
  char cc = ht.Decode(s);
  if(c == cc) {
   cout << "Test passed for char: " << dec << c << endl;
  } else {
   cout << "Test failed for char: " << dec << c << ", decoded char: " << dec << cc << endl;
   break;
  }
 }
#else
 int c = 'A';
 short s = ht.Code(c);
 cout << "Char: " << (int) c << " coded: " << s << endl;
 for(short es = s + 1; es != s; ++es) {
  char cc = ht.Decode(es);
  cout << "Error: " << es << " decoded: " << (int) cc << endl;
  if(c == cc) {
   cout << "Error corrected with difference: " << es - s << endl;
  }
 }
#endif
 return 0;
}
