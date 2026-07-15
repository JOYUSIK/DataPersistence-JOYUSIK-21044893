#pragma once
#include <vector>
#include <optional>
#include <string>
#include "model/Order.h"

class OrderRepository {
public:
    OrderRepository();

    std::vector<Order> findAll() const;
    std::optional<Order> findById(const std::string& orderId) const;
    Order save(Order order);
    bool update(const Order& order);
    bool remove(const std::string& orderId);

private:
    std::string dataPath_;

    std::vector<Order> load() const;
    void persist(const std::vector<Order>& orders) const;
    std::string nextOrderId(const std::vector<Order>& orders) const;
};
