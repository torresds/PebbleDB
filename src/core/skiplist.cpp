#include "core/skiplist.h"
#include <chrono>

SkipList::Node::Node(int level, Record rec) : record(std::move(rec)) {
    forward.resize(level + 1);
}

SkipList::SkipList() : current_level(0), approximate_size(0) {
    rng.seed(std::chrono::high_resolution_clock::now().time_since_epoch().count());
    dist = std::uniform_int_distribution<int>(0, 1);

    Record head_record{"", "", true};
    head = std::make_shared<Node>(SKIPLIST_MAX_LEVEL, head_record);
}

int SkipList::random_level() {
    int level = 0;
    while (dist(rng) == 1 && level < SKIPLIST_MAX_LEVEL) {
        level++;
    }
    return level;
}

void SkipList::put(const std::string& key, const std::string& value) {
    Record new_record(key, value, false);
    update_internal(new_record);
}

void SkipList::del(const std::string& key) {
    Record tombstone(key, "", true);
    update_internal(tombstone);
}

void SkipList::update_internal(const Record& record) {
    std::vector<Node*> update(SKIPLIST_MAX_LEVEL + 1, nullptr);
    Node* current = head.get();

    for (int i = current_level; i >= 0; i--) {
        while (current->forward[i] && current->forward[i]->record.key < record.key) {
            current = current->forward[i].get();
        }
        update[i] = current;
    }

    current = current->forward[0].get();

    if (current && current->record.key == record.key) {
        approximate_size -= (current->record.key.size() + current->record.value.size());
        current->record = record;
        approximate_size += (record.key.size() + record.value.size());
        return;
    }

    int new_lvl = random_level();
    if (new_lvl > current_level) {
        for (int i = current_level + 1; i <= new_lvl; i++) {
            update[i] = head.get();
        }
        current_level = new_lvl;
    }

    auto new_node = std::make_shared<Node>(new_lvl, record);
    approximate_size += (record.key.size() + record.value.size());

    for (int i = 0; i <= new_lvl; i++) {
        new_node->forward[i] = std::move(update[i]->forward[i]);
        update[i]->forward[i] = new_node;
    }
}


std::optional<Record> SkipList::get(const std::string& key) const {
    Node* current = head.get();
    for (int i = current_level; i >= 0; i--) {
        while (current->forward[i] && current->forward[i]->record.key < key) {
            current = current->forward[i].get();
        }
    }
    current = current->forward[0].get();

    if (current && current->record.key == key) {
        return current->record;
    }
    return std::nullopt;
}

std::vector<Record> SkipList::dump() const {
    std::vector<Record> records;
    Node* current = head->forward[0].get();
    while (current) {
        records.push_back(current->record);
        current = current->forward[0].get();
    }
    return records;
}

size_t SkipList::size_bytes() const {
    return approximate_size;
}