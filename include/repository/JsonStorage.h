#pragma once
#include <string>
#include <vector>
#include <optional>
#include <fstream>
#include <algorithm>
#include "third_party/json.hpp"

template <typename T>
class JsonStorage {
public:
    explicit JsonStorage(const std::string& filePath) : filePath_(filePath) {}

    std::vector<T> findAll() {
        auto arr = load();
        std::vector<T> result;
        result.reserve(arr.size());
        for (const auto& item : arr)
            result.push_back(T::fromJson(item));
        return result;
    }

    std::optional<T> findById(const std::string& id) {
        for (const auto& item : findAll())
            if (item.id == id)
                return item;
        return std::nullopt;
    }

    void save(const T& entity) {
        auto arr = load();
        arr.push_back(entity.toJson());
        persist(arr);
    }

    bool update(const T& entity) {
        auto arr = load();
        for (auto& item : arr) {
            if (item["id"] == entity.id) {
                item = entity.toJson();
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
                [&](const nlohmann::json& j) { return j["id"] == id; }),
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
