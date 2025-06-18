#pragma once

#include "core/memtable.h"
#include "core/wal.h"
#include "storage/sstable.h" 
#include <string>
#include <optional>
#include <memory>
#include <vector>

class PebbleDB {
public:
    PebbleDB(const std::string& db_path);
    ~PebbleDB();

    void put(const std::string& key, const std::string& value);
    std::optional<std::string> get(const std::string& key);
    void del(const std::string& key);

private:
    void recover();
    void flush();
    void load_sstables();

    std::string path;
    std::unique_ptr<MemTable> memtable;
    std::unique_ptr<WAL> wal;
    uint64_t sstable_counter;
    
    std::vector<std::unique_ptr<SSTable>> sstables; 
};