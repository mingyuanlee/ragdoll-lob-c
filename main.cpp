#include "LLRBTree.h"
#include <iostream>
#include <string>
using namespace std;

int main() {
  LLRBTree *bid_tree = new LLRBTree(1, BID);

  for (int i = 1; i <= 10; i++) {
    bid_tree->insert_limit_price(i * 10);
  }
  bid_tree->print();

  cout << "---------------------" << endl;

  
}