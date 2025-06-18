#pragma once

#include "core/record.h"
#include "core/skiplist.h"
#include <optional>
#include <vector>
#include <memory>

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
    std::unique_ptr<SkipList> table;
};