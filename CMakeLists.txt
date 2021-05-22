cmake_minimum_required(VERSION 3.4)

project(DataStructures)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED TRUE)
set(CMAKE_C_EXTENSIONS OFF)

include(CTest)
include("cmake/CMakeTemplates/AddTestSuite.cmake")
add_subdirectory("modules")
add_subdirectory("test")

add_library(DataStructures
    "src/BaseOpenHashTable.c"
    "src/BaseChainHashTable.c"
    "src/BaseDoubleList.c"
    "src/BaseMonoList.c"
    "src/BaseVector.c"
    "src/BaseBinHeap.c"
    "src/BaseCachePolicy.c"
    "src/DummyCache.c"
    "src/LRUCache.c"
    "src/LIRSCache.c"
)

target_include_directories(DataStructures 
    INTERFACE
    "${PROJECT_SOURCE_DIR}/include"
    PRIVATE
    "${PROJECT_SOURCE_DIR}/include/DataStructures"
)
