enable_testing()

# Require version
cmake_minimum_required(VERSION 3.11)

# Set project name
project(asio_grpc_server CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_GENERATOR Ninja)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
# set(PROTOFILES_OUTPUT_DIR ${CMAKE_BINARY_DIR}/bin/proto/)

# Add conan packages
include(${CMAKE_BINARY_DIR}/conanbuildinfo_multi.cmake)
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
include(${CMAKE_BINARY_DIR}/conan_paths.cmake)
conan_basic_setup(TARGET KEEP_RPATHS)

# Enable testing with catch2
include(CTest)
include(${CONAN_BUILD_DIRS_CATCH2_DEBUG}/Catch.cmake)
include(${CONAN_BUILD_DIRS_PROTOBUF}/protobuf-generate.cmake)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

# Set global include headers
add_subdirectory(pingpong)

add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE src/main.cpp)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

get_property(importTargets DIRECTORY "${CMAKE_SOURCE_DIR}" PROPERTY IMPORTED_TARGETS)
message(STATUS "${importTargets}")