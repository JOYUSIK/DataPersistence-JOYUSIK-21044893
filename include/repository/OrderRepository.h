#pragma once
#include <string>
#include <vector>
#include <optional>
#include <fstream>
#include <algorithm>
#include "model/Order.h"
#include "third_party/json.hpp"

class OrderRepository {
public:
    explicit OrderRepository(const std::string& filePath)
        : filePath_(filePath) {}

    std::vector<Order> findAll() {
        auto arr = load();
        std::vector<Order> result;
        result.reserve(arr.size());
        for (const auto& j : arr)
            result.push_back(Order::fromJson(j));
        return result;
    }

    std::optional<Order> findById(const std::string& id) {
        for (const auto& o : findAll())
            if (o.orderId == id) return o;
        return std::nullopt;
    }

    void save(const Order& o) {
        auto arr = load();
        arr.push_back(o.toJson());
        persist(arr);
    }

    bool update(const Order& o) {
        auto arr = load();
        for (auto& item : arr) {
            if (item["order_id"] == o.orderId) {
                item = o.toJson();
                persist(arr);
                return true;
            }
        }
        return false;
    }

    bool remove(const std::string& id) {
        auto arr = load();
        auto before = arr.size();
        arr.erase(
            std::remove_if(arr.begin(), arr.end(),
                [&](const nlohmann::json& j) { return j["order_id"] == id; }),
            arr.end());
        if (arr.size() == before) return false;
        persist(arr);
        return true;
    }

private:
    std::string filePath_;

    nlohmann::json load() {
        std::ifstream file(filePath_);
        if (!file.is_open()) return nlohmann::json::array();
        nlohmann::json arr;
        try { file >> arr; } catch (...) { return nlohmann::json::array(); }
        return arr;
    }

    void persist(const nlohmann::json& arr) {
        std::ofstream file(filePath_);
        file << arr.dump(2);
    }
};
