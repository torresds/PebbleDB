#include "core/wal.h"
#include <filesystem>

WAL::WAL(const std::string& wal_path) : path(wal_path) {
    std::filesystem::path p(path);
    if (p.has_parent_path()) {
        std::filesystem::create_directories(p.parent_path());
    }

    out_stream.open(path, std::ios::binary | std::ios::app);
    if (!out_stream) {
        throw std::runtime_error("Failed to open WAL file for writing: " + path);
    }
}

WAL::~WAL() {
    if (out_stream.is_open()) {
        out_stream.close();
    }
}

void WAL::append(const Record& rec) {
    rec.serialize(out_stream);
    out_stream.flush();
}

std::vector<Record> WAL::recover() {
    std::vector<Record> records;
    std::ifstream in_stream(path, std::ios::binary);

    if (!in_stream) {
        return records;
    }

    while (in_stream.peek() != EOF) {
        records.push_back(Record::deserialize(in_stream));
    }
    
    return records;
}

void WAL::clear() {
    out_stream.close();
    out_stream.open(path, std::ios::binary | std::ios::trunc);
    if (!out_stream) {
        throw std::runtime_error("Failed to clear WAL file: " + path);
    }
}