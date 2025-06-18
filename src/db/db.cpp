#include "db/db.h"
#include "storage/sstable.h"
#include <iostream>
#include <filesystem>
#include <algorithm>

constexpr size_t MEMTABLE_MAX_SIZE_BYTES = 1000 * 1024; // 1 MB

PebbleDB::PebbleDB(const std::string& db_path) : path(db_path), sstable_counter(1) {
    memtable = std::make_unique<MemTable>();
    
    std::filesystem::create_directories(path);
    load_sstables();
    
    wal = std::make_unique<WAL>(path + "/wal.log");
    recover(); 
}

PebbleDB::~PebbleDB() {
    if (memtable->size_bytes() > 0) {
        flush();
    }
}

void PebbleDB::put(const std::string& key, const std::string& value) {
    Record rec(key, value, false);
    wal->append(rec);
    memtable->put(key, value);

    // dispara o flush p/ memtable cheia
    if (memtable->size_bytes() >= MEMTABLE_MAX_SIZE_BYTES) {
        flush();
    }
}

void PebbleDB::del(const std::string& key) {
    Record rec(key, "", true);
    wal->append(rec);
    memtable->del(key);
    
    if (memtable->size_bytes() >= MEMTABLE_MAX_SIZE_BYTES) {
        flush();
    }
}

std::optional<std::string> PebbleDB::get(const std::string& key) {
    auto mem_record = memtable->get(key);
    if (mem_record.has_value()) {
        if (mem_record->deleted) {
            return std::nullopt;
        }
        return mem_record->value;
    }

    for (const auto& sstable : sstables) {
        auto sst_record = sstable->get(key);
        if (sst_record.has_value()) {
            if (sst_record->deleted) {
                return std::nullopt; 
            }
            return sst_record->value;
        }
    }

    // 3. Se não encontrou em lugar nenhum
    return std::nullopt;
}


void PebbleDB::recover() {
    std::cout << "Recuperando dados do WAL..." << std::endl;
    auto records = wal->recover();
    for (const auto& rec : records) {
        if (rec.deleted) {
            memtable->del(rec.key);
        } else {
            memtable->put(rec.key, rec.value);
        }
    }
    std::cout << records.size() << " registros recuperados." << std::endl;
}

void PebbleDB::flush() {
    std::cout << "MemTable cheia! Iniciando flush..." << std::endl;
    std::string sstable_path = SSTable::build(path + "/sstables", sstable_counter, memtable->dump());
    std::cout << "Nova SSTable criada em: " << sstable_path << std::endl;
    
    sstables.insert(sstables.begin(), std::make_unique<SSTable>(sstable_path));
    
    sstable_counter++;
    memtable->clear();
    wal->clear();
    std::cout << "Flush concluído. MemTable e WAL limpos." << std::endl;
}

void PebbleDB::load_sstables() {
    std::string sst_path = path + "/sstables";
    if (!std::filesystem::exists(sst_path)) return;

    std::vector<std::string> paths;
    for (const auto& entry : std::filesystem::directory_iterator(sst_path)) {
        if (entry.path().extension() == ".sst") {
            paths.push_back(entry.path().string());
        }
    }

    std::sort(paths.rbegin(), paths.rend());

    for (const auto& p : paths) {
        std::cout << "Carregando SSTable: " << p << std::endl;
        sstables.push_back(std::make_unique<SSTable>(p));
    }
    
    if (!paths.empty()) {
        std::string last_file = std::filesystem::path(paths[0]).stem().string();
        sstable_counter = std::stoull(last_file) + 1;
    }
}
