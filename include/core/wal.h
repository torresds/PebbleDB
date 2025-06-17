#pragma once
#include "core/record.h"
#include <fstream>
#include <string>
#include <vector>

class WAL {
public:
    WAL(const std::string& wal_path);
    ~WAL();

    WAL(const WAL&) = delete;
    WAL& operator=(const WAL&) = delete;

    void append(const Record& rec);
    std::vector<Record> recover();
    void clear();

private:
    std::string path;
    std::ofstream out_stream;
};