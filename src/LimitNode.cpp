#include "LimitNode.h"

#include <iostream>

using namespace std;

// ===================================================
// ==================== OrderNode ====================
// ===================================================

OrderNode::OrderNode(int oid, int limit_price, int volume, int owner, OrderNode *prev, OrderNode *next, LimitNode *limit_node) {
  this->oid = oid;
  this->limit_price = limit_price;
  this->volume = volume;
  this->owner = owner;
  this->prev = prev;
  this->next = next;
  this->limit_node = limit_node;
}


// ===================================================
// ==================== LimitNode ====================
// ===================================================

LimitNode::LimitNode(int limit_price) {
  this->limit_price = limit_price;
  this->total_volume = 0;
  this->size = 0;
  this->left = nullptr;
  this->right = nullptr;
  this->head = nullptr;
  this->tail = nullptr;
  this->color = RED;
}

OrderNode *LimitNode::insert_order(int oid, int volume, int owner) {
  // 1. make the order node
  OrderNode *order = new OrderNode(oid, limit_price, volume, owner, nullptr, nullptr, this);
  // 2. insert into the linked list
  if (head == nullptr) {
    head = order;
    tail = order;
  } else {
    tail->next = order;
    order->prev = tail;
    tail = order;
  }
  // 3. update metadata
  total_volume += order->volume;
  size += limit_price * order->volume;
  return order;
}


void LimitNode::delete_order(OrderNode *order) {
  // 1. remove from the linked list
  if (order->prev != nullptr) {
    order->prev->next = order->next;
  }
  if (order->next != nullptr) {
    order->next->prev = order->prev;
  }

  // update head and tail pointers:
  if (order == tail) tail = order->prev;
  if (order == head) head = order->next;

  // 2. update metadata
  total_volume -= order->volume;
  size -= limit_price * order->volume;
  // 3. clean up the memory
  delete order;
  order = nullptr; // is this line necessary?
}

// Pop an order in FIFO manner
// Warning: the caller must free the order node and delete the map entry
OrderNode *LimitNode::pop_front_order() {
  // 0. assert the linked list is not empty
  if (head == nullptr) {
    return nullptr;
  }
  // 1. remove from the linked list
  OrderNode *order = head;
  head = head->next;
  if (head != nullptr) {
    head->prev = nullptr;
  }
  // 2. update metadata
  total_volume -= order->volume;
  size -= limit_price * order->volume;
  return order;
}

void LimitNode::front_order_deduct_volume(int volume) {
  head->volume -= volume;
  total_volume -= volume;
  size -= limit_price * volume;
}

// // Peek, then can check the data
// OrderNode *LimitNode::peek_front_order() {
//   return head;
// }