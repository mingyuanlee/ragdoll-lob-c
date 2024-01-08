#ifndef DTO_H
#define DTO_H

class OrderInfo {
  int oid;
  int limit_price;
  int volume;
public:
  OrderInfo(int oid, int limit_price, int volume) : oid(oid), limit_price(limit_price), volume(volume) {}
};

#endif // DTO_H