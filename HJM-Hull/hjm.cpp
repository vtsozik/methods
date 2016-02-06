#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>
using namespace std;

#define __DEBUG__

struct HJMSpec {
 map<string, double*> pMap;
 bool ReadSpec(const char *file);
};

struct Deal : HJMSpec {
 double bMaturity, oMaturity, strike, notional, callPutFlag;
 Deal() : bMaturity(0.), oMaturity(0.), strike(0.), notional(0.), callPutFlag(0.) {
  pMap.insert(make_pair("BondMaturiy", &bMaturity));
  pMap.insert(make_pair("OptionMaturiy", &oMaturity));
  pMap.insert(make_pair("Strike", &strike));
  pMap.insert(make_pair("Notional", &notional));
  pMap.insert(make_pair("CallPutFlag", &callPutFlag));
 }
 bool ReadDeal(const char *deal) { return ReadSpec(deal); }
};

struct VolSpec : HJMSpec {
 double sigma, lambda;
 VolSpec() : sigma(0.), lambda(0.) {
  pMap.insert(make_pair("Sigma", &sigma));
  pMap.insert(make_pair("Lambda", &lambda));
 }
 bool ReadVols(const char *vols) { return ReadSpec(vols); }
};

struct HJMTerm {
 HJMTerm() : term(0.), rate(0.), price(0.), vol(0.) {}
 double term, rate, price, vol;
};

istream& operator>>(istream& is, HJMTerm &hjmt) {
 is >> hjmt.term;
 is >> hjmt.rate;
 return is;
}

#if 0
struct HJMTreeNode : HJMTerm {
 HJMTreeNode *u, *d;
 HJMTreeNode() : u(NULL), d(NULL) {}
};
#endif

typedef vector<HJMTerm> hjmv;
typedef vector<hjmv> hjmvv;
typedef vector<hjmvv> hjmvvv;
typedef vector<double> vd;

class HJM {
 const double observPeriod_;
 int optSteps_, bondSteps_;
 hjmv hjmStates_;
 hjmvvv hjmTree_;
 Deal deal_;
 VolSpec vols_;
 bool ReadForwardCurve(const char *);
 void GenCurves();
 void BuildHullTree();
 double ComputeOptionPrice();
public:
 HJM(double observPeriod) : observPeriod_(observPeriod), optSteps_(0), bondSteps_(0) {}
 double Price(const char *, const char *, const char *);
};

bool HJM::ReadForwardCurve(const char *file) {
 bool res;
 ifstream ifs(file);
 if((res = ifs.is_open())) {
  string line;
  hjmStates_.clear();
  //hjmStates_.reserve(100);
  while(getline(ifs, line)) {
   HJMTerm hjmT;
   stringstream iss(line);
   if((res = (iss >> hjmT))) {
    hjmStates_.push_back(hjmT);
#ifdef __DEBUG__
cout << "Term: " << hjmT.term << " Rate: " << hjmT.rate << endl;
#endif
   } else {
    break;
   }
  }
  ifs.close();
 }
 return res;
}

bool HJMSpec::ReadSpec(const char *file) {
 bool res;
 ifstream ifs(file);
 if((res = ifs.is_open())) {
  string line, key;
  double value;
  while(getline(ifs, line)) {
   stringstream iss(line);
   if((res = (iss >> key >> value))) {
    *(pMap[key]) = value;
#ifdef __DEBUG__
cout << "Key: " << key << " Value: " << value << endl;
#endif
   } else {
    break;
   }
  }
  ifs.close();
 }
 return res;
}

void HJM::GenCurves() {
 double t = 0., price = 1.;
 int i, j, frwCrvSize = hjmStates_.size();
 optSteps_ = static_cast<int>(deal_.oMaturity / observPeriod_);
  bondSteps_ = optSteps_ + static_cast<int>((deal_.bMaturity - 
  deal_.oMaturity) / observPeriod_);
 hjmv curves(bondSteps_);
 for(i = 0, j = 0; i < bondSteps_; ++i) {
  curves[i].term = t;
  curves[i].vol = vols_.sigma * exp(-vols_.lambda * (deal_.bMaturity - t));
  curves[i].rate = hjmStates_[j].rate;
  t += observPeriod_;
  for(;j < frwCrvSize && t > hjmStates_[j].term; ++j);
 }
 for(i = bondSteps_ - 1; i >= 0; --i) {
  curves[i].price = price * exp(-curves[i].rate * observPeriod_);  
  price = curves[i].price;
 }
#ifdef __DEBUG__
cout << "Curve Start" << endl;
for(i = 0; i < bondSteps_; ++i) {
cout << "Term: " << curves[i].term << " Price: " << curves[i].price << " Vol: " << curves[i].vol << endl;
}
cout << "Curve End" << endl;
#endif
 hjmStates_ = curves;
}

