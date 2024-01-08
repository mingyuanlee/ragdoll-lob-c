#include <iostream>
#include "Utils.h"
#include "LimitNode.h"

void Utils::print_linked_list(OrderNode *h, OrderNode *t) {
  OrderNode *curr = h;
  int total = 0;
  while (curr != nullptr) {
    std::cout << curr->oid << " ";
    total++;
    curr = curr->next;
  }
  std::cout << "total " << total << std::endl;
}