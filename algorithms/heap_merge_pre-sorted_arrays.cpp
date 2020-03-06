/*
How to merge k sorted arrays into one big sorted array? (each of them is of the same size n)  

Sample Input: k = 3, n = 4

A[3][4] = {{1,4,7,10}, {5,6,8,11}, {2,3,9,12}}

Sample Output: {1,2,3,4,5,6,7,8,9,10,11,12}

Question: Overall complexity of algorithm.
Answer:
k = 3, n = 4
O(log(k)*n*k)
*/

#include <vector>
#include <iostream>
using namespace std;

void heapify(vector< pair<size_t, size_t> > &h, const vector< vector<int> > &a, size_t ind)
{
   size_t root = ind;
   size_t left = 2 * root + 1;
   size_t right = 2 * root + 2;
   if (left < h.size() && h[left].first < a.size() && h[left].second < a[h[left].first].size() && 
       a[h[left].first][h[left].second] < a[h[root].first][h[root].second])
       root = left;
   if (right < h.size() && h[right].first < a.size() && h[right].second < a[h[right].first].size() && 
       a[h[right].first][h[right].second] < a[h[root].first][h[root].second])
       root = right;
   if (root != ind)
   {
      swap(h[ind], h[root]);
      heapify(h, a, root);
   }
}

void merge(const vector< vector<int> > &a, vector<int> &o)
{
    vector< pair<size_t, size_t> > h;
    h.reserve(a.size());
    for (size_t i(0); i < a.size(); ++i) 
    {
        h.push_back(pair<size_t, size_t>(i, 0));
    }
    for (size_t i(a.size() / 2 - 1); i > 0; --i) 
    {
        heapify(h, a, i);
    }
    while(h.size() > 0)
    {
	heapify(h, a, 0);
        o.push_back(a[h[0].first][h[0].second]);
	if (++(h[0].second) >= a[h[0].first].size())
	{
            swap(h[0], h[h.size() - 1]);
	    h.pop_back();
	}
    }
}

int main()
{
   vector< vector<int> > A = {{1,4,7,10}, {5,6,8,11}, {2,3,9,12}};
   cout << "Input:" << endl;
   for (size_t i(0); i < A.size(); ++i)
   {
       if (A[i].size() > 0)
       {
           cout << A[i][0];
           for (size_t j(1); j < A[i].size(); ++j)
           {
               cout << ',' << A[i][j];
           }
           cout << endl;
       }
   }
   vector<int> O;
   O.reserve(A.size() * A[0].size());
   merge(A, O);
   cout << "Output:" << endl;
   if (O.size() > 0)
   {
       cout << O[0];
       for (size_t i(1); i < O.size(); ++i)
       {
           cout << ',' << O[i];
       }
   }
   cout << endl;
}
