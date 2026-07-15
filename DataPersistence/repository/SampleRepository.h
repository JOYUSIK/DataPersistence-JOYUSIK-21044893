#pragma once
#include <vector>
#include <optional>
#include <string>
#include "model/Sample.h"

class SampleRepository {
public:
    SampleRepository();

    std::vector<Sample> findAll() const;
    std::optional<Sample> findById(const std::string& id) const;
    Sample save(Sample sample);
    bool update(const Sample& sample);
    bool remove(const std::string& id);

private:
    std::string dataPath_;

    std::vector<Sample> load() const;
    void persist(const std::vector<Sample>& samples) const;
    std::string nextId(const std::vector<Sample>& samples) const;
};
