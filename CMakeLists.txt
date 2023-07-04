enable_testing()

# Require version
cmake_minimum_required(VERSION 3.11)

# Set project name
project(asio_grpc_server CXX)
# Set CXX standard properties
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# Set Ninja as default generator
set(CMAKE_GENERATOR Ninja)

# Setting executables, libraries and artifacts output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

#  
set(CMAKE_SKIP_RPATH TRUE)
set(THREADS_PREFER_PTHREAD_FLAG ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_Go_COMPILER $ENV{GOROOT}/bin/go)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_BINARY_DIR}" ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
# add go cmake build integration feature
include(cmake/flags.cmake)
include(cmake/CMakeGenerateGolangRPC.cmake)
include(cmake/golang.cmake)
# Packages are provided by conan
find_package(Boost "1.81" REQUIRED)
find_package(Threads REQUIRED)
find_package(gRPC CONFIG REQUIRED)
find_package(Protobuf "3.21.9" CONFIG REQUIRED)
find_package(asio-grpc CONFIG REQUIRED)
find_package(fmt "10" CONFIG REQUIRED)
find_package(range-v3 CONFIG REQUIRED)
find_package(GTest REQUIRED)
# Populate grpc and abseil google std lib for all project to simplify work with generated grpc
include_directories(${gRPC_INCLUDE_DIR})
include_directories(${absl_INCLUDE_DIR})

add_subdirectory(pingpong)
add_subdirectory(pingpong_go)
add_subdirectory(matrix_protofiles)

# sandbox
add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE src/main.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC
    boost::boost
    grpc::grpc
    fmt::fmt
    asio-grpc::asio-grpc
)
target_link_libraries(${PROJECT_NAME} PUBLIC
    Threads::Threads
    boost::boost
    grpc::grpc
    fmt::fmt
    asio-grpc::asio-grpc
)

# Log version and dirs of packages provided by conan
message(STATUS "boost version ${Boost_VERSION_STRING}")
message(STATUS "fmt version ${fmt_VERSION_STRING}")
message(STATUS "gRPC version ${gRPC_VERSION_STRING}")
message(STATUS "Protobuf version ${protobuf_VERSION_STRING}")
message(STATUS "asio-grpc version ${asio-grpc_VERSION_STRING}")
message(STATUS "range-v3 version ${range-v3_VERSION_STRING}")
message(STATUS "abseil version ${absl_VERSION_STRING}")

message(STATUS "boost include ${Boost_INCLUDE_DIR}")
message(STATUS "fmt include ${fmt_INCLUDE_DIR}")
message(STATUS "gRPC include ${gRPC_INCLUDE_DIR}")
message(STATUS "Protobuf  ${protobuf_INCLUDE_DIR}")
message(STATUS "asio-grpc include ${asio-grpc_INCLUDE_DIR}")
message(STATUS "range-v3 include ${range-v3_INCLUDE_DIR}")
message(STATUS "abseil include ${absl_INCLUDE_DIR}")

message(STATUS "Protoc binary of conan installed package ${Protobuf_PROTOC_EXECUTABLE}")

# Log conan provided targets
get_property(importTargets DIRECTORY "${CMAKE_SOURCE_DIR}" PROPERTY IMPORTED_TARGETS)
message(STATUS "${importTargets}")