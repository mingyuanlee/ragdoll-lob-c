#include "LOB.h"
#include <iostream>
#include <string>
using namespace std;



int main() {
  LOB book = LOB(1);
  RBTree *bid_tree = &book.bid_tree;
  bid_tree->insertLimitPrice(100);
  // bid_tree->inorder();
  bid_tree->insertLimitPrice(200);
  // bid_tree->inorder();
  bid_tree->insertLimitPrice(300);
  // bid_tree->inorder();
  bid_tree->insertLimitPrice(150);
  bid_tree->print();

  cout << "---------------------" << endl;
  // bid_tree->inorder();
  bid_tree->deleteLimitPrice(200);
  bid_tree->print();
  bid_tree->deleteLimitPrice(100);
  cout << endl;
  bid_tree->print();
  bid_tree->deleteLimitPrice(300);
  cout << endl;
  bid_tree->print();
}

// first problem: after delete 300, nothing is shown
// second problem: after deleting 200, the tree didn't rebalance