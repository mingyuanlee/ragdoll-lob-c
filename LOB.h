#ifndef LOB_H
#define LOB_H

#include "RBTree.h"
#include <unordered_map>

class LOB {
private:
public:
  // after test, move to private
  RBTree bid_tree;
  RBTree ask_tree;

  int instrument; // instrument id
  int ask_price; // lowest ask price
  int bid_price; // highest bid price

  LOB(int instrument);

  // insert_order
  // cancel_order
};

#endif  // LOB_H