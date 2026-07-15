#pragma once
#include <string>
#include "third_party/json.hpp"

struct Sample {
    std::string id;
    std::string name;
    double avgProductionTime;
    double yieldRate;
    int stock;

    nlohmann::json toJson() const;
    static Sample fromJson(const nlohmann::json& j);
};
