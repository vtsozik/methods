#include <map>
#include <vector>
using namespace std;

typedef vector<double> dvec;
typedef vector< dvec > dvvec;

class mtglck {
 dvec rates;
 dvvec paths;
 bool BuildPaths();
public:
 bool Read(const char *);
 double Sim(int);
 double PV(double);
};
