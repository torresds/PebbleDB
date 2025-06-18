cmake_minimum_required(VERSION 3.14)
project(pebble CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2 -DNDEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")

add_compile_options(-Wall -Wextra -Wpedantic)

add_library(pebble_core STATIC
    src/core/record.cpp
    src/core/memtable.cpp
    src/core/wal.cpp
    src/core/skiplist.cpp
    src/db/db.cpp
    src/storage/sstable.cpp
)

target_include_directories(pebble_core PUBLIC include)


add_executable(pebble
    cli/main.cpp
)

target_link_libraries(pebble PRIVATE pebble_core)


enable_testing()

add_executable(pebble_tests
    tests/core/test_memtable.cpp
)

target_include_directories(pebble_tests PUBLIC tests)

target_link_libraries(pebble_tests PRIVATE pebble_core)

add_test(NAME UnitTests COMMAND pebble_tests)

message(STATUS "Unit tests enabled.")