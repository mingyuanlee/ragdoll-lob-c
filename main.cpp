#include "LLRBTree.h"
#include <iostream>
#include <string>
using namespace std;

int main() {
  LLRBTree *bid_tree = new LLRBTree(1, BID);

  // Test insertions
  for (int i = 1; i <= 5; i++) {
    bid_tree->insert_limit_price(i * 10);
  }
  bid_tree->print();

  // Test deletions
  cout << "---------------------" << endl;
  bid_tree->delete_limit_price(20);
  bid_tree->delete_limit_price(50);
  bid_tree->delete_limit_price(30);
  bid_tree->delete_limit_price(10);
  bid_tree->delete_limit_price(40);

  // Test limit map
  cout << "---------------------" << endl;
  for (int i = 1; i <= 5; i++) {
    bid_tree->insert_limit_price(i * 10);
  }
  bid_tree->print_limit_map();
  bid_tree->delete_limit_price(20);
  bid_tree->delete_limit_price(50);
  bid_tree->print_limit_map();
}