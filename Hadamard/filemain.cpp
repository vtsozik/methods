#include <fstream>
#include "fulltransform.hpp"
using namespace std;

#define EXT ".had"

bool Code(const char *fname) {
 bool success;
 fstream ifs(fname, ios::in | ios::binary);
 if(success = ifs.is_open()) {
  string cfnm(fname);
  cfnm += EXT;
  fstream ofs(cfnm.c_str(), ios::out | ios::binary);
  if(success = ofs.is_open()) {
   FullTransform ft;
   char c;
   CodeWord cw;
   ifs.get(c);
   while(ifs.good()) {
    ft.Code(c, cw);
    ofs.write(cw, cws); 
    ifs.get(c);
   }
  }
  ofs.close();
 }
 ifs.close();
 return success; 
}

bool Decode(const char *fname) {
 bool success;
 fstream ifs(fname, ios::in | ios::binary);
 if(success = ifs.is_open()) {
  string dcfnm(fname);
  dcfnm.erase(dcfnm.find(EXT));
  fstream ofs(dcfnm.c_str(), ios::out | ios::binary);
  if(success = ofs.is_open()) {
   FullTransform ft;
   CodeWord cw;
   ifs.read(cw, cws); 
   while(ifs.good()) {
    ofs.put(ft.Decode(cw));
    ifs.read(cw, cws); 
   }
  }
  ofs.close();
 }
 ifs.close();
 return success; 
}

int main(int narg, char **args) {
 bool success;
 if(success = (narg == 2)) {
  if(strcasestr(args[1], EXT) == NULL) {
   success = Code(args[1]);
  } else {
   success = Decode(args[1]);
  }
 } else {
  cerr << "Usage: " << args[0] << " <file(.had) to (de)code> " << endl;
 }
 return ((success) ? 0 : 1);
}
