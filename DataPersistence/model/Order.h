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

    nlohmann::json toJson() const;
    static Order fromJson(const nlohmann::json& j);
};
