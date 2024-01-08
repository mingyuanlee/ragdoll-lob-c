#include <iostream>
#include "Utils.h"
#include "LimitNode.h"

void Utils::print_linked_list(OrderNode *h, OrderNode *t) {
  OrderNode *curr = h;
  while (curr != nullptr) {
    std::cout << curr->oid << " ";
    curr = curr->next;
  }
  std::cout << std::endl;
}