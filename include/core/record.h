#pragma once
#include <string>
#include <fstream>
#include <cstdint>

struct Record {
    std::string key;
    std::string value;
    uint64_t timestamp;
    bool deleted;

    void serialize(std::ofstream& out) const;
    static Record deserialize(std::ifstream& in);
};
