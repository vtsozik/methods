#include <iostream>
using namespace std;

#define DEBPR

template <typename k, typename v> struct node {
 k key;
 v value;
 node *l, *r;
 bool isRed;
 node(const k &key, const v &value) :
  key(key), value(value), 
  l(NULL), r(NULL), isRed(true) {} 
};

template <typename k, typename v> class map {
 node<k, v> *root;
 void clear(node<k, v> *n) {
  if(n != NULL) {
   clear(n->l);
   clear(n->r);
   delete n;
  }
 }
 const v* const find(node<k, v> *n, const k &key) {
#ifdef DEBPR
int cnt = 0;
#endif
  while(n != NULL) {
#ifdef DEBPR
++cnt;
#endif
   if(n->key < key) {
    n = n->r;
   } else if(key < n->key) {
    n = n->l;
   } else {
#ifdef DEBPR
cout << "found in " << cnt << " hops" << endl;
#endif
    return &(n->value);
   }  
  }  
#ifdef DEBPR
cout << "not found in " << cnt << " hops" << endl;
#endif
  return NULL;
 }
 bool insert(node<k, v> **n, const k &key, const v &value) {
  bool isReplaced(false);
  if(*n == NULL) {
   *n = new node<k, v>(key, value);
  } else {
   if(isNodeRed((*n)->l) && isNodeRed((*n)->r)) {
    flip(*n);
   }  
   if((*n)->key < key) {
    isReplaced = insert(&((*n)->r), key, value);
   } else if(key < (*n)->key) {
    isReplaced = insert(&((*n)->l), key, value);
   } else {
    isReplaced = true;
    (*n)->value = value;
   }  
  }  
  if(!isNodeRed((*n)->l) && isNodeRed((*n)->r)) {
   rotateLeft(n);
  }
  if(isNodeRed((*n)->l) && isNodeRed((*n)->l->l)) {
   rotateRight(n);
  }
  return isReplaced;
 }
 bool erase(node<k, v> **n, const k &key) {
  bool isErased(false);
  if(*n != NULL) {
   if(key < (*n)->key) {
    if((*n)->l == NULL || (!(*n)->l->isRed && !isNodeRed((*n)->l->l))) {
     moveRedLeft(n); 
    }
    isErased = erase(&((*n)->l), key);
   } else {
    if(isNodeRed((*n)->l)) {
     rotateRight(n);
    }
    if((isErased = (key == (*n)->key && (*n)->r == NULL))) {
     eraseNode(n);
    } else {
     if((*n)->r == NULL || (!(*n)->r->isRed && !isNodeRed((*n)->r->l))) {
      moveRedRight(n); 
     }
     if(isErased = (key == (*n)->key)) {
      node<k, v> **tn = smallest(&((*n)->r));
      (*tn)->isRed = (*n)->isRed; 
      **n = **tn;
      rebalance(&((*n)->r));
cout << "MAP PRINT BEGIN" << endl;
printMap(*n);
cout << "MAP PRINT END" << endl;
      eraseNode(tn);
     } else {
      isErased = erase(&((*n)->r), key);
     }
    }
   }  
  }  
  return isErased; 
 }
 void flip(node<k, v> *n) {
  if(n != NULL) {
   n->isRed = !n->isRed;
   if(n->l != NULL) {
    n->l->isRed = !n->l->isRed;
   }
   if(n->r != NULL) {
    n->r->isRed = !n->r->isRed;
   }
  }
 }
 bool isNodeRed(node<k, v> *n) {
  return (n != NULL && n->isRed);
 }
 void rotateLeft(node<k, v> **n) {
  if((*n)->r != NULL) {
   node<k, v> *t = (*n)->r;
   t->isRed = (*n)->isRed;
   (*n)->r = t->l;
   (*n)->isRed = true;
   t->l = *n;
   *n = t; 
  }
 }
 void rotateRight(node<k, v> **n) {
  if((*n)->l != NULL) {
   node<k, v> *t = (*n)->l;
   t->isRed = (*n)->isRed;
   (*n)->l = t->r;
   (*n)->isRed = true;
   t->r = *n;
   *n = t; 
  }
 }
 node<k, v> **smallest(node<k, v> **n) {
  node<k, v> **t = n;
  while((*t)->l != NULL) {
   t = &((*t)->l);
  }
  return t;
 }
 void rebalance(node<k, v> **n) {
  if(*n != NULL && (*n)->l != NULL) {
   if((*n)->l == NULL || (!(*n)->l->isRed && !isNodeRed((*n)->l->l))) {
    moveRedLeft(n);   
   }
   rebalance(&((*n)->l));
  }
 }
 void moveRedLeft(node<k, v> **n) {
  flip(*n);
  if((*n)->r != NULL && isNodeRed((*n)->r->l)) {
   rotateRight(&((*n)->r));   
   rotateLeft(n);   
   flip(*n);
  }
 }
 void moveRedRight(node<k, v> **n) {
  flip(*n);
  if((*n)->l != NULL && isNodeRed((*n)->l->l)) {
   rotateRight(n);   
   flip(*n);
  }
 }
 void eraseNode(node<k, v> **n) {
  delete *n;
  (*n) = NULL;
 }
#ifdef DEBPR
 void printMap(node<k, v> *n) {
  if(n != NULL) {
   cout << "key: " << n->key
	<< ", color: " << (n->isRed ? "red" : "black")
	<< ", left key: " << (n->l != NULL ? n->l->key : NULL)
	<< ", right key: " << (n->r != NULL ? n->r->key : NULL)
	<< endl;
   if(n->l != NULL) {
    printMap(n->l);
   }
   if(n->r != NULL) {
    printMap(n->r);
   }
  }
 }
#endif
public:
 map() : root(NULL) {}
 void clear() { 
  clear(root); 
  root = NULL;
 }
 bool empty() { return (root == NULL); }
 bool insert(const k &key, const v &value) {
  return insert(&root, key, value);  
 }
 const v* const find(const k &key) {
  return find(root, key); 
 }
 bool erase(const k &key) {
  bool succ = erase(&root, key);
  if(succ && root != NULL) {
   root->isRed = false; 
  }
  return succ;
 }
#ifdef DEBPR
 void printMap() {
  printMap(root);  
 }
#endif
};

