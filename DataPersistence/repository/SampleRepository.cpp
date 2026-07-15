#include "repository/SampleRepository.h"
#include <fstream>
#include <filesystem>
#include <stdexcept>

namespace fs = std::filesystem;

static const std::string DATA_PATH = "data/samples.json";

SampleRepository::SampleRepository() : dataPath_(DATA_PATH) {
    fs::create_directories("data");
    if (!fs::exists(dataPath_)) {
        std::ofstream f(dataPath_);
        f << "[]";
    }
}

std::vector<Sample> SampleRepository::findAll() const {
    return load();
}

std::optional<Sample> SampleRepository::findById(const std::string& id) const {
    for (const auto& s : load()) {
        if (s.id == id) return s;
    }
    return std::nullopt;
}

Sample SampleRepository::save(Sample sample) {
    auto samples = load();
    sample.id = nextId(samples);
    samples.push_back(sample);
    persist(samples);
    return sample;
}

bool SampleRepository::update(const Sample& sample) {
    auto samples = load();
    for (auto& stored : samples) {
        if (stored.id == sample.id) {
            stored = sample;
            persist(samples);
            return true;
        }
    }
    return false;
}

bool SampleRepository::remove(const std::string& id) {
    auto samples = load();
    auto before = samples.size();
    samples.erase(
        std::remove_if(samples.begin(), samples.end(),
            [&id](const Sample& s) { return s.id == id; }),
        samples.end());
    if (samples.size() == before) return false;
    persist(samples);
    return true;
}

std::vector<Sample> SampleRepository::load() const {
    std::ifstream f(dataPath_);
    if (!f.is_open()) throw std::runtime_error("데이터 파일을 열 수 없습니다: " + dataPath_);
    auto j = nlohmann::json::parse(f);
    std::vector<Sample> samples;
    for (const auto& entry : j)
        samples.push_back(Sample::fromJson(entry));
    return samples;
}

void SampleRepository::persist(const std::vector<Sample>& samples) const {
    nlohmann::json j = nlohmann::json::array();
    for (const auto& s : samples) j.push_back(s.toJson());
    std::ofstream f(dataPath_);
    f << j.dump(2);
}

std::string SampleRepository::nextId(const std::vector<Sample>& samples) const {
    if (samples.empty()) return "S-001";
    int maxNum = 0;
    for (const auto& s : samples) {
        int num = std::stoi(s.id.substr(2));
        if (num > maxNum) maxNum = num;
    }
    char buf[16];
    std::snprintf(buf, sizeof(buf), "S-%03d", maxNum + 1);
    return buf;
}
