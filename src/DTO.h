#ifndef DTO_H
#define DTO_H

class OrderInfo {
public:
  int oid;
  int limit_price;
  int volume;
  OrderInfo(int oid, int limit_price, int volume) : oid(oid), limit_price(limit_price), volume(volume) {}
};

class PriceInfo {
public:
  int limit_price;
  int total_volume;
  int size;
  PriceInfo(int limit_price, int total_volume, int size) : limit_price(limit_price), total_volume(total_volume), size(size) {}
};

#endif // DTO_H