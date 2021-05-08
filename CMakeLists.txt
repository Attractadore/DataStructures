cmake_minimum_required(VERSION 3.4)

project(DataStructures)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED TRUE)
set(CMAKE_C_EXTENSIONS OFF)

add_library(DataStructures
    "src/BaseOpenHashTable.c"
    "src/BaseVector.c"
    "src/BaseCachePolicy.c"
    "src/DummyCache.c"
    "src/BaseMonoList.c"
#    "src/list.c"
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
)

install(
    FILES
    "include/DataStructures/BaseCompareFunc.h"
    "include/DataStructures/BaseHashFunc.h"
    "include/DataStructures/BaseOpenHashTable.h"
    "include/DataStructures/OpenHashTable.h"
    "include/DataStructures/BaseVector.h"
    "include/DataStructures/Vector.h"
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

