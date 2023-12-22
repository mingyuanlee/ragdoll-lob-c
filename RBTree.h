#ifndef RED_BLACK_TREE_RBTREE_H
#define RED_BLACK_TREE_RBTREE_H

#include <unordered_map>
#include <string>

enum Color {RED, BLACK, DOUBLE_BLACK};
enum TreeType {BID, ASK};

class LimitNode;

class OrderNode {
public:
  int oid;
  int limit_price;
  int volume;
  int owner;

  OrderNode *prev;
  OrderNode *next;
  LimitNode *limit_node;

  OrderNode(int, int, int, int, OrderNode *, OrderNode *, LimitNode *);
};

class LimitNode {
public:
  int limit_price;
  int total_volume;
  int size;
  LimitNode *left;
  LimitNode *right;
  LimitNode *parent;
  OrderNode *head;
  OrderNode *tail;
  bool color;
  LimitNode(int limit_price);

  OrderNode *insert_order(int oid, int volume, int owner);
  void delete_order(OrderNode *);
  OrderNode *pop_front_order();
  // OrderNode *peek_front_order();
  void front_order_deduct_volume(int volume);
};

class RBTree
{
private:
  LimitNode *root;
protected:
  void rotateLeft(LimitNode *&);
  void rotateRight(LimitNode *&);
  void fixInsertRBTree(LimitNode *&);
  void fixDeleteRBTree(LimitNode *&);
  void inorderBST(LimitNode *&);
  int getColor(LimitNode *&);
  void setColor(LimitNode *&, int);
  LimitNode *minValueNode(LimitNode *&);
  LimitNode *maxValueNode(LimitNode *&);
  LimitNode* insertBST(LimitNode *&, LimitNode *&);
  LimitNode* deleteBST(LimitNode *&, int);
  void prettyPrint(LimitNode* root, std::string prefix = "", bool isLeft = false);
public:
  int instrument; // instrument id
  TreeType type; // bid or ask
  std::unordered_map<int, LimitNode*> limit_map; // limit price -> LimitNode
  std::unordered_map<int, OrderNode*> order_map; // order id -> Order

  RBTree(int, TreeType);
  LimitNode *insertLimitPrice(int limit_price);
  void deleteLimitPrice(int limit_price);
  void deleteOrder(int oid);
  void insertOrder(int oid, int volume, int owner, int limit_price);
  LimitNode *minLimitNode();
  // void merge(RBTree);
  void inorder();
  void print();
};

#endif //RED_BLACK_TREE_RBTREE_H