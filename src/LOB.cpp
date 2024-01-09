#include <cassert>
#include <vector>
#include <iostream>
#include "LOB.h"
#include "DTO.h"

using namespace std;

LOB::LOB(int instrument): 
  next_oid {1}, bid_tree {instrument, BID}, ask_tree {instrument, ASK}, instrument {instrument}, 
  ask_price {0}, bid_price {0} {}

void LOB::update_price(OrderType type) {
  if (type == ASK) {
    LimitNode *min_node = ask_tree.min_limit_node();
    ask_price = (min_node == nullptr ? 0 : min_node->limit_price);
  } else {
    LimitNode *max_node = bid_tree.max_limit_node();
    bid_price = (max_node == nullptr ? 0 : max_node->limit_price);
  }
}

/**
 * Make an order
 * If limit_price == 0, means no limit price
 * 
 * There are two situations in the loop:
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
      LimitNode *ask_node = ask_tree.get_limit_node(ask_price);
      last_executed_price = ask_price;
      if (remaining_vol >= ask_node->total_volume) { // Situation 1
        int vol_to_deduct = ask_node->total_volume;
        ask_tree.delete_limit_price(ask_price);
        remaining_vol -= vol_to_deduct;
        update_price(ASK); // updating the best ask price
      } else {
        OrderNode *curr = ask_node->head;
        while (remaining_vol > 0) {
          // the above if statement should ensure the curr pointer won't reach the end, 
          // if the assertion fails, it means there is inconsistency between the metadata and linked list
          assert(curr != nullptr); 
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
        bid_tree.insert_order(oid, limit_price, remaining_vol, owner);
      } else {
        bid_tree.insert_limit_price(last_executed_price);
        bid_tree.insert_order(oid, limit_price, remaining_vol, owner); // limit price here instead of last executed price
      }
      update_price(BID);
    }
  } else { // sell
    // bid_price = 0 means there is no waiting order, empty tree
    // the second clause means if limit price == 0, then it is a market order, so it will be executed at any price
    while (bid_price != 0 && (limit_price != 0 ? bid_price >= limit_price : true) && remaining_vol > 0) {
      LimitNode *bid_node = bid_tree.get_limit_node(bid_price);
      last_executed_price = bid_price;
      if (remaining_vol >= bid_node->total_volume) {
        int vol_to_deduct = bid_node->total_volume;
        bid_tree.delete_limit_price(bid_price);
        remaining_vol -= vol_to_deduct;
        update_price(BID); // updating the best bid price
      } else {
        OrderNode *curr = bid_node->head;
        while (remaining_vol > 0) {
          assert(curr != nullptr); // the above if statement should ensure the curr pointer won't reach the end, if the assertion fails, it means there is inconsistency between the metadata and linked list
          OrderNode *next = curr->next;
          if (curr->volume <= remaining_vol) {
            remaining_vol -= curr->volume;
            bid_node->pop_front_order();
          } else {
            assert(last_executed_price != -1); // must be executed at least once
            bid_node->front_order_deduct_volume(remaining_vol);
            remaining_vol -= curr->volume;
          }
          curr = next;
        }
      }
    }
    if (remaining_vol > 0) {
      if (limit_price > 0) {
        ask_tree.insert_limit_price(limit_price); // insert the limit node if it doesn't exist
        ask_tree.insert_order(oid, limit_price, remaining_vol, owner);
      } else {
        ask_tree.insert_limit_price(last_executed_price);
        ask_tree.insert_order(oid, limit_price, remaining_vol, owner); // limit price here instead of last executed price
      }
      update_price(ASK);
    }
  }
  return oid;
}

/**
 * Cancel an order
 * 
 * Simply delete the order, do nothing if not exist
 * Do not handle dangling limit node: the daemon will periodically clean up the dangling limit nodes
 */
void LOB::cancel_order(OrderType type, int oid) {
  if (type == BID) {
    bid_tree.cancel_order(oid);
  } else {
    ask_tree.cancel_order(oid);
  }
}

/* ************************************
 *            Query Functions
 * ************************************/

std::vector<OrderInfo> LOB::get_all_orders(OrderType type) {
  LLRBTree *tree = (type == BID ? &bid_tree : &ask_tree);
  return tree->get_all_orders();
}

std::vector<PriceInfo> LOB::get_all_price_info(OrderType type) {
  LLRBTree *tree = (type == BID ? &bid_tree : &ask_tree);
  return tree->get_all_price_info();
}

int LOB::get_best_price(OrderType type) {
  return (type == BID ? bid_price : ask_price);
}