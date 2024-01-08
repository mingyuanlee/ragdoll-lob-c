#ifndef LLRB_TREE_H
#define LLRB_TREE_H

#include <unordered_map>
#include <string>

#include "LimitNode.h"

enum TreeType {BID, ASK};

class LLRBTree
{
private:
  LimitNode *root;

  /* Tree Node Operations */
  LimitNode *insert(LimitNode *h, int limit_price);
  LimitNode *delete_(LimitNode *h, int limit_price);
  
  /* LLRB Tree Helpers */
  bool is_red(LimitNode *h);
  void color_flip(LimitNode *h);
  LimitNode *rotate_left(LimitNode *h);
  LimitNode *rotate_right(LimitNode *h);
  LimitNode *move_red_left(LimitNode *h);
  LimitNode *move_red_right(LimitNode *h);
  LimitNode *delete_min(LimitNode *h);
  LimitNode *balance(LimitNode *h);
  LimitNode *min(LimitNode *x);
  void copy_data(LimitNode *dest, LimitNode *src);
  bool is_empty();

  /* Debug Helpers */
  void prettyPrint(LimitNode* root, std::string prefix = "", bool isLeft = false);
  void print_orders_via_tree_helper(LimitNode *h);
public:
  int instrument; // instrument id
  TreeType type; // bid or ask
  std::unordered_map<int, LimitNode*> limit_map; // limit price -> LimitNode
  std::unordered_map<int, OrderNode*> order_map; // order id -> Order

  LLRBTree(int instrument, TreeType tree_type);
  
  void insert_limit_price(int limit_price);
  void delete_limit_price(int limit_price);
  
  void insert_order(int oid, int limit_price, int volume, int owner);
  void cancel_order(int oid);

  // debug functions
  void print();
  void print_limit_map();
  void print_orders_via_tree(); // print orders info via the tree
  void print_orders_via_map(); // print orders info via the map
};

#endif // LLRB_TREE_H