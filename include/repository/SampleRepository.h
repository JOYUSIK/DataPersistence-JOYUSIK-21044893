#pragma once
#include <string>
#include <vector>
#include <optional>
#include "model/Sample.h"
#include "repository/JsonStorage.h"

class SampleRepository {
public:
    explicit SampleRepository(const std::string& filePath)
        : storage_(filePath) {}

    std::vector<Sample> findAll()                        { return storage_.findAll(); }
    std::optional<Sample> findById(const std::string& id){ return storage_.findById(id); }
    void save(const Sample& s)                           { storage_.save(s); }
    bool update(const Sample& s)                         { return storage_.update(s); }
    bool remove(const std::string& id)                   { return storage_.remove(id); }

private:
    JsonStorage<Sample> storage_;
};
