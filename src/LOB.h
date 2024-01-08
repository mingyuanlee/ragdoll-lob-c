#ifndef LOB_H
#define LOB_H

#include "LLRBTree.h"
#include <unordered_map>

class LOB {
private:
  int next_oid;
public:
  // after test, move to private
  LLRBTree bid_tree;
  LLRBTree ask_tree;

  int instrument; // instrument id
  int ask_price; // lowest ask price
  int bid_price; // highest bid price

  LOB(int instrument);

  int make_order(OrderType type, int limit_price, int volume, int owner);
  void cancel_order(OrderType type, int oid);
};

#endif  // LOB_H