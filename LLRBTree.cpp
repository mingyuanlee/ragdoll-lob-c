
#include <iostream>
#include <cassert>

#include "LLRBTree.h"
#include "Utils.h"

using namespace std;

LLRBTree::LLRBTree(int instrument, TreeType type) {
  this->root = nullptr;
  this->instrument = instrument;
  this->type = type;
}

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

void LLRBTree::insert_limit_price(int limit_price) {
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
  // assert key is in the tree
  
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
      h->limit_price = x->limit_price;
      // TODO: h.val = x.val
      h->right = delete_min(h->right);
    } else {
      h->right = delete_(h->right, limit_price);
    }
  }
  return balance(h);
}

// -----------------------
//      test functions
// -----------------------

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
    cout << it->first << " " << it->second->limit_price << endl;
  }
}

void LLRBTree::print_orders_via_tree() {
  print_orders_via_tree_helper(root);
}

void LLRBTree::print_orders_via_tree_helper(LimitNode *h) {
  print_orders_via_tree_helper(h->left);
  Utils::print_linked_list(h->head, h->tail);
  print_orders_via_tree_helper(h->right);
}

void LLRBTree::print_orders_via_map() {
  for (auto it = order_map.begin(); it != order_map.end(); it++) {
    cout << it->first << " " << it->second->oid << endl;
  }
}