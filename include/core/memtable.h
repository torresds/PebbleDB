#pragma once
#include "core/record.h"
#include <map>
#include <string>
#include <optional>
#include <vector>

class MemTable {
public:
    MemTable();

    void put(const std::string& key, const std::string& value);
    void del(const std::string& key);
    std::optional<Record> get(const std::string& key) const;

    std::vector<Record> dump() const;

    void clear();
    size_t size_bytes() const;

private:
    std::map<std::string, Record> table;
    size_t approximate_size;
};