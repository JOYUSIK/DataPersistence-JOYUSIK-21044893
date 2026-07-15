#pragma once
#include <string>
#include "third_party/json.hpp"

struct Order {
    std::string orderId;
    std::string sampleId;
    std::string customer;
    int quantity;
    std::string status;
    std::string createdAt;

    nlohmann::json toJson() const {
        return {
            {"order_id", orderId},
            {"sample_id", sampleId},
            {"customer", customer},
            {"quantity", quantity},
            {"status", status},
            {"created_at", createdAt}
        };
    }

    static Order fromJson(const nlohmann::json& j) {
        Order o;
        o.orderId   = j.at("order_id").get<std::string>();
        o.sampleId  = j.at("sample_id").get<std::string>();
        o.customer  = j.at("customer").get<std::string>();
        o.quantity  = j.at("quantity").get<int>();
        o.status    = j.at("status").get<std::string>();
        o.createdAt = j.at("created_at").get<std::string>();
        return o;
    }
};
