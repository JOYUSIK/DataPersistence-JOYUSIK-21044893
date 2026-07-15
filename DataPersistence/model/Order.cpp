#include "model/Order.h"

nlohmann::json Order::toJson() const {
    return {
        {"order_id",   orderId},
        {"sample_id",  sampleId},
        {"customer",   customer},
        {"quantity",   quantity},
        {"status",     status},
        {"created_at", createdAt}
    };
}

Order Order::fromJson(const nlohmann::json& j) {
    Order o;
    o.orderId   = j.at("order_id").get<std::string>();
    o.sampleId  = j.at("sample_id").get<std::string>();
    o.customer  = j.at("customer").get<std::string>();
    o.quantity  = j.at("quantity").get<int>();
    o.status    = j.at("status").get<std::string>();
    o.createdAt = j.at("created_at").get<std::string>();
    return o;
}
