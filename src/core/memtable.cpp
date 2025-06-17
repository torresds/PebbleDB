#include "core/memtable.h"

MemTable::MemTable() : approximate_size(0) {}

void MemTable::put(const std::string& key, const std::string& value) {
    approximate_size -= table.count(key) ? (table[key].key.size() + table[key].value.size()) : 0;
    
    Record rec(key, value, false);
    table[key] = rec;
    
    approximate_size += key.size() + value.size();
}

void MemTable::del(const std::string& key) {
    approximate_size -= table.count(key) ? (table[key].key.size() + table[key].value.size()) : 0;

    Record tombstone(key, "", true);
    table[key] = tombstone;

    approximate_size += key.size();
}

std::optional<Record> MemTable::get(const std::string& key) const {
    auto it = table.find(key);
    if (it != table.end()) {
        return it->second;
    }
    return std::nullopt;
}

std::vector<Record> MemTable::dump() const {
    std::vector<Record> records;
    records.reserve(table.size());
    for (const auto& pair : table) {
        records.push_back(pair.second);
    }
    return records;
}

void MemTable::clear() {
    table.clear();
    approximate_size = 0;
}

size_t MemTable::size_bytes() const {
    return approximate_size;
}