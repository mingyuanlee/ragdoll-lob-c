#include "RBTree.h"
#include <algorithm>
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
  this->parent = nullptr;
  this->color = RED;
}

OrderNode *LimitNode::insert_order(int oid, int volume, int owner) {
  // 1. make the order node
  OrderNode *order = new OrderNode(oid, limit_price, volume, owner, nullptr, nullptr, this);
  // 2. insert into the linked list
  if (head == nullptr) {
    head = order;
    tail = order;
    return order;
  }
  tail->next = order;
  order->prev = tail;
  tail = order;
  // 3. update metadata
  total_volume += order->volume;
  size += limit_price * order->volume;
  return order;
}

// TODO: Note that bid_order_map still references to this node. It is caller(LOB)'s responsibility to delete the map entry first
// using 
/*
auto it = myMap.find(key);
if (it != myMap.end()) {
    myMap.erase(it);
}
*/
void LimitNode::delete_order(OrderNode *order) {
  // 1. remove from the linked list
  if (order->prev != nullptr) {
    order->prev->next = order->next;
  }
  if (order->next != nullptr) {
    order->next->prev = order->prev;
  }
  // 2. clean up the memory
  delete order;
  order = nullptr;
  // 3. update metadata
  total_volume -= order->volume;
  size -= limit_price * order->volume;
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


// ===================================================
// ===================== RB TREE =====================
// ===================================================

RBTree::RBTree(int instrument, TreeType type) {
  this->root = nullptr;
  this->instrument = instrument;
  this->type = type;
}

int RBTree::getColor(LimitNode *&node) {
  if (node == nullptr)
    return BLACK;
  return node->color;
}

void RBTree::setColor(LimitNode *&node, int color) {
  if (node == nullptr)
    return;
  node->color = color;
}

LimitNode* RBTree::insertBST(LimitNode *&root, LimitNode *&ptr) {
  if (root == nullptr)
    return ptr;
  if (ptr->limit_price < root->limit_price) {
    root->left = insertBST(root->left, ptr);
    root->left->parent = root;
  }
  else if (ptr->limit_price > root->limit_price) {
    root->right = insertBST(root->right, ptr);
    root->right->parent = root;
  }
  return root;
}

// a new limit price arrives, insert it into the tree and map, resulting in an empty limit node
LimitNode *RBTree::insertLimitPrice(int limit_price) {
  LimitNode *node = new LimitNode(limit_price);
  limit_map[limit_price] = node;
  root = insertBST(root, node);
  fixInsertRBTree(node);
  return node;
}

LimitNode* RBTree::deleteBST(LimitNode *&root, int limit_price) {
  if (root == nullptr)
    return root;
  if (limit_price < root->limit_price)
    return deleteBST(root->left, limit_price);
  if (limit_price > root->limit_price)
    return deleteBST(root->right, limit_price);
  // Node with Single Child or No Child
  if (root->left == nullptr || root->right == nullptr)
    return root;
  // Node with two children: Get the inorder successor (smallest in the right subtree)
  LimitNode *temp = minValueNode(root->right);
  // Here we need to copy the data to the deleting node, but need to delete all the orders
  OrderNode *current = root->head;
  while (current != nullptr) {
    OrderNode *next = current->next;
    // Delete the order map entry
    order_map.erase(current->oid);
    // Delete the order
    delete current;
    current = next;
  }
  // Then copy the data
  root->limit_price = temp->limit_price;
  root->total_volume = temp->total_volume;
  root->size = temp->size;
  root->head = temp->head;
  root->tail = temp->tail;
  temp->head = nullptr;
  temp->tail = nullptr;
  // Also need to point the map entry to this node
  limit_map[temp->limit_price] = root;
  return deleteBST(root->right, temp->limit_price);
}

/**
 * Find the leftmost node value
*/
LimitNode *RBTree::minLimitNode() {
  if (root == nullptr) return nullptr;
  LimitNode *curr = root;
  while (curr->left != nullptr) { curr = curr->left; }
  return curr;
}

void RBTree::deleteOrder(int oid) {
  OrderNode *order = order_map[oid];
  LimitNode *node = order->limit_node;
  node->delete_order(order);
  order_map.erase(oid);
}

void RBTree::insertOrder(int oid, int volume, int owner, int limit_price) {
  LimitNode *node = limit_map[limit_price];
  OrderNode *order = node->insert_order(oid, volume, owner);
  order_map[oid] = order;
}

// delete a limit price from the tree, will free all the orders, then free the node
// Idea: put the map in the RB Tree
void RBTree::deleteLimitPrice(int limit_price) {
  // Delete the limit map entry
  limit_map.erase(limit_price);
  LimitNode *node = deleteBST(root, limit_price);
  // fix the tree, also delete the node
  fixDeleteRBTree(node);
}

void RBTree::rotateLeft(LimitNode *&ptr) {
  LimitNode *right_child = ptr->right;
  ptr->right = right_child->left;
  if (ptr->right != nullptr)
    ptr->right->parent = ptr;
  right_child->parent = ptr->parent;
  if (ptr->parent == nullptr)
    root = right_child;
  else if (ptr == ptr->parent->left)
    ptr->parent->left = right_child;
  else
    ptr->parent->right = right_child;
  right_child->left = ptr;
  ptr->parent = right_child;
}

void RBTree::rotateRight(LimitNode *&ptr) {
  LimitNode *left_child = ptr->left;
  ptr->left = left_child->right;
  if (ptr->left != nullptr)
    ptr->left->parent = ptr;
  left_child->parent = ptr->parent;
  if (ptr->parent == nullptr)
    root = left_child;
  else if (ptr == ptr->parent->left)
    ptr->parent->left = left_child;
  else
    ptr->parent->right = left_child;
  left_child->right = ptr;
  ptr->parent = left_child;
}

void RBTree::fixInsertRBTree(LimitNode *&ptr) {
    LimitNode *parent = nullptr;
    LimitNode *grandparent = nullptr;
    while (ptr != root && getColor(ptr) == RED && getColor(ptr->parent) == RED) {
      parent = ptr->parent;
      grandparent = parent->parent;
      if (parent == grandparent->left) {
        LimitNode *uncle = grandparent->right;
        if (getColor(uncle) == RED) {
          setColor(uncle, BLACK);
          setColor(parent, BLACK);
          setColor(grandparent, RED);
          ptr = grandparent;
        } else {
          if (ptr == parent->right) {
            rotateLeft(parent);
            ptr = parent;
            parent = ptr->parent;
          }
          rotateRight(grandparent);
          swap(parent->color, grandparent->color);
          ptr = parent;
        }
      } else {
        LimitNode *uncle = grandparent->left;
        if (getColor(uncle) == RED) {
          setColor(uncle, BLACK);
          setColor(parent, BLACK);
          setColor(grandparent, RED);
          ptr = grandparent;
        } else {
          if (ptr == parent->left) {
            rotateRight(parent);
            ptr = parent;
            parent = ptr->parent;
          }
          rotateLeft(grandparent);
          swap(parent->color, grandparent->color);
          ptr = parent;
        }
      }
    }
    setColor(root, BLACK);
}

void RBTree::fixDeleteRBTree(LimitNode *&node) {
  if (node == nullptr)
    return;

  if (node == root) {
    root = nullptr;
    return;
  }

  if (getColor(node) == RED || getColor(node->left) == RED || getColor(node->right) == RED) {
    LimitNode *child = node->left != nullptr ? node->left : node->right;

    if (node == node->parent->left) {
      node->parent->left = child;
      if (child != nullptr)
        child->parent = node->parent;
      setColor(child, BLACK);
      delete (node);
    } else {
      node->parent->right = child;
      if (child != nullptr)
        child->parent = node->parent;
      setColor(child, BLACK);
      delete (node);
    }
  } else {
    LimitNode *sibling = nullptr;
    LimitNode *parent = nullptr;
    LimitNode *ptr = node;
    setColor(ptr, DOUBLE_BLACK);
    while (ptr != root && getColor(ptr) == DOUBLE_BLACK) {
      parent = ptr->parent;
      if (ptr == parent->left) {
        sibling = parent->right;
        if (getColor(sibling) == RED) {
          setColor(sibling, BLACK);
          setColor(parent, RED);
          rotateLeft(parent);
        } else {
          if (getColor(sibling->left) == BLACK && getColor(sibling->right) == BLACK) {
            setColor(sibling, RED);
            if(getColor(parent) == RED)
              setColor(parent, BLACK);
            else
              setColor(parent, DOUBLE_BLACK);
            ptr = parent;
          } else {
            if (getColor(sibling->right) == BLACK) {
              setColor(sibling->left, BLACK);
              setColor(sibling, RED);
              rotateRight(sibling);
              sibling = parent->right;
            }
            setColor(sibling, parent->color);
            setColor(parent, BLACK);
            setColor(sibling->right, BLACK);
            rotateLeft(parent);
            break;
          }
        }
      } else {
        sibling = parent->left;
        if (getColor(sibling) == RED) {
          setColor(sibling, BLACK);
          setColor(parent, RED);
          rotateRight(parent);
        } else {
          if (getColor(sibling->left) == BLACK && getColor(sibling->right) == BLACK) {
            setColor(sibling, RED);
            if (getColor(parent) == RED)
              setColor(parent, BLACK);
            else
              setColor(parent, DOUBLE_BLACK);
            ptr = parent;
          } else {
            if (getColor(sibling->left) == BLACK) {
              setColor(sibling->right, BLACK);
              setColor(sibling, RED);
              rotateLeft(sibling);
              sibling = parent->left;
            }
            setColor(sibling, parent->color);
            setColor(parent, BLACK);
            setColor(sibling->left, BLACK);
            rotateRight(parent);
            break;
          }
        }
      }
    }
    if (node == node->parent->left)
      node->parent->left = nullptr;
    else
      node->parent->right = nullptr;
    delete(node);
    setColor(root, BLACK);
  }
}

LimitNode *RBTree::minValueNode(LimitNode *&node) {
  LimitNode *ptr = node;

  while (ptr->left != nullptr)
    ptr = ptr->left;

  return ptr;
}

LimitNode* RBTree::maxValueNode(LimitNode *&node) {
  LimitNode *ptr = node;

  while (ptr->right != nullptr)
    ptr = ptr->right;

  return ptr;
}

void RBTree::inorderBST(LimitNode *&ptr) {
  if (ptr == nullptr)
    return;

  inorderBST(ptr->left);
  cout << ptr->limit_price << " " << ptr->color << endl;
  inorderBST(ptr->right);
}

void RBTree::inorder() {
  inorderBST(root);
}

void RBTree::print() {
  prettyPrint(root);
}

void RBTree::prettyPrint(LimitNode* root, string prefix, bool isLeft) {
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