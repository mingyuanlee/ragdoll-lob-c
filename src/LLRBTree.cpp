
#include <iostream>
#include <cassert>
#include <vector>

#include "LLRBTree.h"
#include "Utils.h"

using namespace std;

LLRBTree::LLRBTree(int instrument, OrderType type): root {nullptr}, instrument {instrument}, type {type} {}

/* ************************************
 *            Tree Helpers
 * ************************************/

bool LLRBTree::is_red(LimitNode *h) {
  if (h == nullptr) return false;
  return h->color == RED;
}

void LLRBTree::color_flip(LimitNode *h) {
  assert(h != nullptr);
  assert(h->left != nullptr);
  assert(h->right != nullptr);
  h->color = !h->color;
  h->left->color = !h->left->color;
  h->right->color = !h->right->color;
}

LimitNode *LLRBTree::rotate_left(LimitNode *h) {
  LimitNode *x = h->right;
  h->right = x->left;
  x->left = h;
  x->color = h->color;
  h->color = RED;
  return x;
}

LimitNode *LLRBTree::rotate_right(LimitNode *h) {
  LimitNode *x = h->left;
  h->left = x->right;
  x->right = h;
  x->color = h->color;
  h->color = RED;
  return x;
}

LimitNode *LLRBTree::move_red_left(LimitNode *h) {
  color_flip(h);
  if (is_red(h->right->left)) {
    h->right = rotate_right(h->right);
    h = rotate_left(h);
    color_flip(h);
  }
  return h;
}

LimitNode *LLRBTree::move_red_right(LimitNode *h) {
  color_flip(h);
  if (is_red(h->left->left)) {
    h = rotate_right(h);
    color_flip(h);
  }
  return h;
}

LimitNode *LLRBTree::delete_min(LimitNode *h) {
  if (h->left == nullptr) return nullptr;
  if (!is_red(h->left) && !is_red(h->left->left)) h = move_red_left(h);
  h->left = delete_min(h->left);
  return balance(h);
}

// restore red-black tree invariant
LimitNode *LLRBTree::balance(LimitNode *h) {
  assert(h != nullptr);

  if (is_red(h->right) && !is_red(h->left)) h = rotate_left(h);
  if (is_red(h->left) && is_red(h->left->left)) h = rotate_right(h);
  if (is_red(h->left) && is_red(h->right)) color_flip(h);

  return h;
}

LimitNode *LLRBTree::min(LimitNode *x) {
  assert(x != nullptr);
  if (x->left == nullptr) return x;
  else  return min(x->left);
}

bool LLRBTree::is_empty() {
  return root == nullptr;
}

void LLRBTree::copy_data(LimitNode *dest, LimitNode *src) {
  dest->total_volume = src->total_volume;
  dest->size = src->size;
  dest->head = src->head;
  dest->tail = src->tail;
}

/* ************************************
 *            Tree Functions
 * ************************************/

// will do nothing if the limit price already exists
void LLRBTree::insert_limit_price(int limit_price) {
  if (limit_map.find(limit_price) != limit_map.end()) return;
  root = insert(root, limit_price);
  root->color = BLACK;
}

LimitNode *LLRBTree::insert(LimitNode *h, int limit_price) {
  if (h == nullptr) {
    // put the limit node in the map
    LimitNode *new_node = new LimitNode(limit_price);
    limit_map[limit_price] = new_node;
    return new_node;
  }

  if (is_red(h->left) && is_red(h->right)) color_flip(h);

  if (limit_price < h->limit_price) h->left = insert(h->left, limit_price);
  else if (limit_price > h->limit_price) h->right = insert(h->right, limit_price);
  else {
    cout << "Limit price already exists" << endl;
  }

  if (is_red(h->right) && !is_red(h->left)) h = rotate_left(h);
  if (is_red(h->left) && is_red(h->left->left)) h = rotate_right(h);

  return h;
}

void LLRBTree::delete_limit_price(int limit_price) {
  // limit price must exist
  assert(limit_map.find(limit_price) != limit_map.end());
  
  // delete all orders in the limit node
  delete_all_orders(limit_price);

  // tree node deletion
  if (!is_red(root->left) && !is_red(root->right)) root->color = RED;
  root = delete_(root, limit_price);
  if (!is_empty()) root->color = BLACK;

  // remove the limit node from the map
  limit_map.erase(limit_price);
}

