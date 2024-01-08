#include "LLRBTree.h"
#include "LOB.h"
#include <iostream>
#include <string>
using namespace std;

void test_tree() {
  LLRBTree *bid_tree = new LLRBTree(1, BID);

  // Test limit insertions
  for (int i = 1; i <= 5; i++) {
    bid_tree->insert_limit_price(i * 10);
  }
  bid_tree->print();

  // Test limit deletions
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
  bid_tree->print_limit_map();
  bid_tree->insert_order(1, 30, 10, 1);
  bid_tree->insert_order(2, 30, 20, 2);
  bid_tree->insert_order(3, 30, 30, 3);
  bid_tree->insert_order(4, 10, 10, 4);
  bid_tree->insert_order(5, 40, 20, 5);
  bid_tree->insert_order(6, 40, 20, 5);
  bid_tree->insert_order(7, 10, 20, 5);
  bid_tree->print_orders_via_tree();
  bid_tree->print_orders_via_map();

  // Test single order deletions
  cout << "---------------------" << endl;
  cout << "Testing single order deletions" << endl;
  bid_tree->print();
  bid_tree->cancel_order(1);
  bid_tree->cancel_order(3);
  bid_tree->cancel_order(5);
  bid_tree->print_orders_via_tree();
  bid_tree->print_orders_via_map();

  // Test remove whole limit node and its orders
  cout << "---------------------" << endl;
  cout << "Testing remove whole limit node and its orders" << endl;
  cout << "Before remove limit node 10" << endl;
  bid_tree->print();
  bid_tree->print_orders_via_tree();
  bid_tree->print_orders_via_map();
  cout << "After remove limit node 10" << endl;
  bid_tree->delete_limit_price(10);
  bid_tree->print();
  bid_tree->print_orders_via_tree();
  bid_tree->print_orders_via_map();

  // clean up
  delete bid_tree;
  bid_tree = nullptr; 
}

void test_lob() {
  LOB lob = LOB(1);
  
  /* test making bid orders */
  lob.make_order(BID, 100, 10, 1);
  lob.make_order(BID, 105, 10, 2);
  lob.make_order(BID, 95, 20, 1);

}

int main() {
  // test_tree();

  test_lob();
}