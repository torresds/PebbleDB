#include "core/memtable.h"

MemTable::MemTable() {
    table = std::make_unique<SkipList>();
}

void MemTable::put(const std::string& key, const std::string& value) {
    table->put(key, value);
}

void MemTable::del(const std::string& key) {
    table->del(key);
}

std::optional<Record> MemTable::get(const std::string& key) const {
    return table->get(key);
}

std::vector<Record> MemTable::dump() const {
    return table->dump();
}

void MemTable::clear() {
    table = std::make_unique<SkipList>();
}

size_t MemTable::size_bytes() const {
    return table->size_bytes();
}