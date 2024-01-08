
#include <iostream>
#include "LLRBTree.h"
#include <cassert>

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
  h->color = RED;
  h->left->color = BLACK;
  h->right->color = BLACK;
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

void LLRBTree::insert_limit_price(int limit_price) {
  root = insert(root, limit_price);
  root->color = BLACK;
}

LimitNode *LLRBTree::insert(LimitNode *h, int limit_price) {
  if (h == nullptr) return new LimitNode(limit_price);

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

void LLRBTree::print() {
  prettyPrint(root);
}

void LLRBTree::prettyPrint(LimitNode* root, string prefix, bool isLeft) {
  if( root != nullptr )
  {
    std::cout << prefix;

    std::cout << (isLeft ? "├──" : "└──" );

    // print the value of the node
    std::cout << root->limit_price << std::endl;

    // enter the next tree level - left and right branch
    prettyPrint( root->left, prefix + (isLeft ? "│   " : "    "), true);
    prettyPrint( root->right, prefix + (isLeft ? "│   " : "    "), false);
  }
}