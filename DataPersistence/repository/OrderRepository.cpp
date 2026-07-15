#include "repository/OrderRepository.h"
#include <fstream>
#include <filesystem>
#include <stdexcept>
#include <ctime>

namespace fs = std::filesystem;

static const std::string DATA_PATH = "data/orders.json";

OrderRepository::OrderRepository() : dataPath_(DATA_PATH) {
    fs::create_directories("data");
    if (!fs::exists(dataPath_)) {
        std::ofstream f(dataPath_);
        f << "[]";
    }
}

std::vector<Order> OrderRepository::findAll() const {
    return load();
}

std::optional<Order> OrderRepository::findById(const std::string& orderId) const {
    for (const auto& o : load()) {
        if (o.orderId == orderId) return o;
    }
    return std::nullopt;
}

Order OrderRepository::save(Order order) {
    auto orders = load();
    order.orderId = nextOrderId(orders);
    orders.push_back(order);
    persist(orders);
    return order;
}

bool OrderRepository::update(const Order& order) {
    auto orders = load();
    for (auto& stored : orders) {
        if (stored.orderId == order.orderId) {
            stored = order;
            persist(orders);
            return true;
        }
    }
    return false;
}

bool OrderRepository::remove(const std::string& orderId) {
    auto orders = load();
    auto before = orders.size();
    orders.erase(
        std::remove_if(orders.begin(), orders.end(),
            [&orderId](const Order& o) { return o.orderId == orderId; }),
        orders.end());
    if (orders.size() == before) return false;
    persist(orders);
    return true;
}

std::vector<Order> OrderRepository::load() const {
    std::ifstream f(dataPath_);
    if (!f.is_open()) throw std::runtime_error("데이터 파일을 열 수 없습니다: " + dataPath_);
    auto j = nlohmann::json::parse(f);
    std::vector<Order> orders;
    for (const auto& entry : j)
        orders.push_back(Order::fromJson(entry));
    return orders;
}

void OrderRepository::persist(const std::vector<Order>& orders) const {
    nlohmann::json j = nlohmann::json::array();
    for (const auto& o : orders) j.push_back(o.toJson());
    std::ofstream f(dataPath_);
    f << j.dump(2);
}

std::string OrderRepository::nextOrderId(const std::vector<Order>& orders) const {
    std::time_t t = std::time(nullptr);
    std::tm tm{};
    localtime_s(&tm, &t);

    char dateBuf[16];
    std::strftime(dateBuf, sizeof(dateBuf), "%Y%m%d", &tm);

    int maxSeq = 0;
    std::string prefix = std::string("ORD-") + dateBuf + "-";
    for (const auto& o : orders) {
        if (o.orderId.rfind(prefix, 0) == 0) {
            int seq = std::stoi(o.orderId.substr(prefix.size()));
            if (seq > maxSeq) maxSeq = seq;
        }
    }
    char buf[32];
    std::snprintf(buf, sizeof(buf), "ORD-%s-%04d", dateBuf, maxSeq + 1);
    return buf;
}
