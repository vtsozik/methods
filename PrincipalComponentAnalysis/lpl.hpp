#ifndef LPL_HPP
#define  LPL_HPP
#include <vector>
#include "matrix.hpp"
using namespace std;

typedef vector<bool> BArray;

class Laplacian {
 DMatrix input_;
 BArray empty_;
public:	
 Laplacian(const DMatrix &input);
 double GetDeterminant(int row = 0, BArray arr = BArray());
};
#endif //LPL_HPP
