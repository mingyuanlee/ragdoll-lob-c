#include "LLRBTree.h"
#include "LOB.h"
#include "DTO.h"
#include <iostream>
#include <string>
#include <vector>
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
  bool dangling;
  bid_tree->cancel_order(1, dangling);
  bid_tree->cancel_order(3, dangling);
  bid_tree->cancel_order(5, dangling);
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

void print_order_and_price_info(LOB &lob) {
  cout << "all orders - BID" << endl;
  vector<OrderInfo> orders = lob.get_all_orders(BID);
  for (int i = 0; i < orders.size(); i++) {
    cout << orders[i].oid << " " << orders[i].limit_price << " " << orders[i].volume << endl;
  }
  cout << "all orders - ASK" << endl;
  orders = lob.get_all_orders(ASK);
  for (int i = 0; i < orders.size(); i++) {
    cout << orders[i].oid << " " << orders[i].limit_price << " " << orders[i].volume << endl;
  }
  cout << "all price info - BID" << endl;
  vector<PriceInfo> prices = lob.get_all_price_info(BID);
  for (int i = 0; i < prices.size(); i++) {
    cout << prices[i].limit_price << " " << prices[i].total_volume << " " << prices[i].size << endl;
  }
  cout << "all price info - ASK" << endl;
  prices = lob.get_all_price_info(ASK);
  for (int i = 0; i < prices.size(); i++) {
    cout << prices[i].limit_price << " " << prices[i].total_volume << " " << prices[i].size << endl;
  }
}

void print_best_price_info(LOB &lob) {
  cout << "best price " << lob.get_best_price(BID) << " " << lob.get_best_price(ASK) << endl;
}

void test_lob() {
  LOB lob = LOB(1);
  
  /* test making limit orders - BID */
  lob.make_order(BID, 100, 10, 1);
  print_best_price_info(lob);
  lob.make_order(BID, 105, 10, 2);
  lob.make_order(BID, 95, 20, 1);
  print_best_price_info(lob);
  lob.make_order(BID, 105, 20, 3);
  lob.make_order(BID, 100, 35, 1);
  print_best_price_info(lob);
  print_order_and_price_info(lob);

  /* test making limit orders - ASK */
  cout << "---------------------" << endl;
  lob.make_order(ASK, 120, 10, 1);
  print_best_price_info(lob);
  lob.make_order(ASK, 115, 10, 2);
  lob.make_order(ASK, 130, 20, 1);
  print_best_price_info(lob);
  lob.make_order(ASK, 115, 20, 3);
  lob.make_order(ASK, 120, 35, 1);
  print_best_price_info(lob);
  print_order_and_price_info(lob);

  /* test canceling limit orders */
  cout << "---------------------" << endl;
  lob.cancel_order(BID, 2);
  lob.cancel_order(BID, 4);
  lob.cancel_order(BID, 5);
  lob.cancel_order(ASK, 9);
  lob.cancel_order(ASK, 10);
  lob.cancel_order(ASK, 7);
  print_best_price_info(lob);
  print_order_and_price_info(lob);

  /* test match engine */
  cout << "---------------------" << endl;
  lob.make_order(BID, 130, 15, 1);
  print_best_price_info(lob);
  print_order_and_price_info(lob);

}

int main() {
  // test_tree();

  test_lob();
}