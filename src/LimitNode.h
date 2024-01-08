#ifndef LIMIT_NODE_H
#define LIMIT_NODE_H

enum Color {RED, BLACK};

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
  /* tree properties */
  bool color;
  LimitNode *left;
  LimitNode *right;

  /* key */
  int limit_price;
  
  /* data */
  int total_volume;
  int size;
  OrderNode *head;
  OrderNode *tail;

  /* constructor */
  LimitNode(int limit_price);

  /* order operations */
  OrderNode *insert_order(int oid, int volume, int owner);
  void delete_order(OrderNode *);
  OrderNode *pop_front_order();
  void front_order_deduct_volume(int volume);
};

#endif // LIMIT_NODE_H