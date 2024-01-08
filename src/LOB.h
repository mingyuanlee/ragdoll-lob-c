#ifndef LOB_H
#define LOB_H

#include <unordered_map>
#include <vector>
#include "LLRBTree.h"

class LOB {
private:
  int next_oid;

  LLRBTree bid_tree;
  LLRBTree ask_tree;

  int instrument; // instrument id
  int ask_price; // lowest ask price
  int bid_price; // highest bid price

  // call this function whenever inserting or deleting limit price
  // if operating on ASK tree, call with ASK, otherwise BID
  void update_price(OrderType type); 
public:
  LOB(int instrument);

  /* modification */
  int make_order(OrderType type, int limit_price, int volume, int owner);
  void cancel_order(OrderType type, int oid);

  /* query */
  std::vector<OrderInfo> get_all_orders(OrderType type);
  std::vector<PriceInfo> get_all_price_info(OrderType type);
  int get_best_price(OrderType type);
};

#endif  // LOB_H