void HJM::BuildHullTree() {
 double dt05 = sqrt(observPeriod_), drift, driftSum = 0., 
  scaledVol, volSum = 0., priceDn, priceUp; 
 int numOfNodes = 1, i, j, k, stateSize = bondSteps_ - 1; 
 vector<double> dn(stateSize), up(stateSize);
 hjmTree_.clear();
 hjmTree_.reserve(optSteps_ + 1);
 hjmTree_.push_back(hjmvv(numOfNodes, hjmv(hjmStates_)));
 for(i = stateSize - 1; i >= 0; --i) {
  scaledVol = hjmStates_[i + 1].vol * dt05;
  volSum += (scaledVol * observPeriod_);
  drift = log(cosh(volSum)) / observPeriod_ - driftSum;
  driftSum += drift; 
  up[i] = drift + scaledVol;
  dn[i] = drift - scaledVol;
 }
 for(i = 1; i <= optSteps_; ++i) {
  numOfNodes = static_cast<int>(pow(2., static_cast<double>(i)));
  hjmv hjmStates(hjmStates_.begin() + i, hjmStates_.end());
  stateSize = hjmStates.size();
  hjmvv hjmNodes(numOfNodes, hjmStates);
  for(j = 0; j < numOfNodes; j += 2) {
   hjmv &prevHJMSates = hjmTree_[i - 1][j / 2];
   priceDn = priceUp = 1.;
   for(k = stateSize - 1; k >= 0; --k) {
    hjmNodes[j][k].rate = prevHJMSates[k + 1].rate + up[k + i - 1];
    hjmNodes[j + 1][k].rate = prevHJMSates[k + 1].rate + dn[k + i - 1];
    hjmNodes[j][k].price = priceUp * exp(-hjmNodes[j][k].rate * observPeriod_);
    priceUp = hjmNodes[j][k].price;
    hjmNodes[j + 1][k].price = priceDn * exp(-hjmNodes[j + 1][k].rate * observPeriod_);
    priceDn = hjmNodes[j + 1][k].price;
   }
  }
  hjmTree_.push_back(hjmNodes);
#ifdef __DEBUG__
cout << "State size: " << stateSize << " Computed size: " << bondSteps_ - i << 
 " Nodes: " << numOfNodes << endl;
#endif
 }
#ifdef __DEBUG__
/*
for(i = 0; i < bondSteps_ - 1; ++i) {
cout << "Term: " << hjmStates_[i + 1].term 
 << " Up: " << up[i] 
 << " Down: " << dn[i] 
 << " Vol: " << hjmStates_[i + 1].vol
 << endl;
}
*/
for(i = 0; i < optSteps_; ++i) {
 stateSize = hjmTree_[i].size(); 
 for(j = 0; j < stateSize; ++j) {
  cout << "Tree Level: " << i << " Node: " << j << " Term: " << hjmTree_[i][j][0].term <<
   " Rate: " << hjmTree_[i][j][0].rate <<
   " Price: " << exp(hjmTree_[i][j][0].rate * observPeriod_) << endl;
 }
}
#endif
}

double HJM::ComputeOptionPrice() {
 int i, j, prevOptPrSize, maxTreeWidth = hjmTree_[optSteps_].size();
 vector<double> optionPrices(maxTreeWidth, 0.);
 for(i = 0; i < maxTreeWidth; ++i) {
  optionPrices[i] = deal_.notional * max(
   (hjmTree_[optSteps_][i][0].price -
   deal_.strike) * deal_.callPutFlag, 0.);
#ifdef __DEBUG__
cout << "Term: " << hjmTree_[optSteps_][i][0].term 
 << " Price: " << hjmTree_[optSteps_][i][0].price 
 << " Option Price: " << optionPrices[i] 
 << " Rate: " << hjmTree_[optSteps_][i][0].rate
 << " Vol: " << hjmTree_[optSteps_][i][0].vol << endl;
#endif
 }
 for(i = optSteps_ - 1; i >= 0; --i) {
  vector<double> prevOptionPrices(optionPrices.begin(), 
   optionPrices.begin() + optionPrices.size() / 2);
  prevOptPrSize = prevOptionPrices.size();
  for(j = 0; j < prevOptPrSize; ++j) {
   prevOptionPrices[j] = exp(-hjmTree_[i][j][0].rate * observPeriod_) * 
    (optionPrices[2 * j] + optionPrices[2 * j + 1]) / 2.;
  }
  optionPrices = prevOptionPrices;
 }
 return optionPrices[0];
}

double HJM::Price(const char *deal, const char *frwr, const char *vol) {
 if(deal_.ReadDeal(deal) && ReadForwardCurve(frwr) && vols_.ReadVols(vol)) {
  GenCurves();
  BuildHullTree();
  return ComputeOptionPrice();
 }
 return -1.;
}

int main() {
#ifdef __DEBUG__
 HJM hjm(1. / 6.);
#else
 HJM hjm(14 / 365.25);
#endif
 cout << "Price: " << hjm.Price("deal.dat", "forw.dat", "vols.dat") << endl;
 return 0;
}
