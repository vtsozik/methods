#ifndef RSAWRAPPER_HPP
#define RSAWRAPPER_HPP

#include <fstream>
#include "rsa.hpp"

class rsawrapper {

 rsa r;

public:

 static const unsigned scaler = sizeof(unsigned short) / sizeof(char), 
  shiftchar = 1 << (sizeof(char) * BITS), maxchar = shiftchar - 1,
  maxchbuffsize, effchbuffsize;

 inline bool generatekeys() const {
  bool scs;
  bigint puk(0), prk(0), mlds(0);
  if((scs = r.generatekeys(puk, prk, mlds))) {
   cout << "Keys are successfuly generated!" << endl;
   cout << "Public Key: ";
   puk.print();
   cout << "Private key: ";
   prk.print();
   cout << "Modulos: ";
   mlds.print();
  } else {
   cout << "Failed to successfuly generate keys!" << endl;
  }
  return scs;
 }

 inline bool rsafile(const char *keyf, const char *inf, const char *outf, bool enc) const {
  bool scs;
  bigint key(0), mlds(0);
  if((scs = parsekey(keyf, key, mlds))) {
   scs = rsafile(inf, outf, key, mlds, enc);
  } 
  return scs;
 }

 inline bool parsekey(const char *keyf, bigint &key, bigint &mlds) const {
  bool scs;
  ifstream ifs(keyf);
  if((scs = ifs.is_open())) {
   scs = parsekey(ifs, key, "key") && parsekey(ifs, mlds, "modulos");
  } else {
   cout << "Can't open " << keyf << " file!" << endl;
  }
  return scs;
 }

 inline bool parsekey(ifstream &ifs, bigint &bi, const char *desc) const {
  bool scs;
  string line;
  if((scs = getline(ifs, line))) {
   size_t start = line.find_last_of(':'), end = line.find_last_not_of(' ');
   if((scs = (start != string::npos))) {
    start = line.find_first_not_of(' ', start + 1);
    if((scs = (start != string::npos && end != string::npos))) {
     bi.init(line.substr(start, end + 1));
    }
   }
   if(!scs) {
    cout << "Can't parse " << desc << " line!";
   }
  } else {
   cout << "Can't extract " << desc << " line!";
  }
  return scs;
 }

 inline bool rsafile(const char *inf, const char *outf, const bigint &key, const bigint &mlds, bool enc) const {
  bool scs;
  unsigned mldscpc = mlds.capacity() * scaler;
  if((scs = (maxchbuffsize <= mldscpc))) {
   cout << "Buffer size: " << maxchbuffsize << " characters!" << endl;
   ifstream ifs(inf, ios::binary);
   if((scs = ifs.is_open())) {
    ofstream ofs(outf, ios::binary);
    if((scs = ofs.is_open())) {
     bool eof = false;
     char chbuff[maxchbuffsize];
     streamsize pcnt, offset = 0, chbuffsize = (enc) ? effchbuffsize : maxchbuffsize;
     while(!eof && scs) {
      ifs.read(chbuff, chbuffsize); 
      if((scs = (ifs.good() || ifs.eof()))) {
       pcnt = ifs.gcount();
       eof = ifs.eof() || (!enc && ifs.peek() < 0 && pcnt == chbuffsize);
       if(!(eof && !enc && pcnt == 0)) {
        if((scs = rsabuff(chbuff, pcnt, key, mlds, enc, eof))) {
         if((scs = ofs.write(chbuff, pcnt).good())) {
          offset += pcnt;
          cout << "\rProcessed: " << offset << " characters!";
         } else {
          cout << endl << "Can't write " << pcnt << " characters at " << offset << " offset!";
         }
        } else {
         cout << endl << "Can't rsa " << pcnt << " characters at " << offset << " offset!";
        }
       }
      } else {
       cout << endl << "Can't read " << pcnt << " characters at " << offset << " offset!";
      }
     }
     cout << endl;
    } else {
     cout << "Can't open " << outf << " output file!" << endl;
    }  
   } else {
    cout << "Can't open " << inf << " input file!" << endl;
   }
  } else {
   cout << mldscpc << " modulo character size is less than " << maxchbuffsize << " character buffer size!" << endl;
  }
  return scs; 
 }

