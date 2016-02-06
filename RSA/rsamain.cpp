#include <string.h>
#include "rsawrapper.hpp"

int main(int argc, char** argv) {
 rsawrapper rw;
 bool enc;
 srand(time(NULL));
 if(argc > 1 && strcasecmp(argv[1], "-genkeys") == 0) {
  return !rw.generatekeys();
 } else if(argc > 4 && ((enc = strcasecmp(argv[1], "-encrypt") == 0) || strcasecmp(argv[1], "-decrypt") == 0)) {
  return !rw.rsafile(argv[2], argv[3], argv[4], enc);
 }
 cout << "Usage: " << argv[0] << " <-genkeys> | ((<-encrypt|-decrypt>) <keyfile> <inputfile> <outputfile>)" << endl;
 return 1;
}
