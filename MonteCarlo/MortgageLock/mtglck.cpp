#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "mtglck.hpp"
using namespace std;

#define PATH_LEN 8

bool mtglck::Read(const char *dataFile) {
 bool res;
 ifstream ifs(dataFile);
 if(res = ifs) {
  string line;
  while(getline(ifs, line)) {
   double val;
   stringstream iss(line);
   if(res = (iss >> val)) {
    rates.push_back(val);
   } else {
    break;
   }
  }
  res = BuildPaths();
  ifs.close();
 }
 return res;
}

bool mtglck::BuildPaths() {
 int size = rates.size() - PATH_LEN;
 bool res = (size > 0); 
 if(res) {
  paths.reserve(size);
  for(int i = 0; i < size; ++i) {
   dvec path;
   int end = i + PATH_LEN;
   path.reserve(PATH_LEN);
   for(int j = i + 1; j < end; ++j) {
    path.push_back(rates[j] - rates[j - 1]); 
cerr << rates[j] - rates[j - 1] << " ";
   }
   paths.push_back(path);
cerr << endl;
  }
 }
 return res;
}

double mtglck::Sim(int stopping) {
 double sum = 0, val, sv;
 int isize = paths.size(), jsize = PATH_LEN;
 for(int i = 0; i < isize; ++i) { 
  val = sv = rates[isize - 1];
  int k = (stopping > 0) ? 0 : -1;
  for(int j = 0; j < jsize ; ++j) {
   val += paths[i][j];
   if(k >= 0) {
    sv = val; 
    if(paths[i][j] > 0) {
     ++k;
    }
    if(k >= stopping) {
     k = -1;
    }
   }
  }
  sum += (val - sv);
 }
 sum /= (double) isize;
 return sum;
}

double mtglck::PV(double diff) {
 double r = rates[rates.size() - 1], c, cl, pv;
 c = pow((1. + r / 1200.), 360.);
 cl = pow((1. + (r - diff) / 1200.), 360.);
 pv = 1. - (r - diff) / r * cl / c * (c - 1.) / (cl - 1.);
 return pv;
}

int main() {
 mtglck mtgLck;
 const char *file = "data.dat";
 if(!mtgLck.Read(file)) {
  cerr << "Can't open/parse " << file << " file" << endl;
  return 1;
 }
 for(int i = 0; i <= PATH_LEN; ++i) {
  double diff = mtgLck.Sim(i), pv = mtgLck.PV(diff); 
  cerr << i << " " << diff << " " << pv << endl;
 }
 return 0;
}

