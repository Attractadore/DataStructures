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
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
    "$<INSTALL_INTERFACE:include>"
    PRIVATE
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/DataStructures>"
)

install(
    TARGETS DataStructures 
    EXPORT DataStructuresTargets
    ARCHIVE DESTINATION lib
)

install(
    FILES
    "include/DataStructures/BaseFunc.h"
    "include/DataStructures/BaseOpenHashTable.h"
    "include/DataStructures/OpenHashTable.h"
    "include/DataStructures/BaseVector.h"
    "include/DataStructures/Vector.h"
    "include/DataStructures/BaseBinHeap.h"
    "include/DataStructures/BinHeap.h"
    "include/DataStructures/BaseCachePolicy.h"
    "include/DataStructures/CachePolicyAddResult.h"
    "include/DataStructures/CachePolicy.h"
    "include/DataStructures/BaseDoubleList.h"
    "include/DataStructures/BaseMonoList.h"
    DESTINATION
    "include/DataStructures"
)

install(
    EXPORT DataStructuresTargets
    FILE DataStructuresTargets.cmake
    NAMESPACE DataStructures::
    DESTINATION "lib/cmake/DataStructures"
)

add_library(DataStructures::DataStructures ALIAS DataStructures)

