# 🪨 PebbleDB

[![License](https://img.shields.io/badge/license-MIT-blue.svg)](https://opensource.org/licenses/MIT)
[![Language](https://img.shields.io/badge/language-C%2B%2B17-purple.svg)](https://isocpp.org/)
[![Build](https://img.shields.io/badge/build-CMake-green.svg)](https://cmake.org/)

A hobbyist Key-Value storage engine, inspired by LevelDB/RocksDB, built from scratch in modern C++17 to explore database internals.

## About

PebbleDB was born from a passion for understanding how high-performance storage systems work under the hood. This project is a from-scratch implementation of the core components of a **Log-Structured Merge-Tree (LSM-Tree)**, aiming to demystify the concepts behind modern NoSQL databases.

It is first and foremost a **learning project**, where clarity and a step-by-step implementation of advanced concepts take precedence over being fully production-ready.

## Architecture

PebbleDB is implemented using a Log-Structured Merge-Tree (LSM-Tree) architecture. The lifecycle of a write operation is as follows:

1.  **Write-Ahead Log (WAL):** The operation is first written to a sequential log on disk to ensure durability and allow for recovery in case of a crash.
2.  **MemTable:** The record is then inserted into an in-memory table (the MemTable), which uses a **Skip List** to keep keys sorted and allow for very fast writes.
3.  **SSTable Flush:** When the MemTable reaches a certain size threshold, its sorted contents are flushed to a persistent, immutable file on disk called an **SSTable** (Sorted String Table).
4.  **Read Path:** Read operations first check the MemTable for the most recent data. If not found, they search through the SSTables on disk, from newest to oldest.

## Features

-   **Core operations:** Full support for `put`, `get`, and `del` (using tombstones).
-   **Optimized In-Memory Engine:** Utilizes a **Skip List** for the MemTable, ensuring `O(log N)` insertions and lookups.
-   **Crash safety:** The **Write-Ahead Log (WAL)** guarantees that no acknowledged write is lost, recovering the MemTable state on restart.
-   **Disk-based Persistence:** A system of immutable **SSTables** with on-disk indexes for fast lookups.
-   **Automatic Flush:** The MemTable is automatically persisted to a new SSTable when it reaches its size limit.
-   **Command-Line Interface (CLI):** A simple client (`./pebble`) for direct interaction and testing.

## Getting started

### Prerequisites
* A C++17 compliant compiler (g++, Clang)
* CMake (version 3.14 or higher)
* A build tool like `make` or `ninja`

### Building from source
```bash
# 1. Clone the repository
git clone https://github.com/torresds/PebbleDB.git
cd pebbledb

# 2. Create the build directory
mkdir build
cd build

# 3. Configure with CMake and compile
cmake ..
make
````

The CLI executable will be available at `build/pebble`.

### Usage

The CLI allows for direct interaction with the database engine. The data directory (`data/`) will be created automatically.

```bash
# Put or update a key
./build/pebble put user:101 "Alice"

# Get a key
./build/pebble get user:101
# Output: Alice

# Delete a key
./build/pebble del user:101

# Get a deleted key
./build/pebble get user:101
# Output: ℹ️ Info: Key 'user:101' not found.
```

## Roadmap
This project is under active development. The future focus is on adding core database features and improving robustness.
  - [ ] **Compaction:** Implement the compaction process to merge SSTables, permanently remove deleted data, and optimize disk usage and read performance.
  - [ ] **Iterators & Scans:** Create iterators to allow for scanning key ranges and searching by prefix.
  - [ ] **Block Cache:** Implement an in-memory cache for SSTable data blocks to accelerate repeated reads.
  - [ ] **Network Layer:** Expose the database over a network API (HTTP/JSON or TCP), likely using Go or Rust for the server layer.
  - [ ] **Complex Data Support:** Add a logical layer to support JSON documents with **secondary indexes**.
  - [ ] **Testing:** Greatly expand the unit test suite to cover all components and edge cases.
  - [ ] **Concurrency:** Add thread-safety using `std::shared_mutex` to allow for concurrent operations.

## Contributing

As a learning project, contributions, suggestions, and corrections are very welcome\! Please feel free to open an issue to discuss a bug or a new feature.

## License

This project is licensed under the **MIT License**. See the `LICENSE` file for details.