void testnode() {
 node<int, int> n(4, 7);
 cout << "testnode: key = " << n.key << "; value = " << n.value << endl;
}

void testmap() {
 map<int, int> m;
 for(int i = 1; i <= 7; ++i) {
  cout << "testmap: inserting key = " << i << "; value = " << i << "; isReplaced = " << m.insert(i, i) << endl;
 }
 for(int i = 1; i <= 7; ++i) {
  cout << "testmap: inserting key = " << i << "; value = " << i << "; isReplaced = " << m.insert(i, i) << endl;
 }
 for(int i = 1; i <= 7; ++i) {
  cout << "testmap: retrieving key = " << i << "; value = " << *(m.find(i)) << endl; 
 }
 m.clear();
 cout << "testmap: is empty after clearing: " << m.empty() << endl;
 for(int i = 1; i <= 7; ++i) {
  cout << "testmap: inserting key = " << i << "; value = " << i << "; isReplaced = " << m.insert(i, i) << endl;
 }
 for(int i = 1; i <= 7; ++i) {
cout << "BEGIN PRINT MAP IN ERASE LOOP" << endl;
m.printMap();
cout << "END PRINT MAP IN ERASE LOOP" << endl;
  cout << "testmap: erasing key = " << i << "; isErased = " << m.erase(i) << endl;
 }
 cout << "testmap: is empty: " << m.empty() << endl;
}

void testprintmap() {
 map<int, int> m;
 for(int i = 7; i >= 1; --i) {
  m.insert(i, i);
 }
 m.printMap();
}
 
int main() {
/*
 testnode();
 testprintmap();
*/
 testmap();
 return 0;
}
