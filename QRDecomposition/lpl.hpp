#ifndef LPL_HPP
#define  LPL_HPP
#include <vector>
using namespace std;

typedef vector<bool> BArray;

#if 0
typedef vector<double> DArray;
typedef vector<DArray> DMatrix;
#else
#include "qr.hpp"
#endif

class Laplacian {
 DMatrix input_;
 BArray empty_;
public:	
 Laplacian(const DMatrix &input);
 double GetDeterminant(int row = 0, BArray arr = BArray());
};
#endif //LPL_HPP
