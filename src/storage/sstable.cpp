#include "storage/sstable.h"
#include <fstream>
#include <map>
#include <filesystem>
#include <iomanip>
#include <stdexcept>
#include <vector>

SSTable::~SSTable() = default;

SSTable::SSTable(const std::string& file_path) : path(file_path) {
    file_stream.open(path, std::ios::binary);
    if (!file_stream) {
        throw std::runtime_error("Falha ao abrir SSTable para leitura: " + path);
    }
    load_index();
}

std::optional<Record> SSTable::get(const std::string& key) {
    auto it = index.find(key);
    if (it == index.end()) {
        return std::nullopt;
    }

    uint64_t offset = it->second;
    file_stream.seekg(offset);
    
    return Record::deserialize(file_stream);
}


void SSTable::load_index() {
    file_stream.seekg(0, std::ios::end);
    std::streampos file_size = file_stream.tellg();
    if (file_size < 16) {
        throw std::runtime_error("Arquivo SSTable inválido ou corrompido (muito pequeno): " + path);
    }

    file_stream.seekg(-16, std::ios::end); 
    uint64_t index_offset, index_size;
    file_stream.read(reinterpret_cast<char*>(&index_offset), sizeof(index_offset));
    file_stream.read(reinterpret_cast<char*>(&index_size), sizeof(index_size));

    file_stream.seekg(index_offset);
    std::vector<char> index_buffer(index_size);
    file_stream.read(index_buffer.data(), index_size);

    size_t current_pos = 0;
    while (current_pos < index_size) {
        size_t key_len = *reinterpret_cast<const size_t*>(index_buffer.data() + current_pos);
        current_pos += sizeof(size_t);

        std::string key(index_buffer.data() + current_pos, key_len);
        current_pos += key_len;

        uint64_t offset = *reinterpret_cast<const uint64_t*>(index_buffer.data() + current_pos);
        current_pos += sizeof(uint64_t);

        index[key] = offset;
    }
}


std::string SSTable::build(const std::string& path, 
                             uint64_t sstable_id, 
                             const std::vector<Record>& records) {
    
    std::filesystem::create_directories(path);

    std::stringstream ss;
    ss << std::setw(4) << std::setfill('0') << sstable_id;
    std::string filename = path + "/" + ss.str() + ".sst";

    std::ofstream out(filename, std::ios::binary);
    if (!out) {
        throw std::runtime_error("Algo deu errado ao criar o arquivo de SSTable: " + filename);
    }

    std::map<std::string, uint64_t> index;
    uint64_t current_offset = 0;

    for (const auto& record : records) {
        index[record.key] = current_offset;
        record.serialize(out);
        current_offset = out.tellp();
    }

    uint64_t index_offset = current_offset;

    for (const auto& [key, offset] : index) {
        size_t key_len = key.size();
        out.write(reinterpret_cast<const char*>(&key_len), sizeof(key_len));
        out.write(key.data(), key_len);
        out.write(reinterpret_cast<const char*>(&offset), sizeof(offset));
    }
    
    uint64_t index_size = static_cast<uint64_t>(out.tellp()) - index_offset;

    out.write(reinterpret_cast<const char*>(&index_offset), sizeof(index_offset));
    out.write(reinterpret_cast<const char*>(&index_size), sizeof(index_size));

    out.close();
    return filename;
}