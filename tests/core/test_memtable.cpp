#define CATCH_CONFIG_MAIN

#include "lib/catch.hpp" 
#include "core/memtable.h"
#include "core/record.h"

TEST_CASE("MemTable: inserção, atualização e busca", "[memtable]") {
    MemTable mem;

    SECTION("put e get de um novo registro") {
        mem.put("user:1", "João");
        auto record = mem.get("user:1");

        REQUIRE(record.has_value());
        REQUIRE(record->key == "user:1");
        REQUIRE(record->value == "João");
        REQUIRE(record->deleted == false);
    }

    SECTION("put atualiza um registro existente") {
        mem.put("user:1", "João Silva");
        mem.put("user:1", "João da Silva");

        auto record = mem.get("user:1");
        REQUIRE(record.has_value());
        REQUIRE(record->value == "João da Silva");
    }

    SECTION("get em uma chave inexistente retorna nullopt") {
        auto record = mem.get("chave-que-nao-existe");
        REQUIRE_FALSE(record.has_value());
    }

    SECTION("del marca um registro como deletado (tombstone)") {
        mem.put("user:2", "Maria");
        mem.del("user:2");

        auto record = mem.get("user:2");
        REQUIRE(record.has_value());
        REQUIRE(record->deleted == true);
        REQUIRE(record->value.empty());
    }
}

TEST_CASE("MemTable: Dump e Clear", "[memtable]") {
    MemTable mem;
    mem.put("user:1", "João");
    mem.put("user:2", "Maria");

    SECTION("dump retorna todos os registros") {
        auto records = mem.dump();
        REQUIRE(records.size() == 2);
    }

    SECTION("clear limpa a tabela") {
        mem.clear();
        auto records = mem.dump();
        REQUIRE(records.empty());
        REQUIRE_FALSE(mem.get("user:1").has_value());
    }
}