#ifndef LOB_H
#define LOB_H

#include "RBTree.h"
#include <unordered_map>

enum OrderType { Bid, Ask };

class LOB {
private:
  int next_oid;
public:
  // after test, move to private
  RBTree bid_tree;
  RBTree ask_tree;

  int instrument; // instrument id
  int ask_price; // lowest ask price
  int bid_price; // highest bid price

  LOB(int instrument);

  int make_order(OrderType type, int limit_price, int volume, int owner);
  void cancel_order(OrderType type, int oid);
  // cancel_order
};

#endif  // LOB_H