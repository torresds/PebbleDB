#pragma once
#include <string>
#include <fstream>
#include <cstdint>
#include <chrono>

struct Record {
    std::string key;
    std::string value;
    uint64_t timestamp;
    bool deleted;

    Record(std::string k = "", std::string v = "", bool del = false)
        : key(std::move(k)), 
          value(std::move(v)), 
          deleted(del) {
        timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()
        ).count();
    }

    void serialize(std::ostream& out) const;
    static Record deserialize(std::istream& in);
};