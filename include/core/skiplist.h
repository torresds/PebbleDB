#pragma once

#include "core/record.h"
#include <vector>
#include <optional>
#include <memory>
#include <random>

constexpr int SKIPLIST_MAX_LEVEL = 16;

class SkipList {
private:
    struct Node {
        Record record;
        std::vector<std::unique_ptr<Node>> forward;

        Node(int level, Record rec);
    };

public:
    SkipList();

    void put(const std::string& key, const std::string& value);
    void del(const std::string& key);
    std::optional<Record> get(const std::string& key) const;
    std::vector<Record> dump() const;
    size_t size_bytes() const;

private:
    int random_level();
    void update_internal(const Record& record);

    std::unique_ptr<Node> head;
    int current_level;
    size_t approximate_size;

    std::mt19937 rng;
    std::uniform_int_distribution<int> dist;
};