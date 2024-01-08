#include <cassert>
#include "LOB.h"

/**
 * Make an order
 * If limit_price == 0, means no limit price
 * 
 * There are three situations in the loop:
 * **** Situation 1: remaining_vol >= node.total_vol -> delete the whole node 
 * **** Situation 2: remaining_vol < node.total_vol -> do loop: check the first node, if fully filled pop the node, if partially filled deduct the amount
 * After the loop, need to create the limit node for the remaining
 */
int LOB::make_order(OrderType type, int limit_price, int volume, int owner) {
  int remaining_vol = volume;
  int last_executed_price = -1;
  int oid = next_oid++;
  if (type == BID) { // buy
    // ask_price = 0 means there is no waiting order, empty tree
    while (ask_price != 0 && (limit_price != 0 ? ask_price <= limit_price : true) && remaining_vol > 0) {
      LimitNode *ask_node = ask_tree.limit_map[ask_price];
      last_executed_price = ask_price;
      if (remaining_vol >= ask_node->total_volume) {
        int vol_to_deduct = ask_node->total_volume;
        ask_tree.delete_limit_price(ask_price);
        remaining_vol -= vol_to_deduct;
        LimitNode *min_node = ask_tree.min_limit_node();
        ask_price = (min_node == nullptr ? 0 : min_node->limit_price);
      } else {
        OrderNode *curr = ask_node->head;
        while (remaining_vol > 0) {
          assert(curr != nullptr); // the above if statement should ensure the curr pointer won't reach the end, if the assertion fails, it means there is inconsistency between the metadata and linked list
          OrderNode *next = curr->next;
          if (curr->volume <= remaining_vol) {
            remaining_vol -= curr->volume;
            ask_node->pop_front_order();
          } else {
            assert(last_executed_price != -1); // must be executed at least once
            ask_node->front_order_deduct_volume(remaining_vol);
            remaining_vol -= curr->volume;
          }
          curr = next;
        }
      }
    }
    if (remaining_vol > 0) {
      if (limit_price > 0) {
        bid_tree.insert_limit_price(limit_price);
        // LimitNode *new_node = bid_tree.get_limit_node(limit_price);
        bid_tree.insert_order(oid, remaining_vol, owner, limit_price);
      } else {
        bid_tree.insert_limit_price(last_executed_price);
        // LimitNode *new_node = bid_tree.get_limit_node(last_executed_price);
        bid_tree.insert_order(oid, remaining_vol, owner, limit_price); // limit price here instead of last executed price
      }
    }
  } else { // sell

  }
  return oid;
}

void LOB::cancel_order(OrderType type, int oid) {

}