#pragma once

#include "core/record.h"
#include <string>
#include <vector>
#include <map>
#include <optional>
#include <fstream>

class SSTable {
public:
    static std::string build(const std::string& path, 
                             uint64_t sstable_id, 
                             const std::vector<Record>& records);

    SSTable(const std::string& file_path); 
    ~SSTable();
    
    SSTable(const SSTable&) = delete;
    SSTable& operator=(const SSTable&) = delete;

    std::optional<Record> get(const std::string& key);

private:
    void load_index();

    std::string path;
    std::ifstream file_stream;
    std::map<std::string, uint64_t> index;
};