LimitNode *LLRBTree::delete_(LimitNode *h, int limit_price) {
  if (limit_price < h->limit_price) {
    if (!is_red(h->left) && !is_red(h->left->left)) h = move_red_left(h);
    h->left = delete_(h->left, limit_price);
  } else {
    if (is_red(h->left)) h = rotate_right(h);
    if (limit_price == h->limit_price && h->right == nullptr) return nullptr;
    if (!is_red(h->right) && !is_red(h->right->left)) h = move_red_right(h);
    if (limit_price == h->limit_price) {
      LimitNode *x = min(h->right);
      
      // Note: must swap map refenrences
      LimitNode *tmp = limit_map[h->limit_price];
      limit_map[h->limit_price] = limit_map[x->limit_price];
      limit_map[x->limit_price] = tmp;

      h->limit_price = x->limit_price;
      copy_data(h, x);

      h->right = delete_min(h->right);
    } else {
      h->right = delete_(h->right, limit_price);
    }
  }
  return balance(h);
}

void LLRBTree::delete_all_orders(int limit_price) {
  LimitNode *node = limit_map[limit_price];
  OrderNode *current = node->head;
  while (current != nullptr) {
    OrderNode *next = current->next;
    // Delete the order map entry
    order_map.erase(current->oid);
    // Delete the order
    delete current;
    current = next;
  }
}

/* ************************************
 *            Query Functions
 * ************************************/

// TODO: O(h) time, make it O(1) time when have time
LimitNode *LLRBTree::min_limit_node() {
  if (root == nullptr) return nullptr;
  LimitNode *curr = root;
  while (curr->left != nullptr) { curr = curr->left; }
  return curr;
}

// TODO: O(h) time, make it O(1) time when have time
LimitNode *LLRBTree::max_limit_node() {
  if (root == nullptr) return nullptr;
  LimitNode *curr = root;
  while (curr->right != nullptr) { curr = curr->right; }
  return curr;
}

// returns null if the limit node does not exist, return read-only pointer
LimitNode *LLRBTree::get_limit_node(int limit_price) const {
  auto it = limit_map.find(limit_price);
  if (it != limit_map.end()) {
    return it->second;
  } else {
    return nullptr;
  }
}

vector<OrderInfo> LLRBTree::get_all_orders() {
  vector<OrderInfo> orders;
  for (auto it = order_map.begin(); it != order_map.end(); it++) {
    OrderNode *order = it->second;
    orders.push_back(OrderInfo(order->oid, order->limit_price, order->volume));
  }
  return orders;
}

void LLRBTree::get_all_price_info_helper(LimitNode const *curr, vector<PriceInfo> &prices) {
  if (curr == nullptr) return;
  get_all_price_info_helper(curr->left, prices);
  prices.push_back(PriceInfo(curr->limit_price, curr->total_volume, curr->size));
  get_all_price_info_helper(curr->right, prices);
}

vector<PriceInfo> LLRBTree::get_all_price_info() {
  vector<PriceInfo> prices;
  get_all_price_info_helper(root, prices);
  return prices;
}


/* ************************************
 *            Order Functions
 * ************************************/

void LLRBTree::insert_order(int oid, int limit_price, int volume, int owner) {
  LimitNode *node = limit_map[limit_price];
  OrderNode *order = node->insert_order(oid, volume, owner);
  order_map[oid] = order;
}

void LLRBTree::cancel_order(int oid) {
  OrderNode *order = order_map[oid];
  LimitNode *node = order->limit_node;
  node->delete_order(order);
  order_map.erase(oid);
}

/* ************************************
 *            Test Functions
 * ************************************/

void LLRBTree::print() {
  prettyPrint(root);
}

void LLRBTree::prettyPrint(LimitNode* root, string prefix, bool isLeft) {
  if( root != nullptr ) {
    std::cout << prefix;
    std::cout << (isLeft ? "├──" : "└──" );
    std::cout << root->limit_price << std::endl;
    prettyPrint( root->left, prefix + (isLeft ? "│   " : "    "), true);
    prettyPrint( root->right, prefix + (isLeft ? "│   " : "    "), false);
  }
}

void LLRBTree::print_limit_map() {
  for (auto it = limit_map.begin(); it != limit_map.end(); it++) {
    int key = it->first;
    LimitNode *node = it->second;
    cout << "<LimitNode: key " << key;
    cout << " price " << node->limit_price;
    cout << " vol " << node->total_volume << " size " << node->size;
    cout << " head " << node->head << " tail " << node->tail << ">" << endl;
  }
}

void LLRBTree::print_orders_via_tree() {
  print_orders_via_tree_helper(root);
}

void LLRBTree::print_orders_via_tree_helper(LimitNode *h) {
  if (h == nullptr) return;
  print_orders_via_tree_helper(h->left);
  cout << "Limit price: " << h->limit_price << " ";
  Utils::print_linked_list(h->head, h->tail);
  print_orders_via_tree_helper(h->right);
}

void LLRBTree::print_orders_via_map() {
  for (auto it = order_map.begin(); it != order_map.end(); it++) {
    cout << it->first << " " << it->second->oid << endl;
  }
}

