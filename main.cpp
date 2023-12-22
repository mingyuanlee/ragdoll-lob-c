#include "LOB.h"
#include <iostream>
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
  bid_tree->inorder();
}