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
  // cout << "---------------------" << endl;
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

  // Test order insertions
  cout << "---------------------" << endl;
  // Here we have limit nodes: 30, 10, 40
  bid_tree->print();
  bid_tree->insert_order(1, 30, 10, 1);
  bid_tree->insert_order(2, 30, 20, 2);
  bid_tree->insert_order(3, 30, 30, 3);
  bid_tree->insert_order(4, 10, 10, 4);
  bid_tree->insert_order(5, 40, 20, 5);
  bid_tree->insert_order(6, 40, 20, 5);
  // bid_tree->insert_order(7, 20, 20, 5);
  bid_tree->print_orders_via_tree();
  bid_tree->print_orders_via_map();
}