 inline bool rsabuff(char *chbuff, streamsize &pcnt, const bigint &key, const bigint &mlds, bool enc, bool eof) const {
  bool scs;
   bigint biin, biout(0);
#if 1
   scs = bifrmchbuff(biin, chbuff, (unsigned &) pcnt, enc, eof) &&
    r.processmessage(key, mlds, biin, biout) && bitochbuff(biout, chbuff, (unsigned &) pcnt, enc, eof);
#else
   scs = bifrmchbuff(biin, chbuff, (unsigned &) pcnt, enc, eof) && bitochbuff(biin, chbuff, (unsigned &) pcnt, enc, eof);
#endif
/*
cout << "key: ";
key.print(); 
cout << "mlds: ";
mlds.print(); 
cout << "biin: ";
biin.print(); 
cout << "biout: ";
biout.print(); 
*/
  return scs; 
 }

 inline bool bifrmchbuff(bigint &bi, const char *chbuff, unsigned &chbfsz, bool enc, bool eof) const {
  bool scs;
  unsigned chbuffsize = (enc) ? effchbuffsize : maxchbuffsize;
  if((scs = (chbfsz <= chbuffsize))) {
   if(chbfsz != chbuffsize) {
    if((scs = eof && enc)) {
     int start = (chbfsz % scaler == 0) ? chbfsz / scaler : (chbfsz + 1) / scaler,
      end = effchbuffsize / scaler - 1, rem = effchbuffsize - chbfsz - 1;
     bi.init_rand(start, end);
     if(rem == 0) {
      ((char *) &bi.num[end])[scaler - 1] = (char) maxchar;
     } else {
      int div = 1;
      for(unsigned i = 0; i < scaler; ++i) {
       ((char *) &bi.num[end])[i] = (char) ((rem / div) % shiftchar);
       div *= shiftchar;
      }
     }
    } else {
     cout << endl << chbfsz << " character buffer size is not equal to the expected " << chbuffsize << " character buffer size!" << endl;
    }
   }
   for(unsigned i = 0; i < chbfsz; ++i) {
    ((char *) &bi.num[i / scaler])[i % scaler] = chbuff[i];
   }
   if(enc) {
    chbfsz = maxchbuffsize;
   }
  } else {
   cout << endl << chbfsz << " character buffer size exceeds " << chbuffsize << " character size limit!" << endl;
  }
  return scs;
 }

 inline bool bitochbuff(bigint &bi, char *chbuff, unsigned &chbfsz, bool enc, bool eof) const {
  bool scs;
  if((scs = (chbfsz == maxchbuffsize))) {
   if(eof && !enc) {
    unsigned start = 0, end = effchbuffsize / scaler - 1, 
     rem = 0, trem = ((char *) &bi.num[end])[scaler - 1] % shiftchar;
    if(trem != maxchar) {
     unsigned mult = 1, last = scaler - 1;
     for(unsigned i = 0; i < last; ++i) {
      rem += ((((char *) &bi.num[end])[i] % shiftchar) * mult);
      mult *= shiftchar;
     }
     rem += (trem * mult);
    }
    end = effchbuffsize - 1;
    if((scs = (start <= rem && rem <= end))) {
     chbfsz = effchbuffsize - rem - 1;
    } else {
     chbfsz = 0;
     cout << endl << "Extracted " << rem << " character buffer size remainder is out of [" << start << ',' << end << "] range!" << endl;
    }
   } else {
    chbfsz = (enc) ? maxchbuffsize : effchbuffsize;
   }
   for(unsigned i = 0; i < chbfsz; ++i) {
    chbuff[i] =  ((char *) &bi.num[i / scaler])[i % scaler];
   }
  } else {
   cout << endl << chbfsz << " character buffer size is not equal to the expected " << maxchbuffsize 
    << " character buffer size, when enc is " << enc << " and eof is " << eof << '!' << endl;
  }
  return scs;
 }

};

const unsigned rsawrapper::maxchbuffsize = bigint::size * scaler / 2;
const unsigned rsawrapper::effchbuffsize = maxchbuffsize - 1 * scaler;

#endif //RSAWRAPPER_HPP
