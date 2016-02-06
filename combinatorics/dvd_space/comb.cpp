#include <vector>
#include <algorithm>
#include <iostream>
using namespace std;

#define MAX_SPACE 4700000

int find_best_combination(const vector<int> &space_set, vector<int> &best_combination, int space_taken, int current_level, int start) {
 int i, size = space_set.size(), best_usage = space_taken;
 vector<int> combination = best_combination;
 for(i = current_level + start; i < size; ++i) {
  int add_space_taken = space_taken + space_set[i];
  if(add_space_taken < MAX_SPACE) {
   add_space_taken = find_best_combination(space_set, combination, add_space_taken, current_level + 1, i - current_level);
   if(best_usage < add_space_taken) { 
    combination[current_level] = space_set[i];
    best_combination = combination;
    best_usage = add_space_taken;
   }
  } else {
   break;
  } 
 } 
 return best_usage;
}

int find_best_combination(vector<int> &space_set, vector<int> &best_combination) {
 int i, size = space_set.size();
 best_combination.reserve(size);
 for(i = 0; i < size; ++i) {
  best_combination.push_back(0);
 }
 sort(space_set.begin(), space_set.end());
 int sum = 0, best_usage = find_best_combination(space_set, best_combination, 0, 0, 0);
 for(i = 0; i < size; ++i) {
  sum += best_combination[i];
  if(sum >= MAX_SPACE) {
   best_combination[i] = 0;
  }
 }
 return best_usage;
}

int main() {
 vector<int> space_set, best_combination;
 space_set.push_back(709508);
 space_set.push_back(746030);
 space_set.push_back(804177);
 space_set.push_back(1206645);
 cout << "Best Usage: " << find_best_combination(space_set, best_combination) << endl;
 cout << "Best Combination: ";
 for(int i = 0; i < best_combination.size(); ++i) {
  cout << best_combination[i] << " ";
 }
 cout << endl;
 return 0;
}
