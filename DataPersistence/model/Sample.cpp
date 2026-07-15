#include "model/Sample.h"

nlohmann::json Sample::toJson() const {
    return {
        {"id",                  id},
        {"name",                name},
        {"avg_production_time", avgProductionTime},
        {"yield_rate",          yieldRate},
        {"stock",               stock}
    };
}

Sample Sample::fromJson(const nlohmann::json& j) {
    Sample s;
    s.id                = j.at("id").get<std::string>();
    s.name              = j.at("name").get<std::string>();
    s.avgProductionTime = j.at("avg_production_time").get<double>();
    s.yieldRate         = j.at("yield_rate").get<double>();
    s.stock             = j.at("stock").get<int>();
    return s;
}
