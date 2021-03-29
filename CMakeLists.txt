cmake_minimum_required(VERSION 3.4)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED TRUE)
set(CMAKE_C_EXTENSIONS OFF)

project(DataStructures)
add_library(DataStructures
    "src/BaseOpenHashTable.c"
    "src/BaseVector.c"
)
target_include_directories(DataStructures 
    PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/DataStructures>"
    "$<INSTALL_INTERFACE:include>"
)

install(
    TARGETS DataStructures
    EXPORT DataStructuresTargets
    LIBRARY DESTINATION "lib"
    ARCHIVE DESTINATION "lib"
    RUNTIME DESTINATION "bin"
    INCLUDES DESTINATION "include"
)

install(
    FILES
    "include/DataStructures/BaseOpenHashTable.h"
    "include/DataStructures/BaseVector.h"
    "include/DataStructures/OpenHashTable.h"
    "include/DataStructures/Vector.h"
    DESTINATION "include/DataStructures"
)

install(
    EXPORT DataStructuresTargets
    FILE DataStructuresTargets.cmake
    NAMESPACE DataStructures::
    DESTINATION "lib/cmake/DataStructures"
)

